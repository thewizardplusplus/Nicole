#!/bin/bash

SOURCES_PATH="./../source/thewizardplusplus/nicole/framework/"

parseExportedFunction() {
	files=$(find "$SOURCES_PATH" -name *.cpp)
	for file in $files
	do
		grep -P "^extern\s+\"C\"\s+(float|void)" "$file"
	done
}

processExportedFunction() {
	counted_arguments=""
	if [ "$2" != "framework" ]
	then
		formatted_functions=$(echo "$1" | sed "s/^extern\s\+\"C\"\s\+\(float\|void\)\s\+\(\w\+\)\s*(\(.*\))\s*{/\1 \2\/\3/")
		replaced_void_type=$(echo "$formatted_functions" | sed "s/^void/procedure/")
		replaced_float_type=$(echo "$replaced_void_type" | sed "s/^float/function/")
		replaced_argument_list=$(echo "$replaced_float_type" | sed "s/\(\/\|\,\)\s*\(float\|\(const\s\+char\s*\*\)\)\s\+\w\(\w\|\d\)*/\1#/g")
		counted_arguments=$(echo "$replaced_argument_list" | awk '{ counter = 0; number_of_symbols = split($0, symbols, ""); for (i = number_of_symbols; i > 0 && symbols[i] != "/"; i--) if (symbols[i] == "#") counter++; printf "%s%d\n", substr($0, 1, i), counter; }')
	else
		formatted_functions=$(echo "$1" | sed "s/^extern\s\+\"C\"\s\+\(float\|void\)\s\+\(\w\+\)\s*(\(.*\))\s*{/\1\2\"] =\3/")
		replaced_void_type=$(echo "$formatted_functions" | sed "s/^void/byte_code_module.procedures[\"/")
		replaced_float_type=$(echo "$replaced_void_type" | sed "s/^float/byte_code_module.functions[\"/")
		replaced_argument_list=$(echo "$replaced_float_type" | sed "s/\(=\|\,\)\s*\(float\|\(const\s\+char\s*\*\)\)\s\+\w\(\w\|\d\)*/\1#/g")

		maximal_line_length=$(echo "$replaced_argument_list" | awk 'BEGIN { maximal_length = 0; } { current_length = index($0, "="); if (current_length > maximal_length) maximal_length = current_length; } END { print maximal_length; }')

		counted_arguments=$(echo "$replaced_argument_list" | awk -v maximal_length=$maximal_line_length '{ counter = 0; number_of_symbols = split($0, symbols, ""); for (i = number_of_symbols; i > 0 && symbols[i] != "="; i--) if (symbols[i] == "#") counter++; begin_of_line = substr($0, 1, i); number_of_spaces = maximal_length - length(begin_of_line); indent = " "; for (i = 0; i < number_of_spaces; i++) indent = indent " "; printf "\t%s%s%d;\n", begin_of_line, indent, counter; }')
	fi
	sorted=$(echo "$counted_arguments" | sort)

	echo "$sorted";
}

generateCodeForNicoleCompiler() {
	echo
	echo "Code for Nicole compiler:"
	exported_function=$(parseExportedFunction)
	processExportedFunction "$exported_function" "framework"
}

if [[ ! -z "$1" && "$1" != "-f" && "$1" != "--framework" ]]
then
	SOURCES_PATH="$1"
fi

echo "List of functions for Nicole-program:"
exported_function=$(parseExportedFunction)
processExportedFunction "$exported_function"

if [[ "$1" = "-f" || "$1" = "--framework" || "$2" = "-f" || "$2" = "--framework" ]]
then
	generateCodeForNicoleCompiler
fi
