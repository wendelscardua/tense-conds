#!/usr/bin/ruby
# frozen_string_literal: true

# generate lookup tables for animation easing
#
# output: .s source

output = ARGV.shift

DROP_ANIM_FRAMES = 9
MIN_DROP_HEIGHT = 1
MAX_DROP_HEIGHT = 9

NEXT_COOKIE_FRAMES = 8

File.open(output, 'wb') do |f|
  f.puts <<~"ASM"
    .segment "RODATA"
    .export _drop_lut
    _drop_lut:
  ASM

  (MIN_DROP_HEIGHT..MAX_DROP_HEIGHT).each do |height|
    pos = 16 * height
    len = pos
    toggle = 0

    positions = []

    (1..DROP_ANIM_FRAMES).each do
      positions.unshift(pos)
      pos -= len / 3.0
      toggle += 1
      if toggle == 2
        toggle = 0
        len = pos
      end
    end

    deltas = positions.unshift(0).map(&:round).each_cons(2).map { |a, b| b - a }

    f.puts <<~"ASM"
      .byte #{deltas.map(&:to_s).join(', ')}
    ASM
  end

  f.puts <<~"ASM"
    .export _next_cookie_anim_lut
    _next_cookie_anim_lut:
  ASM

  positions = []
  pos = 0
  len = 42
  toggle = 0

  (1..NEXT_COOKIE_FRAMES).each do
    positions.push(pos)
    pos += len / 3.0
    toggle += 1
    if toggle == 2
      toggle = 0
      len /= 3.0
    end
  end

  deltas = positions.unshift(0).map(&:round).each_cons(2).map { |a, b| b - a }

  f.puts <<~"ASM"
    .byte #{deltas.map(&:to_s).join(', ')}
  ASM
end
