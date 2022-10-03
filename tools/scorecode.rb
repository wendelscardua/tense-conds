#!/usr/bin/ruby
# frozen_string_literal: true

def digit(d)
  if d =~/[0-9]/
    d.ord - '0'.ord
  elsif d =~ /[A-Z]/
    d.ord - 'A'.ord + 10
  elsif d =~ /[a-z]/
    d.ord - 'a'.ord + 36
  elsif d == '?'
    62
  elsif d == '@'
    63
  else
    raise "What is #{d}"
  end
end

def decode(base64ish)
  puts base64ish
  (digit(base64ish[0]) * 262144 + digit(base64ish[1]) * 4096 + digit(base64ish[2]) * 64 + digit(base64ish[3])).tap do |v|
    puts v
  end
end

code = ARGV.shift

n = decode(code[4...8])

score = seed = extra = 0
8.times {
  extra <<= 1
  extra |= (n&1)
  n>>=1
  seed <<=1
  seed |= (n&1)
  n>>=1
  score <<= 1
  score|= (n&1)
  n>>=1
}

n = decode(code[0...4])
8.times {
  extra <<= 1
  extra |= (n&1)
  n>>=1
  seed <<=1
  seed |= (n&1)
  n>>=1
  score <<= 1
  score|= (n&1)
  n>>=1
}

puts "score = #{score ^ 0xcafe}"
puts "seed = #{seed.to_s(16)}"
puts "ok" if (score ^ seed) == extra
