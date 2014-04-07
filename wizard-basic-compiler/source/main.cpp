#include <string>
#include <map>
#include <list>
#include <stack>
#include <set>
//TODO: уточнить порядок и необходимость подключения
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>

// основано на http://sourceforge.net/p/predef/wiki/OperatingSystems/
#if \
	defined(__gnu_linux__) \
	|| defined(__linux__) \
	|| defined(linux) \
	|| defined(__linux)
	#define OS_LINUX
#elif \
	defined(_WIN32) \
	|| defined(_WIN64) \
	|| defined(__WIN32__) \
	|| defined(__TOS_WIN__) \
	|| defined(__WINDOWS__)
	#define OS_WINDOWS
#else
	#error: \
		an attempt to compile for an unsupported platform; \
		supports Linux and Windows x86 and x86-64.
#endif

enum MessageType {
	MESSAGE_INFORMATION,
	MESSAGE_ERROR
};
enum OperatorAssociativity {
	ASSOCIATIVITY_LEFT,
	ASSOCIATIVITY_RIGHT
};
enum TokenizeState {
	TOKENIZE_RESET,
	TOKENIZE_NUMBER,
	TOKENIZE_IDENTIFIER
};
typedef std::map<size_t, std::string> CodeLines;
typedef std::list<std::string> StringList;
typedef std::map<std::string, std::string> AliasMap;
struct ByteCodeMnemonic {
	size_t line_number;
	std::string mnemonic;
	std::string operand;

	ByteCodeMnemonic(
		size_t line_number,
		std::string mnemonic,
		std::string operand = ""
	) :
		line_number(line_number),
		mnemonic(mnemonic),
		operand(operand)
	{}
};
typedef std::list<ByteCodeMnemonic> ByteCode;
enum ExpressionResultFlag {
	EXPRESSION_NOTHING =        0,
	EXPRESSION_WERE_CONVERTED = 1 << 0,
	EXPRESSION_IS_RESULT =      1 << 1
};
struct CompileExpressionResult {
	ByteCode byte_code;
	unsigned char flags;
};
typedef std::map<std::string, size_t> SubprogramMap;
typedef std::stack<size_t> ArgumentCounterStack;
typedef std::set<std::string> VariableSet;
struct Library {
	std::string path;
	std::string name;
};
typedef std::list<Library> LibrariesList;
struct ByteCodeModule {
	ByteCode byte_code;
	VariableSet variables;
	SubprogramMap procedures;
	SubprogramMap functions;
	LibrariesList libraries;
};
typedef std::map<std::string, float> InbuildVariableMap;
typedef std::map<std::string, std::string> InbuildStringConstantMap;
typedef std::stack<long> ConditionIdStack;

template<typename Type>
std::string ConvertToString(Type value) {
	std::ostringstream out;
	out << value;

	return out.str();
}

template<typename Type>
Type ConvertFromString(const std::string& value) {
	Type result = Type();

	std::istringstream in(value);
	in >> result;

	return result;
}

std::string StringTrim(const std::string& string) {
	std::string result = string;

	std::string::iterator result_begin = result.begin();
	std::unary_negate<std::pointer_to_unary_function<int, int> > predicate =
		std::not1(std::ptr_fun<int, int>(std::isspace));
	result.erase(result_begin, std::find_if(result_begin, result.end(),
		predicate));
	result.erase(std::find_if(result.rbegin(), result.rend(), predicate).base(),
		result.end());

	return result;
}

void ShowMessage(const std::string& message, MessageType type =
	MESSAGE_INFORMATION)
{
	switch (type) {
	case MESSAGE_ERROR:
		std::cerr << message << std::endl;
		break;
	case MESSAGE_INFORMATION:
	default:
		std::cout << message << std::endl;
		break;
	}
}

void ShowCodeLines(const CodeLines& code_lines) {
	std::string result = "Code:\n";
	CodeLines::const_iterator i = code_lines.begin();
	for (; i != code_lines.end(); ++i) {
		result += ConvertToString(i->first) + " " + i->second + "\n";
	}

	ShowMessage(result);
}

void ShowByteCode(const ByteCodeModule& byte_code_module) {
	std::string result = "Libraries:\n";
	LibrariesList::const_iterator i = byte_code_module.libraries.begin();
	for (; i != byte_code_module.libraries.end(); ++i) {
		Library library = *i;
		result += " -L" + library.path + " -l" + library.name + "\n";
	}

	result += "\nFunctions:\n";
	SubprogramMap::const_iterator j = byte_code_module.functions.begin();
	for (; j != byte_code_module.functions.end(); ++j) {
		result += j->first + " (" + ConvertToString(j->second) +
			(j->second != 1 ? " arguments)" : " argument)") + "\n";
	}

	result += "\nVariables:\n";
	VariableSet::const_iterator k = byte_code_module.variables.begin();
	for (; k != byte_code_module.variables.end(); ++k) {
		result += *k + "\n";
	}

	result += "\nByte code:\n";
	ByteCode::const_iterator l = byte_code_module.byte_code.begin();
	for (; l != byte_code_module.byte_code.end(); ++l) {
		result += ConvertToString((*l).line_number) + " " + (*l).mnemonic + " "
			+ (*l).operand + "\n";
	}

	ShowMessage(result);
}

void ProcessError(const std::string& description) {
	ShowMessage(description, MESSAGE_ERROR);
	std::exit(EXIT_FAILURE);
}

bool IsNumber(const std::string& string) {
	std::ptrdiff_t number_of_digits = std::count_if(string.begin(), string
		.end(), std::ptr_fun<int, int>(std::isdigit));
	std::ptrdiff_t number_of_dots = std::count_if(string.begin(), string.end(),
		std::bind1st(std::equal_to<char>(), '.'));
	return number_of_digits != 0 && (number_of_dots == 0 || number_of_dots == 1)
		&& string[0] != '.' && string[string.size() - 1] != '.' && static_cast<
		std::size_t>(number_of_digits + number_of_dots) == string.length();
}

bool IsString(const std::string& string) {
	return string.size() >= 2 && string[0] == '"' && string[string.size() - 1]
		== '"';
}

bool IsIdentifier(const std::string& string) {
	std::ptrdiff_t number_of_digits = std::count_if(string.begin(), string
		.end(), std::ptr_fun<int, int>(std::isdigit));
	std::ptrdiff_t number_of_letters = std::count_if(string.begin(), string
		.end(), std::ptr_fun<int, int>(std::isalpha));
	std::ptrdiff_t number_of_underscores = std::count_if(string.begin(), string
		.end(), std::bind1st(std::equal_to<char>(), '_'));
	return (number_of_letters != 0 || number_of_underscores != 0) &&
		!std::isdigit(string[0]) && static_cast<std::size_t>(number_of_digits +
		number_of_letters + number_of_underscores) == string.length();
}

bool IsOperator(const std::string& string) {
	return string == "!" || string == "*" || string == "/" || string == "+" ||
		string == "-" || string == "$" || string == "<" || string == ">" ||
		string == "=" || string == "&" || string == "|";
}

OperatorAssociativity GetAssociativity(const std::string& string) {
	if (string != "!") {
		return ASSOCIATIVITY_LEFT;
	} else {
		return ASSOCIATIVITY_RIGHT;
	}
}

unsigned long GetPrecedence(const std::string& string) {
	if (string == "!") {
		return 6;
	} else if (string == "*" || string == "/") {
		return 5;
	} else if (string == "+" || string == "-" || string == "$") {
		return 4;
	} else if (string == "<" || string == ">") {
		return 3;
	} else if (string == "=") {
		return 2;
	} else if (string == "&") {
		return 1;
	} else {
		return 0;
	}
}

std::string ProcessCommandLineArguments(int number_of_arguments, char**
	arguments)
{
	if (number_of_arguments != 2) {
		ProcessError("Invalid number of arguments. Expected one argument.");
	} else {
		std::string argument = arguments[1];
		if (argument == "-v" || argument == "--version") {
			ShowMessage("Nicole Compiler, compiler for the programming "
				"language Nicole, version 1.0.0a.\n"
				"(c) 2013 thewizardplusplus, http://thewizardplusplus.ru.");
			std::exit(EXIT_SUCCESS);
		} else if (argument == "-h" || argument == "--help") {
			ShowMessage("Usage:\n"
				"\tnicole_compiler (option|filename)\n"
				"\n"
				"Option:\n"
				"\t-v, --version - display compiler version;\n"
				"\t-h, --help -    display this information.");
			std::exit(EXIT_SUCCESS);
		} else {
			return argument;
		}
	}

	return std::string();
}

CodeLines FileRead(const std::string& filename) {
	std::ifstream in(filename.c_str());
	if (!in.is_open()) {
		ProcessError("Unable to open source file \"" + filename + "\".");
	}

	CodeLines code_lines;
	size_t line_number = 0;
	std::string buffer;
	while (in.good()) {
		std::getline(in, buffer);
		code_lines[line_number] = buffer;
		line_number++;
	}

	return code_lines;
}

void FileWrite(const std::string& filename, const std::string& text) {
	std::ofstream out(filename.c_str());
	if (!out.is_open()) {
		ProcessError("Unable to open target file \"" + filename + "\".");
	}

	out << text;
}

CodeLines Preprocess(const CodeLines& code_lines) {
	CodeLines result;
	CodeLines::const_iterator i = code_lines.begin();
	for (; i != code_lines.end(); ++i) {
		std::string code_line = StringTrim(i->second);
		if (code_line.empty()) {
			continue;
		} else if (code_line.substr(0, 5) == "note " || code_line == "note") {
			continue;
		} else {
			result[i->first] = code_line;
		}
	}

	return result;
}

StringList Tokenize(const std::string& string, size_t line_number) {
	StringList tokens;
	std::string accumulator;
	TokenizeState state = TOKENIZE_RESET;

	for (size_t i = 0; i < string.size(); i++) {
		char symbol = string[i];
		std::string symbol_in_string = std::string(1, symbol);
		if (std::isspace(symbol)) {
			continue;
		} else if (IsOperator(symbol_in_string) || symbol == '(' || symbol ==
			')' || symbol == ',')
		{
			if (!accumulator.empty()) {
				if (state == TOKENIZE_NUMBER && accumulator[accumulator
					.size() - 1] == '.')
				{
					ProcessError("Unfinished number on line " + ConvertToString(
						line_number) + ".");
				}

				tokens.push_back(accumulator);
				accumulator.clear();
			}
			state = TOKENIZE_RESET;

			tokens.push_back(symbol_in_string);
		} else if (std::isdigit(symbol)) {
			accumulator += symbol;
			if (state == TOKENIZE_RESET) {
				state = TOKENIZE_NUMBER;
			}
		} else if (symbol == '.' && state == TOKENIZE_NUMBER) {
			accumulator += symbol;
		} else if ((std::isalpha(symbol) || symbol == '_') && (state ==
			TOKENIZE_RESET || state == TOKENIZE_IDENTIFIER))
		{
			accumulator += symbol;
			if (state == TOKENIZE_RESET) {
				state = TOKENIZE_IDENTIFIER;
			}
		} else {
			ProcessError("Unexpected symbol \"" + symbol_in_string + "\" in "
				"expression on line " + ConvertToString(line_number) + ".");
		}
	}

	if (!accumulator.empty()) {
		if (state == TOKENIZE_NUMBER && accumulator[accumulator
			.size() - 1] == '.')
		{
			ProcessError("Unfinished number on line " + ConvertToString(
				line_number) + ".");
		}

		tokens.push_back(accumulator);
	}

	return tokens;
}

std::string GetSubprogramNameByAlias(const std::string& alias) {
	AliasMap alias_map;
	#ifdef OS_LINUX
	alias_map["!"] =        "NumberNot";
	alias_map["*"] =        "NumberMul";
	alias_map["/"] =        "NumberDiv";
	alias_map["+"] =        "NumberAdd";
	alias_map["-"] =        "NumberSub";
	alias_map["$"] =        "ArrayAppend";
	alias_map["<"] =        "NumberLs";
	alias_map[">"] =        "NumberGt";
	alias_map["="] =        "NumberEq";
	alias_map["&"] =        "NumberAnd";
	alias_map["|"] =        "NumberOr";
	alias_map["ToString"] = "ArrayCreateFromNumber";
	alias_map["ToNumber"] = "ArrayConvertToNumber";
	#elif defined(OS_WINDOWS)
	alias_map["!"] =        "_NumberNot";
	alias_map["*"] =        "_NumberMul";
	alias_map["/"] =        "_NumberDiv";
	alias_map["+"] =        "_NumberAdd";
	alias_map["-"] =        "_NumberSub";
	alias_map["$"] =        "_ArrayAppend";
	alias_map["<"] =        "_NumberLs";
	alias_map[">"] =        "_NumberGt";
	alias_map["="] =        "_NumberEq";
	alias_map["&"] =        "_NumberAnd";
	alias_map["|"] =        "_NumberOr";
	alias_map["ToString"] = "_ArrayCreateFromNumber";
	alias_map["ToNumber"] = "_ArrayConvertToNumber";
	#endif

	if (alias_map.count(alias) == 1) {
		return alias_map[alias];
	} else {
		#ifdef OS_LINUX
		return alias;
		#elif defined(OS_WINDOWS)
		return "_" + alias;
		#endif
	}
}

CompileExpressionResult CompileExpression(const StringList& tokens, const
	SubprogramMap& procedures, SubprogramMap& functions, size_t line_number)
{
	CompileExpressionResult result;
	StringList stack;
	ArgumentCounterStack argument_counter_stack;
	bool push_operation_were = false;
	bool conversion_is_last = false;
	size_t procedures_counter = 0;

	StringList::const_iterator i = tokens.begin();
	for (; i != tokens.end(); ++i) {
		std::string token = *i;
		if (IsNumber(token)) {
			result.byte_code.push_back(ByteCodeMnemonic(line_number, "push_n", token));
			result.flags |= EXPRESSION_IS_RESULT;
			push_operation_were = true;
		} else if (IsIdentifier(token)) {
			std::string subprogram_name = GetSubprogramNameByAlias(token);
			if (!IsOperator(token) && (procedures.count(subprogram_name) == 1 ||
				functions.count(subprogram_name) == 1))
			{
				stack.push_back(token);
				argument_counter_stack.push(0);
			} else {
				result.byte_code.push_back(ByteCodeMnemonic(line_number, "push_v", token));
				result.flags |= EXPRESSION_IS_RESULT;
				push_operation_were = true;
			}
		} else if (token == ",") {
			while (true) {
				if (stack.empty()) {
					ProcessError("Missed subprogram argument separator or "
						"opening bracket on line " + ConvertToString(
						line_number) + ".");
				}

				std::string token_from_stack = stack.back();
				if (token_from_stack != "(") {
					std::string subprogram_name = GetSubprogramNameByAlias(
						token_from_stack);
					result.byte_code.push_back(ByteCodeMnemonic(line_number, "call",
						subprogram_name));
					stack.pop_back();
					conversion_is_last = false;
					if (!IsOperator(token_from_stack) && procedures.count(
						subprogram_name) == 1)
					{
						procedures_counter++;
						result.flags &= ~EXPRESSION_IS_RESULT;
					} else {
						result.flags |= EXPRESSION_IS_RESULT;
						push_operation_were = true;
					}
				} else {
					break;
				}
			}

			if (!argument_counter_stack.empty()) {
				if (push_operation_were) {
					argument_counter_stack.top()++;
					push_operation_were = false;
				}
			} else {
				ProcessError("Subprogram argument separator without subprogram "
					"on line " + ConvertToString(line_number) + ".");
			}
		} else if (IsOperator(token)) {
			while (true) {
				if (stack.empty()) {
					break;
				}

				std::string token_from_stack = stack.back();
				if (!IsOperator(token_from_stack)) {
					break;
				}

				if ((GetAssociativity(token) == ASSOCIATIVITY_LEFT &&
					GetPrecedence(token) <= GetPrecedence(token_from_stack)) ||
					(GetAssociativity(token) == ASSOCIATIVITY_RIGHT &&
					GetPrecedence(token) < GetPrecedence(token_from_stack)))
				{
					std::string subprogram_name = GetSubprogramNameByAlias(
						token_from_stack);
					result.byte_code.push_back(ByteCodeMnemonic(line_number, "call",
						subprogram_name));
					stack.pop_back();
					conversion_is_last = false;
					if (!IsOperator(token_from_stack) && procedures.count(
						subprogram_name) == 1)
					{
						procedures_counter++;
						result.flags &= ~EXPRESSION_IS_RESULT;
					} else {
						result.flags |= EXPRESSION_IS_RESULT;
						push_operation_were = true;
					}
				} else {
					break;
				}
			}

			stack.push_back(token);
		} else if (token == "(") {
			stack.push_back(token);
		} else if (token == ")") {
			while (true) {
				if (stack.empty()) {
					ProcessError("Missed opening bracket on line " +
						ConvertToString(line_number) + ".");
				}

				std::string token_from_stack = stack.back();
				stack.pop_back();
				if (token_from_stack != "(") {
					std::string subprogram_name = GetSubprogramNameByAlias(
						token_from_stack);
					result.byte_code.push_back(ByteCodeMnemonic(line_number, "call",
						subprogram_name));
					conversion_is_last = false;
					if (!IsOperator(token_from_stack) && procedures.count(
						subprogram_name) == 1)
					{
						procedures_counter++;
						result.flags &= ~EXPRESSION_IS_RESULT;
					} else {
						result.flags |= EXPRESSION_IS_RESULT;
						push_operation_were = true;
					}
				} else {
					break;
				}
			}

			if (!argument_counter_stack.empty() && push_operation_were) {
				argument_counter_stack.top()++;
				push_operation_were = false;
			}

			if (!stack.empty()) {
				std::string token_from_stack = stack.back();
				std::string subprogram_name = GetSubprogramNameByAlias(
					token_from_stack);
				if (!IsOperator(token_from_stack) && (procedures.count(
					subprogram_name) == 1 || functions.count(subprogram_name) ==
					1))
				{
					size_t expected_argument_number = 0;
					if (procedures.count(subprogram_name) == 1) {
						expected_argument_number = procedures.find(
							subprogram_name)->second;
					} else if (functions.count(subprogram_name) == 1) {
						expected_argument_number = functions.find(
							subprogram_name)->second;
					}
					size_t real_argument_number = argument_counter_stack.top();
					if (real_argument_number != expected_argument_number) {
						ProcessError("Invalid number of arguments for "
							"subprogram \"" + token_from_stack + "\" (expected "
							+ ConvertToString(expected_argument_number) +
							" argument" + (expected_argument_number != 1 ? "s" :
							"") + ", but gots " + ConvertToString(
							real_argument_number) + ") on line " +
							ConvertToString(line_number) + ".");
					}

					if (subprogram_name != "c_string") {
						result.byte_code.push_back(ByteCodeMnemonic(line_number, "call",
							subprogram_name));
						conversion_is_last = false;
						if (procedures.count(subprogram_name) == 1) {
							procedures_counter++;
							result.flags &= ~EXPRESSION_IS_RESULT;
						} else {
							result.flags |= EXPRESSION_IS_RESULT;
							push_operation_were = true;
						}
					} else {
						result.byte_code.push_back(ByteCodeMnemonic(line_number, "to_str"));
						result.flags |= EXPRESSION_WERE_CONVERTED;
						conversion_is_last = true;
						result.flags |= EXPRESSION_IS_RESULT;
						push_operation_were = true;
					}
					stack.pop_back();
					argument_counter_stack.pop();
				}
			}
		} else {
			ProcessError("Unknown expression token \"" + token + "\" on line " +
				ConvertToString(line_number) + ".");
		}
	}

	while (!stack.empty()) {
		std::string token_from_stack = stack.back();
		if (token_from_stack == "(") {
			ProcessError("Missed closing bracket on line " + ConvertToString(
				line_number) + ".");
		}

		std::string subprogram_name = GetSubprogramNameByAlias(
			token_from_stack);
		result.byte_code.push_back(ByteCodeMnemonic(line_number, "call", subprogram_name));
		stack.pop_back();
		conversion_is_last = false;
		if (procedures.count(subprogram_name) == 1) {
			procedures_counter++;
			result.flags &= ~EXPRESSION_IS_RESULT;
		} else {
			result.flags |= EXPRESSION_IS_RESULT;
		}
	}

	if (conversion_is_last) {
		ProcessError("Illegal conversion on line " + ConvertToString(
			line_number) + ".");
	}
	if (procedures_counter > 1 || (procedures_counter == 1 && result.flags &
		EXPRESSION_IS_RESULT))
	{
		ProcessError("Using result of procedure on line " + ConvertToString(
			line_number) + ".");
	}

	return result;
}

ByteCodeModule Compile(const CodeLines& code_lines, const InbuildVariableMap&
	inbuild_variables, const InbuildStringConstantMap& inbuild_string_constants)
{
	ByteCodeModule byte_code_module;
	#ifdef OS_LINUX
	byte_code_module.functions["c_string"] =                                1;
	byte_code_module.functions["ArrayAppend"] =                             2;
	byte_code_module.functions["ArrayConvertToNumber"] =                    1;
	byte_code_module.functions["ArrayCreate"] =                             1;
	byte_code_module.functions["ArrayCreateFromNumber"] =                   1;
	byte_code_module.functions["ArrayCreateFromString"] =                   1;
	byte_code_module.functions["ArrayGetSize"] =                            1;
	byte_code_module.functions["ArrayItemGet"] =                            2;
	byte_code_module.functions["FileOpen"] =                                2;
	byte_code_module.functions["FileRead"] =                                2;
	byte_code_module.functions["FileReadAll"] =                             1;
	byte_code_module.functions["GetOs"] =                                   0;
	byte_code_module.functions["MathAngle"] =                               2;
	byte_code_module.functions["MathArccos"] =                              1;
	byte_code_module.functions["MathArcsin"] =                              1;
	byte_code_module.functions["MathArctg"] =                               1;
	byte_code_module.functions["MathCos"] =                                 1;
	byte_code_module.functions["MathExponent"] =                            1;
	byte_code_module.functions["MathInteger"] =                             1;
	byte_code_module.functions["MathLg"] =                                  1;
	byte_code_module.functions["MathLn"] =                                  1;
	byte_code_module.functions["MathModulus"] =                             1;
	byte_code_module.functions["MathPower"] =                               2;
	byte_code_module.functions["MathRandom"] =                              2;
	byte_code_module.functions["MathSin"] =                                 1;
	byte_code_module.functions["MathSquareRoot"] =                          1;
	byte_code_module.functions["MathTg"] =                                  1;
	byte_code_module.functions["NumberAdd"] =                               2;
	byte_code_module.functions["NumberAnd"] =                               2;
	byte_code_module.functions["NumberDiv"] =                               2;
	byte_code_module.functions["NumberEq"] =                                2;
	byte_code_module.functions["NumberGt"] =                                2;
	byte_code_module.functions["NumberLs"] =                                2;
	byte_code_module.functions["NumberMul"] =                               2;
	byte_code_module.functions["NumberNot"] =                               1;
	byte_code_module.functions["NumberOr"] =                                2;
	byte_code_module.functions["NumberSub"] =                               2;
	byte_code_module.functions["TimerGetElapsedTimeInUs"] =                 0;
	byte_code_module.procedures["ArrayClearMemoryAfterConvertsToStrings"] = 0;
	byte_code_module.procedures["ArrayDelete"] =                            1;
	byte_code_module.procedures["ArrayDeleteAll"] =                         0;
	byte_code_module.procedures["ArrayItemAppend"] =                        2;
	byte_code_module.procedures["ArrayItemInsert"] =                        3;
	byte_code_module.procedures["ArrayItemRemove"] =                        2;
	byte_code_module.procedures["ArrayItemSet"] =                           3;
	byte_code_module.procedures["Exit"] =                                   0;
	byte_code_module.procedures["FileClose"] =                              1;
	byte_code_module.procedures["FileCloseAll"] =                           0;
	byte_code_module.procedures["FileDelete"] =                             1;
	byte_code_module.procedures["FileWrite"] =                              2;
	byte_code_module.procedures["Show"] =                                   1;
	byte_code_module.procedures["TimerStart"] =                             0;
	#elif defined(OS_WINDOWS)
	byte_code_module.functions["_c_string"] =                                1;
	byte_code_module.functions["_ArrayAppend"] =                             2;
	byte_code_module.functions["_ArrayConvertToNumber"] =                    1;
	byte_code_module.functions["_ArrayCreate"] =                             1;
	byte_code_module.functions["_ArrayCreateFromNumber"] =                   1;
	byte_code_module.functions["_ArrayCreateFromString"] =                   1;
	byte_code_module.functions["_ArrayGetSize"] =                            1;
	byte_code_module.functions["_ArrayItemGet"] =                            2;
	byte_code_module.functions["_FileOpen"] =                                2;
	byte_code_module.functions["_FileRead"] =                                2;
	byte_code_module.functions["_FileReadAll"] =                             1;
	byte_code_module.functions["_GetOs"] =                                   0;
	byte_code_module.functions["_MathAngle"] =                               2;
	byte_code_module.functions["_MathArccos"] =                              1;
	byte_code_module.functions["_MathArcsin"] =                              1;
	byte_code_module.functions["_MathArctg"] =                               1;
	byte_code_module.functions["_MathCos"] =                                 1;
	byte_code_module.functions["_MathExponent"] =                            1;
	byte_code_module.functions["_MathInteger"] =                             1;
	byte_code_module.functions["_MathLg"] =                                  1;
	byte_code_module.functions["_MathLn"] =                                  1;
	byte_code_module.functions["_MathModulus"] =                             1;
	byte_code_module.functions["_MathPower"] =                               2;
	byte_code_module.functions["_MathRandom"] =                              2;
	byte_code_module.functions["_MathSin"] =                                 1;
	byte_code_module.functions["_MathSquareRoot"] =                          1;
	byte_code_module.functions["_MathTg"] =                                  1;
	byte_code_module.functions["_NumberAdd"] =                               2;
	byte_code_module.functions["_NumberAnd"] =                               2;
	byte_code_module.functions["_NumberDiv"] =                               2;
	byte_code_module.functions["_NumberEq"] =                                2;
	byte_code_module.functions["_NumberGt"] =                                2;
	byte_code_module.functions["_NumberLs"] =                                2;
	byte_code_module.functions["_NumberMul"] =                               2;
	byte_code_module.functions["_NumberNot"] =                               1;
	byte_code_module.functions["_NumberOr"] =                                2;
	byte_code_module.functions["_NumberSub"] =                               2;
	byte_code_module.functions["_TimerGetElapsedTimeInUs"] =                 0;
	byte_code_module.procedures["_ArrayClearMemoryAfterConvertsToStrings"] = 0;
	byte_code_module.procedures["_ArrayDelete"] =                            1;
	byte_code_module.procedures["_ArrayDeleteAll"] =                         0;
	byte_code_module.procedures["_ArrayItemAppend"] =                        2;
	byte_code_module.procedures["_ArrayItemInsert"] =                        3;
	byte_code_module.procedures["_ArrayItemRemove"] =                        2;
	byte_code_module.procedures["_ArrayItemSet"] =                           3;
	byte_code_module.procedures["_Exit"] =                                   0;
	byte_code_module.procedures["_FileClose"] =                              1;
	byte_code_module.procedures["_FileCloseAll"] =                           0;
	byte_code_module.procedures["_FileDelete"] =                             1;
	byte_code_module.procedures["_FileWrite"] =                              2;
	byte_code_module.procedures["_Show"] =                                   1;
	byte_code_module.procedures["_TimerStart"] =                             0;
	#endif

	byte_code_module.variables.insert("APPLICATION_PATH");
	InbuildVariableMap::const_iterator i = inbuild_variables.begin();
	for (; i != inbuild_variables.end(); ++i) {
		byte_code_module.variables.insert(i->first);
	}
	InbuildStringConstantMap::const_iterator j = inbuild_string_constants.
		begin();
	for (; j != inbuild_string_constants.end(); ++j) {
		byte_code_module.variables.insert(j->first);
	}

	long condition_counter = -1;
	ConditionIdStack condition_id_stack;
	CodeLines::const_iterator k = code_lines.begin();
	for (; k != code_lines.end(); ++k) {
		size_t      line_number = k->first;
		std::string code_line =   k->second;
		bool new_condition = false;
		if (code_line.substr(0, 4) == "use ") {
			std::string path = code_line.substr(4);
			path = StringTrim(path);
			if (!IsString(path) || path == "\"\"") {
				ProcessError("Invalid format of path on line " +
					ConvertToString(line_number) + ".");
			}

			Library library;
			path = path.substr(1, path.size() - 2);
			#ifdef OS_LINUX
			size_t index = path.find_last_of('/');
			#elif defined(OS_WINDOWS)
			size_t index = path.find_last_of('\\');
			#endif
			if (index != std::string::npos) {
				index++;
				library.path = path.substr(0, index);
			} else {
				index = 0;
			}

			library.name = path.substr(index);
			if (library.name.empty()) {
				ProcessError("Invalid format of path on line " +
					ConvertToString(line_number) + ".");
			}
			size_t library_length = library.name.length();
			size_t suffix_begin_index = library_length - 2;
			if (library_length > 5 && library.name.substr(0, 3) == "lib" &&
				(library.name.substr(suffix_begin_index) == ".a" || library.
				name.substr(suffix_begin_index) == ".o"))
			{
				library.name = library.name.substr(3, suffix_begin_index - 3);
			}

			byte_code_module.libraries.push_back(library);
		} else if (code_line.substr(0, 10) == "procedure ") {
			size_t index = code_line.find('/');
			if (index == std::string::npos) {
				ProcessError("Separator not found on line " + ConvertToString(
					line_number) + ".");
			}

			std::string number = code_line.substr(index + 1);
			number = StringTrim(number);

			std::string identifier = code_line.substr(10, index - 10);
			identifier = StringTrim(identifier);
			if (!IsIdentifier(identifier)) {
				ProcessError("Invalid identifier \"" + identifier + "\" on "
					"line " + ConvertToString(line_number) + ".");
			}

			byte_code_module.procedures[identifier] = ConvertFromString<size_t>(
				number);
		} else if (code_line.substr(0, 9) == "function ") {
			size_t index = code_line.find('/');
			if (index == std::string::npos) {
				ProcessError("Separator not found on line " + ConvertToString(
					line_number) + ".");
			}

			std::string number = code_line.substr(index + 1);
			number = StringTrim(number);

			std::string identifier = code_line.substr(9, index - 9);
			identifier = StringTrim(identifier);
			if (!IsIdentifier(identifier)) {
				ProcessError("Invalid identifier \"" + identifier + "\" on "
					"line " + ConvertToString(line_number) + ".");
			}

			byte_code_module.functions[identifier] = ConvertFromString<size_t>(
				number);
		} else if (code_line.substr(0, 7) == "string ") {
			size_t index = code_line.find('=');
			if (index == std::string::npos) {
				ProcessError("Separator not found on line " + ConvertToString(
					line_number) + ".");
			}

			std::string string = code_line.substr(index + 1);
			string = StringTrim(string);
			if (!IsString(string)) {
				ProcessError("Invalid format of string on line " +
					ConvertToString(line_number) + ".");
			}
			byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "push_s",
				string));

			std::string identifier = code_line.substr(7, index - 7);
			identifier = StringTrim(identifier);
			if (!IsIdentifier(identifier)) {
				ProcessError("Invalid identifier \"" + identifier + "\" on "
					"line " + ConvertToString(line_number) + ".");
			}

			byte_code_module.variables.insert(identifier);
			byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "pop",
				identifier));
		} else if (code_line.substr(0, 4) == "let ") {
			size_t index = code_line.find('=');
			if (index == std::string::npos) {
				ProcessError("Separator not found on line " + ConvertToString(
					line_number) + ".");
			}

			std::string expression = code_line.substr(index + 1);
			expression = StringTrim(expression);
			StringList tokens = Tokenize(expression, line_number);
			CompileExpressionResult compile_expression_result =
				CompileExpression(tokens, byte_code_module.procedures,
					byte_code_module.functions, line_number);
			if (!(compile_expression_result.flags & EXPRESSION_IS_RESULT))
			{
				ProcessError("Using result of procedure on line " +
					ConvertToString(line_number) + ".");
			}
			byte_code_module.byte_code.insert(byte_code_module.byte_code.end(),
				compile_expression_result.byte_code.begin(),
				compile_expression_result.byte_code.end());
			if (compile_expression_result.flags & EXPRESSION_WERE_CONVERTED)
			{
				#ifdef OS_LINUX
				byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "call",
					GetSubprogramNameByAlias(
					"ArrayClearMemoryAfterConvertsToStrings")));
				#elif defined(OS_WINDOWS)
				byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "call",
					GetSubprogramNameByAlias(
					"_ArrayClearMemoryAfterConvertsToStrings")));
				#endif
			}

			std::string identifier = code_line.substr(4, index - 4);
			identifier = StringTrim(identifier);
			if (!IsIdentifier(identifier)) {
				ProcessError("Invalid identifier \"" + identifier + "\" on "
					"line " + ConvertToString(line_number) + ".");
			}

			byte_code_module.variables.insert(identifier);
			byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "pop",
				identifier));
		} else if (code_line.substr(0, 6) == "label ") {
			std::string identifier = code_line.substr(6);
			identifier = StringTrim(identifier);
			if (!IsIdentifier(identifier)) {
				ProcessError("Invalid identifier \"" + identifier + "\" on "
					"line " + ConvertToString(line_number) + ".");
			}

			byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "lbl",
				identifier));
		} else if (code_line.substr(0, 6) == "go to ") {
			std::string identifier = code_line.substr(6);
			identifier = StringTrim(identifier);
			if (!IsIdentifier(identifier)) {
				ProcessError("Invalid identifier \"" + identifier + "\" on "
					"line " + ConvertToString(line_number) + ".");
			}

			byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "jmp",
				identifier));
		} else if (code_line.substr(0, 3) == "if ") {
			size_t index = code_line.find("then");
			if (index == std::string::npos) {
				ProcessError("Separator not found on line " + ConvertToString(
					line_number) + ".");
			}

			std::string expression = code_line.substr(3, index - 3);
			expression = StringTrim(expression);
			StringList tokens = Tokenize(expression, line_number);
			CompileExpressionResult compile_expression_result =
				CompileExpression(tokens, byte_code_module.procedures,
					byte_code_module.functions, line_number);
			if (!(compile_expression_result.flags & EXPRESSION_IS_RESULT))
			{
				ProcessError("Using result of procedure on line " +
					ConvertToString(line_number) + ".");
			}
			byte_code_module.byte_code.insert(byte_code_module.byte_code.end(),
				compile_expression_result.byte_code.begin(),
				compile_expression_result.byte_code.end());
			if (compile_expression_result.flags & EXPRESSION_WERE_CONVERTED)
			{
				#ifdef OS_LINUX
				byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "call",
					GetSubprogramNameByAlias(
					"ArrayClearMemoryAfterConvertsToStrings")));
				#elif defined(OS_WINDOWS)
				byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "call",
					GetSubprogramNameByAlias(
					"_ArrayClearMemoryAfterConvertsToStrings")));
				#endif
			}

			condition_counter++;
			condition_id_stack.push(condition_counter);
			byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "je",
				"end_of_condition" + ConvertToString(condition_counter)));

			new_condition = true;
		} else {
			StringList tokens = Tokenize(code_line, line_number);
			CompileExpressionResult compile_expression_result =
				CompileExpression(tokens, byte_code_module.procedures,
					byte_code_module.functions, line_number);
			byte_code_module.byte_code.insert(byte_code_module.byte_code.end(),
				compile_expression_result.byte_code.begin(),
				compile_expression_result.byte_code.end());
			if (compile_expression_result.flags & EXPRESSION_WERE_CONVERTED)
			{
				#ifdef OS_LINUX
				byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "call",
					GetSubprogramNameByAlias(
					"ArrayClearMemoryAfterConvertsToStrings")));
				#elif defined(OS_WINDOWS)
				byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "call",
					GetSubprogramNameByAlias(
					"_ArrayClearMemoryAfterConvertsToStrings")));
				#endif
			}
			if (compile_expression_result.flags & EXPRESSION_IS_RESULT)
			{
				byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number,
					"clr_stck"));
			}
		}

		if (!new_condition) {
			while (!condition_id_stack.empty()) {
				long last_condition_id = condition_id_stack.top();
				byte_code_module.byte_code.push_back(ByteCodeMnemonic(line_number, "lbl",
					"end_of_condition" + ConvertToString(last_condition_id)));
				condition_id_stack.pop();
			}
		}
	}

	return byte_code_module;
}

std::string ConvertByteCodeToGnuAssembler(
	const ByteCodeModule& byte_code_module, const InbuildVariableMap&
	inbuild_variables, const InbuildStringConstantMap& inbuild_string_constants)
{
	std::string gnu_assembler_code =
		"\t.text\n"
		#ifdef OS_LINUX
		"\t.global main\n"
		"main:\n"
		#elif defined(OS_WINDOWS)
		"\t.global _main\n"
		"_main:\n"
		#endif
		"\tmov 8(%esp), %eax\n"
		"\tpush (%eax)\n"
		#ifdef OS_LINUX
		"\tcall ProcessApplicationPath\n"
		#elif defined(OS_WINDOWS)
		"\tcall _ProcessApplicationPath\n"
		#endif
		"\tadd $4, %esp\n"
		"\tsubl $4, %esp\n"
		"\tfstps (%esp)\n"
		"\tpop APPLICATION_PATH\n";
	InbuildStringConstantMap::const_iterator i =
		inbuild_string_constants.begin();
	for (; i != inbuild_string_constants.end(); ++i) {
		gnu_assembler_code +=
			"\tpush $CONSTANT_STRING" + ConvertToString(std::distance(
			inbuild_string_constants.begin(), i)) + "\n"
			#ifdef OS_LINUX
			"\tcall ArrayCreateFromString\n"
			#elif defined(OS_WINDOWS)
			"\tcall _ArrayCreateFromString\n"
			#endif
			"\tadd $4, %esp\n"
			"\tsubl $4, %esp\n"
			"\tfstps (%esp)\n"
			"\tpop " + i->first + "\n";
	}

	ByteCode::const_iterator j = byte_code_module.byte_code.begin();
	StringList numbers;
	StringList strings;
	for (; j != byte_code_module.byte_code.end(); ++j) {
		ByteCodeMnemonic mnemonic = *j;
		if (mnemonic.mnemonic == "push_n") {
			if (!IsNumber(mnemonic.operand)) {
				ProcessError("Invalid number \"" + mnemonic.operand + "\" on "
					"line " + ConvertToString(mnemonic.line_number) + ".");
			}

			numbers.push_back(mnemonic.operand);
			gnu_assembler_code +=
				"\tsubl $4, %esp\n"
				"\tfld CONSTANT_NUMBER" + ConvertToString(numbers.size() - 1) +
				"\n"
				"\tfstps (%esp)\n";
		} else if (mnemonic.mnemonic == "push_s") {
			if (!IsString(mnemonic.operand)) {
				ProcessError("Invalid string format on line " + ConvertToString(
					mnemonic.line_number) + ".");
			}

			strings.push_back(mnemonic.operand);
			gnu_assembler_code +=
				"\tpush $CONSTANT_STRING" + ConvertToString(
				inbuild_string_constants.size() + strings.size() - 1) + "\n"
				#ifdef OS_LINUX
				"\tcall ArrayCreateFromString\n"
				#elif defined(OS_WINDOWS)
				"\tcall _ArrayCreateFromString\n"
				#endif
				"\tadd $4, %esp\n"
				"\tsubl $4, %esp\n"
				"\tfstps (%esp)\n";
		} else if (mnemonic.mnemonic == "push_v") {
			if (!IsIdentifier(mnemonic.operand)) {
				ProcessError("Invalid identifier \"" + mnemonic.operand + "\" "
					"on line " + ConvertToString(mnemonic.line_number) + ".");
			}

			VariableSet::const_iterator iterator = byte_code_module.variables
				.find(mnemonic.operand);
			if (iterator == byte_code_module.variables.end()) {
				ProcessError("Undefined variable \"" + mnemonic.operand + "\" "
					"on line " + ConvertToString(mnemonic.line_number) + ".");
			}

			gnu_assembler_code += "\tpush " + mnemonic.operand + "\n";
		} else if (mnemonic.mnemonic == "pop") {
			if (!IsIdentifier(mnemonic.operand)) {
				ProcessError("Invalid identifier \"" + mnemonic.operand + "\" "
					"on line " + ConvertToString(mnemonic.line_number) + ".");
			}

			VariableSet::const_iterator iterator = byte_code_module.variables
				.find(mnemonic.operand);
			if (iterator == byte_code_module.variables.end()) {
				ProcessError("Undefined variable \"" + mnemonic.operand + "\" "
					"on line " + ConvertToString(mnemonic.line_number) + ".");
			}

			gnu_assembler_code += "\tpop " + mnemonic.operand + "\n";
		} else if (mnemonic.mnemonic == "lbl") {
			if (!IsIdentifier(mnemonic.operand)) {
				ProcessError("Invalid identifier \"" + mnemonic.operand + "\" "
					"on line " + ConvertToString(mnemonic.line_number) + ".");
			}

			gnu_assembler_code += mnemonic.operand + ":\n";
		} else if (mnemonic.mnemonic == "jmp") {
			if (!IsIdentifier(mnemonic.operand)) {
				ProcessError("Invalid identifier \"" + mnemonic.operand + "\" "
					"on line " + ConvertToString(mnemonic.line_number) + ".");
			}

			gnu_assembler_code += "\tjmp " + mnemonic.operand + "\n";
		} else if (mnemonic.mnemonic == "je") {
			if (!IsIdentifier(mnemonic.operand)) {
				ProcessError("Invalid identifier \"" + mnemonic.operand + "\" "
					"on line " + ConvertToString(mnemonic.line_number) + ".");
			}

			gnu_assembler_code +=
				"\tcmp $0, (%esp)\n"
				"\tpop %eax\n"
				"\tje " + mnemonic.operand + "\n";
		} else if (mnemonic.mnemonic == "call") {
			SubprogramMap::const_iterator iterator = byte_code_module
				.procedures.find(mnemonic.operand);
			bool return_result = false;
			if (iterator == byte_code_module.procedures.end()) {
				iterator = byte_code_module.functions.find(mnemonic.operand);
				if (iterator == byte_code_module.functions.end()) {
					ProcessError("Undefined subprogram \"" + mnemonic.operand +
						"\" on line " + ConvertToString(mnemonic.line_number) +
						".");
				}

				return_result = true;
			}

			gnu_assembler_code +=
				"\tcall " + mnemonic.operand + "\n";
			size_t number_of_arguments = 4 * iterator->second;
			if (number_of_arguments > 0) {
				gnu_assembler_code += "\taddl $" + ConvertToString<size_t>(
					number_of_arguments) + ", %esp\n";
			}
			if (return_result) {
				gnu_assembler_code +=
					"\tsubl $4, %esp\n"
					"\tfstps (%esp)\n";
			}
		} else if (mnemonic.mnemonic == "to_str") {
			gnu_assembler_code +=
				#ifdef OS_LINUX
				"\tcall ArrayConvertToString\n"
				#elif defined(OS_WINDOWS)
				"\tcall _ArrayConvertToString\n"
				#endif
				"\taddl $4, %esp\n"
				"\tpush %eax\n";
		} else if (mnemonic.mnemonic == "clr_stck") {
			gnu_assembler_code += "\tpop %eax\n";
		} else {
			ProcessError("Undefined mnemonic \"" + mnemonic.operand + "\" on "
				"line " + ConvertToString(mnemonic.line_number) + ".");
		}
	}

	gnu_assembler_code +=
		"\n"
		#ifdef OS_LINUX
		"\tcall ArrayDeleteAll\n"
		"\tcall FileCloseAll\n"
		#elif defined(OS_WINDOWS)
		"\tcall _ArrayDeleteAll\n"
		"\tcall _FileCloseAll\n"
		#endif
		"\tmov $0, %eax\n"
		"\tret\n"
		"\n"
		"\t.data\n";
	StringList::const_iterator numbers_begin = numbers.begin();
	StringList::const_iterator k = numbers_begin;
	for (; k != numbers.end(); ++k) {
		gnu_assembler_code +=
			"CONSTANT_NUMBER" + ConvertToString(std::distance(numbers_begin, k))
			+ ":\n"
			"\t.float " + *k + "\n";
	}

	InbuildStringConstantMap::const_iterator l =
		inbuild_string_constants.begin();
	for (; l != inbuild_string_constants.end(); ++l) {
		gnu_assembler_code +=
			"CONSTANT_STRING" + ConvertToString(std::distance(
			inbuild_string_constants.begin(), l)) + ":\n"
			"\t.string " + l->second + "\n";
	}
	StringList::const_iterator strings_begin = strings.begin();
	StringList::const_iterator m = strings_begin;
	for (; m != strings.end(); ++m) {
		gnu_assembler_code +=
			"CONSTANT_STRING" + ConvertToString(inbuild_string_constants.size()
			+ std::distance(strings_begin, m)) + ":\n"
			"\t.string " + *m + "\n";
	}

	VariableSet::const_iterator n = byte_code_module.variables.begin();
	for (; n != byte_code_module.variables.end(); ++n) {
		std::string variable_name = *n;
		if (inbuild_variables.count(variable_name) == 0) {
			gnu_assembler_code +=
				variable_name + ":\n"
				"\t.float 0\n";
		} else {
			gnu_assembler_code +=
				variable_name + ":\n"
				"\t.float " + ConvertToString(inbuild_variables.find(
				variable_name)->second) + "\n";
		}
	}

	return gnu_assembler_code;
}

void MakeExecutableFile(const std::string& gnu_assembler_code, const
	LibrariesList& libraries, bool debug_output)
{
	#ifdef OS_LINUX
	std::string filename = std::string(std::tmpnam(NULL)) + ".s";
	#elif defined(OS_WINDOWS)
	std::string filename = std::string(std::tmpnam(NULL)) + "s";
	#endif
	if (debug_output) {
		ShowMessage("Save GNU Assembler code to \"" + filename + "\".");
	}
	FileWrite(filename, gnu_assembler_code);

	if (debug_output) {
		ShowMessage("Make executable file.");
	}
	std::string command = "g++ -m32 -o program " + filename;
	LibrariesList::const_iterator i = libraries.begin();
	for (; i != libraries.end(); ++i) {
		Library library = *i;
		if (!library.path.empty()) {
			command += " -L" + library.path;
		}
		command += " -l" + library.name;
	}
	command += " -L./libs/ -lNicoleFramework";
	if (debug_output) {
		ShowMessage("Using command: \"" + command + "\".");
	}
	int result = std::system(command.c_str());
	if (result != 0) {
		ProcessError("Assembling or linking finished with error.");
	}

	std::remove(filename.c_str());
}

int main(int number_of_arguments, char** arguments) {
	bool debug_output = true;

	std::string filename = ProcessCommandLineArguments(number_of_arguments,
		arguments);
	CodeLines code_lines = FileRead(filename);
	code_lines = Preprocess(code_lines);
	if (code_lines.empty()) {
		ProcessError("Program is empty.");
	}
	if (debug_output) {
		ShowCodeLines(code_lines);
	}

	InbuildVariableMap inbuild_variables;
	inbuild_variables["FALSE"] =                  0.0f;
	inbuild_variables["TRUE"] =                   1.0f;
	inbuild_variables["PI"] =                     3.14159f;
	inbuild_variables["E"] =                      2.71828f;
	inbuild_variables["FILE_OPEN_MODE_READ"] =    0.0f;
	inbuild_variables["FILE_OPEN_MODE_APPEND"] =  1.0f;
	inbuild_variables["FILE_OPEN_MODE_REWRITE"] = 2.0f;
	inbuild_variables["OS_LINUX"] =               1.0f;
	inbuild_variables["OS_WINDOWS"] =             0.0f;
	InbuildStringConstantMap inbuild_string_constants;
	inbuild_string_constants["NEW_LINE"] = "\"\\n\"";
	#ifdef OS_LINUX
	inbuild_string_constants["PATH_SEPARATOR"] = "\"/\"";
	#elif defined(OS_WINDOWS)
	inbuild_string_constants["PATH_SEPARATOR"] = "\"\\\\\"";
	#endif
	ByteCodeModule byte_code_module = Compile(code_lines, inbuild_variables,
		inbuild_string_constants);
	if (debug_output) {
		ShowByteCode(byte_code_module);
	}

	std::string gnu_assembler_code = ConvertByteCodeToGnuAssembler(
		byte_code_module, inbuild_variables, inbuild_string_constants);
	if (debug_output) {
		ShowMessage("GNU Assembler:");
		ShowMessage(gnu_assembler_code);
	}
	MakeExecutableFile(gnu_assembler_code, byte_code_module.libraries,
		debug_output);
}
