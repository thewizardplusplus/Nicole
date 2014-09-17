#!/usr/bin/env ruby

require 'optparse'
require 'pathname'

def testAndWrapName(name)
	raise "invalid name \"#{name}\"" unless /\w+/ =~ name
	"\\b#{name}\\b"
end

def parseOptions
	options = {
		:void => '\bvoid\b',
		:float => '\bfloat\b',
		:c_string => '\b(const\s+)?char(\s+const)?\s*\*',
		:indent => ''
	}
	OptionParser.new do |option_parser|
		option_parser.program_name = Pathname.new($0).basename
		option_parser.banner =
			"Usage: #{option_parser.program_name} [options] filename"

		option_parser.on('--void NAME', 'void type name') do |name|
			options[:void] = testAndWrapName name
		end
		option_parser.on('--float NAME', 'float type name') do |name|
			options[:float] = testAndWrapName name
		end
		option_parser.on('--c-string NAME', 'C string type name') do |name|
			options[:c_string] = testAndWrapName name
		end
		option_parser.on('--indent PREFIX', 'indent prefix') do |prefix|
			options[:indent] = prefix
		end
	end.parse!

	options[:filename] = ARGV.pop
	raise "need to specify a file to process" unless options[:filename]

	options
end

def unwrapName(name)
	name.gsub(/^\\b|\\b$/, '')
end

def processCode(code, options)
	subprograms = []
	code.scan(
		/
			(?<return_type> #{options[:void]} | #{options[:float]} )\s*
			# optional calling convention on Windows
			\w*?\s*
			(?<function_name> \w+ )\s*
			# parameters list...
			\(\s*
				# ...may be empty
				(
					# ...or void
					#{options[:void]}
					# ...or some parameters
					| (?<first_parameter>
						(?<parameter>
							# ...with type
							(
								#{options[:float]} | #{options[:c_string]}
							)\s*
							# ...and name
							\w+\s*
						)
					)
					(?<rest_parameters> (,\s*\g<parameter>)* )
				)?
			\)
		/x
	) do |
		return_type,
		function_name,
		first_parameter,
		parameter,
		rest_parameters,
	|
		subprogram = ''
		case return_type
			when unwrapName(options[:void])
				subprogram += "procedure #{function_name}/"
			when unwrapName(options[:float])
				subprogram += "function #{function_name}/"
		end

		number_of_parameters = 0
		first_parameter && number_of_parameters += 1
		rest_parameters && number_of_parameters += rest_parameters.count(',')
		subprogram += number_of_parameters.to_s

		subprograms << subprogram
	end

	subprograms
end

def printSubprograms(subprograms, options)
	subprograms.each do |subprogram|
		puts options[:indent] + subprogram
	end
end

begin
	options = parseOptions
	code = IO.read(options[:filename])
	subprograms = processCode(code, options)
	printSubprograms(subprograms, options)
rescue Exception => exception
	puts "Error: \"#{exception.message}\"."
end
