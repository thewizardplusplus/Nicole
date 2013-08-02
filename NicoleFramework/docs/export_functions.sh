#!/bin/bash

SOURCES_PATH="./../source/thewizardplusplus/nicole/framework/"

parseExportedFunctions() {
	files=$(find "$SOURCES_PATH" -name *.cpp)
	for file in $files
	do
		grep -Pzo "\bextern\s*\"C\"\s*(float|void)\s*\w+\s*\([^\)]*\)" "$file"
	done
}

processExportedFunctions() {
	counted_arguments=""
	if [[ "$2" != "framework" ]]
	then
		formatted_functions=$(echo $1 | sed "s/\bextern\s*\"C\"\s*\(float\|void\)\s*\(\w\+\)\s*(\([^)]*\))\s*/\1 \2\/\3\n/g")
		replaced_void_type=$(echo "$formatted_functions" | sed "s/^void/procedure/")
		replaced_float_type=$(echo "$replaced_void_type" | sed "s/^float/function/")
		replaced_argument_list=$(echo "$replaced_float_type" | sed "s/\(\/\|\,\)\s*\(float\s\|\(const\s\+char\s*\*\)\)\s*\w\+/\1#/g")
		counted_arguments=$(echo "$replaced_argument_list" | awk '{ counter = 0; number_of_symbols = split($0, symbols, ""); for (i = number_of_symbols; symbols[i] != "/" && i > 0; i--) if (symbols[i] == "#") counter++; if (i > 0) printf "%s%d\n", substr($0, 1, i), counter; }')
	else
		formatted_functions=$(echo $1 | sed "s/\bextern\s*\"C\"\s*\(float\|void\)\s*\(\w\+\)\s*(\([^)]*\))\s*/\1\2\"] =\3\n/g")
		replaced_void_type=$(echo "$formatted_functions" | sed "s/^void/byte_code_module.procedures[\"/")
		replaced_float_type=$(echo "$replaced_void_type" | sed "s/^float/byte_code_module.functions[\"/")
		replaced_argument_list=$(echo "$replaced_float_type" | sed "s/\(=\|\,\)\s*\(float\|\(const\s\+char\s*\*\)\)\s*\w\+/\1#/g")

		maximal_line_length=$(echo "$replaced_argument_list" | awk 'BEGIN { maximal_length = 0; } { current_length = index($0, "="); if (current_length > maximal_length) maximal_length = current_length; } END { print maximal_length; }')

		counted_arguments=$(echo "$replaced_argument_list" | awk -v maximal_length=$maximal_line_length '{ counter = 0; number_of_symbols = split($0, symbols, ""); for (i = number_of_symbols; symbols[i] != "=" && i > 0; i--) if (symbols[i] == "#") counter++; begin_of_line = substr($0, 1, i); number_of_spaces = maximal_length - length(begin_of_line); indent = " "; for (j = 0; j < number_of_spaces; j++) indent = indent " "; if (i > 0) printf "\t%s%s%d;\n", begin_of_line, indent, counter; }')
	fi
	sorted=$(echo "$counted_arguments" | sort)

	echo "$sorted";
}

if [[ -n "$1" && "$1" != "-f" && "$1" != "--framework" ]]
then
	SOURCES_PATH="$1"
fi

echo
echo "List of functions for Nicole program:"
exported_functions=$(parseExportedFunctions)
processExportedFunctions "$exported_functions"

if [[ "$1" == "-f" || "$1" == "--framework" || "$2" == "-f" || "$2" == "--framework" ]]
then
	echo
	echo "Code for Nicole compiler:"
	exported_functions=$(parseExportedFunctions)
	processExportedFunctions "$exported_functions" "framework"
fi
