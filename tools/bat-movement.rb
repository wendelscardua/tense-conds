#!/usr/bin/ruby
# frozen_string_literal: true

output = ARGV.shift

LOOP_FRAMES = 60

AMPLITUDE = 0x2000
LENGTH = 4

File.open(output, 'wb') do |f|
  positions = (0..LOOP_FRAMES).map do |frame_index|
    radians = Math::PI * 2 * (frame_index / LOOP_FRAMES.to_f)

    AMPLITUDE * Math.sin(LENGTH * radians)
  end

  deltas = positions.map(&:round)
                    .each_cons(2)
                    .map { |a, b| b - a }
                    .map { |delta| delta.negative? ? 0x10000 + delta : delta }

  f.puts <<~"ASM"
    .segment "RODATA"
    sine_deltas:
  ASM

  deltas.each do |delta|
    f.puts ".word $#{format('%04x', delta)}"
  end
end
