#!/bin/ruby

require 'optparse'

options = {}
OptionParser.new do |option_parser|
	#option_parser.banner = 'Usage: example.rb [options]'
	option_parser.default_argv = [
		'--void',
		'void',
		'--float',
		'float',
		'--string',
		'const char*'
	]

	option_parser.on('--void NAME', 'void type name') do |void_type_name|
		options[:void_type_name] = void_type_name
	end
	option_parser.on('--float NAME', 'float type name') do |float_type_name|
		options[:float_type_name] = float_type_name
	end
	option_parser.on('--string NAME', 'string type name') do |string_type_name|
		options[:string_type_name] = string_type_name
	end
end.parse!

p options

#code = IO.read('testfile')
#p code
