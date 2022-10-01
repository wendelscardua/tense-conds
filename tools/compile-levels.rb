#!/usr/bin/ruby
# frozen_string_literal: true

# represents a tiled dungeon in assembly
# arguments:
# - levels folder (input)
# - .s file (output)

require 'bundler/inline'

gemfile do
  source 'https://rubygems.org'

  gem 'nokogiri'
  gem 'pry'
end

require 'json'
require_relative './rle'

LevelMetadata = Struct.new(:filename, :level, :label)

class LevelsCompiler
  def initialize(levels_folder, output_file)
    @levels_folder = levels_folder
    @output_file = output_file
    # TODO: read from pawns.tsx
    @entity_types = {
      '65' => 'Player'
    }
  end

  def process
    File.open(@output_file, 'wb') do |f|
      # TODO: dynamic list of entity types?
      f.puts <<~"PREAMBLE"
             .enum entity_type
                Player
             .endenum
             .segment "RODATA"
             .export _levels
             _levels:
      PREAMBLE

      level_metadata =
        Dir[File.join(@levels_folder, '*.tmx')].map { |name| metadatify(name) }
                                               .sort_by(&:level)


      level_metadata.each do |datum|
        f.puts <<~"LEVEL_LIST"
          .word #{datum.label}
        LEVEL_LIST
      end

      level_metadata.each do |datum|
        f.puts "#{datum.label}:"

        f.puts read_tmx_file(datum.filename)
      end
    end
  end

  private

  def metadatify(level_file)
    matches = level_file.match(/level-(?<level>\d+)[.]tmx\z/)
    raise "Invalid level filename #{level_file}" unless matches

    level = matches['level'].to_i

    LevelMetadata.new(level_file,
                      level,
                      "level_#{level}")
  end

  def numberify(coordinate)
    coordinate.to_f.round.to_i
  end

  def fmt(byte)
    format('$%02x', byte)
  end

  def read_tmx_file(tmx_file)
    document = Nokogiri::XML(File.read(tmx_file))

    entity_payload = []

    objects = document.xpath('//objectgroup/object')

    entity_payload << objects.count

    objects.each do |object|
      entity_payload << "entity_type::#{@entity_types[object['gid']]}"

      # tile object coordinates refer to the bottom-left of the tile
      object_x = numberify(object['x'])
      object_y = numberify(object['y']) - 0x10

      entity_payload << fmt(object_x / 0x10)
      entity_payload << fmt(object_y / 0x10)
    end

    metatiles = document.xpath('//layer/data')
                        .text
                        .scan(/\d+/)
                        .map { |t| t.to_i - 1 }

    rle_tiles = RLE.rle(metatiles)
    <<~"TMX_DATA"
      ; entities
        .byte #{entity_payload.join(', ')}
      ; map
        .byte #{rle_tiles.join(', ')}
    TMX_DATA
  end
end

levels_folder, output_file = ARGV

LevelsCompiler.new(levels_folder, output_file).process
