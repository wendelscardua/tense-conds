#!/usr/bin/ruby
# frozen_string_literal: true

#
# computes cycles estimate for procs
# doesn't know about loops or branches, though
#
# input: .s source

input = ARGV.shift

# worst case scenario assumed (crossing pages, successful branches)
INSTRUCTION_TO_CYCLES = {
  '00' => 7,
  '01' => 6,
  '05' => 3,
  '06' => 5,
  '08' => 3,
  '09' => 2,
  '0A' => 2,
  '0E' => 6,
  '10' => 5,
  '11' => 6,
  '15' => 4,
  '16' => 6,
  '18' => 2,
  '19' => 5,
  '1D' => 5,
  '1E' => 7,
  '20' => 6,
  '21' => 6,
  '24' => 3,
  '25' => 3,
  '26' => 5,
  '28' => 4,
  '29' => 2,
  '2A' => 2,
  '2C' => 4,
  '2D' => 4,
  '2E' => 6,
  '30' => 5,
  '31' => 6,
  '35' => 4,
  '36' => 6,
  '38' => 2,
  '39' => 5,
  '3D' => 5,
  '3E' => 7,
  '40' => 6,
  '41' => 6,
  '45' => 3,
  '46' => 5,
  '48' => 3,
  '49' => 2,
  '4A' => 2,
  '4C' => 3,
  '4D' => 4,
  '4E' => 6,
  '50' => 5,
  '51' => 6,
  '55' => 4,
  '56' => 6,
  '58' => 2,
  '59' => 5,
  '5D' => 5,
  '5E' => 7,
  '60' => 6,
  '61' => 6,
  '65' => 3,
  '66' => 5,
  '68' => 4,
  '69' => 2,
  '6A' => 2,
  '6C' => 5,
  '6D' => 4,
  '6E' => 6,
  '70' => 5,
  '71' => 6,
  '75' => 4,
  '76' => 6,
  '78' => 2,
  '79' => 5,
  '7D' => 5,
  '7E' => 7,
  '81' => 6,
  '84' => 3,
  '85' => 3,
  '86' => 3,
  '88' => 2,
  '8A' => 2,
  '8C' => 4,
  '8D' => 4,
  '8E' => 4,
  '90' => 5,
  '91' => 6,
  '94' => 4,
  '95' => 4,
  '96' => 4,
  '98' => 2,
  '99' => 5,
  '9A' => 2,
  '9D' => 5,
  'A0' => 2,
  'A1' => 6,
  'A2' => 2,
  'A4' => 3,
  'A5' => 3,
  'A6' => 3,
  'A8' => 2,
  'A9' => 2,
  'AA' => 2,
  'AC' => 4,
  'AD' => 4,
  'AE' => 4,
  'B0' => 5,
  'B1' => 6,
  'B4' => 4,
  'B5' => 4,
  'B6' => 4,
  'B8' => 2,
  'B9' => 5,
  'BA' => 2,
  'BC' => 5,
  'BD' => 5,
  'BE' => 5,
  'C0' => 2,
  'C1' => 6,
  'C4' => 3,
  'C5' => 3,
  'C6' => 5,
  'C8' => 2,
  'C9' => 2,
  'CA' => 2,
  'CC' => 4,
  'CD' => 4,
  'CE' => 6,
  'D0' => 5,
  'D1' => 6,
  'D5' => 4,
  'D6' => 6,
  'D8' => 2,
  'D9' => 5,
  'DD' => 5,
  'DE' => 7,
  'E0' => 2,
  'E1' => 6,
  'E4' => 3,
  'E5' => 3,
  'E6' => 5,
  'E8' => 2,
  'E9' => 2,
  'EA' => 2,
  'EC' => 4,
  'ED' => 4,
  'EE' => 6,
  'F0' => 5,
  'F1' => 6,
  'F5' => 4,
  'F6' => 6,
  'F8' => 2,
  'F9' => 5,
  'FD' => 5,
  'FE' => 7
}.freeze

CODE_SEGMENTS = ['CODE']

current_segment = nil
current_proc = nil
cycles = 0

IO.popen(['ca65', input, '-o', '/dev/null', '-l', '/dev/stdout'], 'rb') do |pipe|
  pipe.each_line(chomp: true) do |line|
    if (match = line.match(/\.segment\s+"(?<segment_name>\w+)"/))
      current_segment = match['segment_name']
    end

    next unless CODE_SEGMENTS.include?(current_segment)

    if (match = line.match(/\.proc\s+(?<proc_name>\w+)/))
      current_proc = match['proc_name']
      cycles = 0
    elsif line.match(/\.endproc/)
      puts "#{current_proc}: #{cycles}"
    elsif (match = line.match(/\A[^r]*r\s\d\s+(?<instruction>[0-9A-F]{2})/))
      instruction = match['instruction']
      if (instruction_cycles = INSTRUCTION_TO_CYCLES[instruction])
        cycles += instruction_cycles
      else
        puts "Unknown instruction: #{instruction}"
        puts "Line: #{line}"
        exit 1
      end
    end
  end
end
