#!/usr/bin/env ruby

require 'optparse'
require 'pathname'

def parseOptions
	options = {:void => 'void', :float => 'float', :c_string => 'const char*'}
	OptionParser.new do |option_parser|
		option_parser.program_name = Pathname.new($0).basename
		option_parser.banner =
			"Usage: #{option_parser.program_name} [options] filename"

		option_parser.on('--void NAME', 'void type name') do |name|
			options[:void] = name
		end
		option_parser.on('--float NAME', 'float type name') do |name|
			options[:float] = name
		end
		option_parser.on('--c-string NAME', 'C string type name') do |name|
			options[:c_string] = name
		end
	end.parse!

	options[:filename] = ARGV.pop
	raise "need to specify a file to process" unless options[:filename]

	options
end

begin
	options = parseOptions

	code = IO.read(options[:filename])
	p code
rescue Exception => exception
	puts "Error: \"#{exception.message}\"."
end
