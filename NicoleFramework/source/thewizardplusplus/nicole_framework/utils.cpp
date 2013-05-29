#include "utils.h"
#include "array.h"
#include "os.h"
#include <iostream>
#include <cstdlib>

using namespace thewizardplusplus::nicole_framework;

extern "C" float ProcessApplicationPath(const char* path) {
	std::string application_path = path;
	#ifdef OS_LINUX
	size_t index = application_path.find_last_of('/');
	#elif defined(OS_WINDOWS)
	size_t index = application_path.find_last_of('\\');
	#endif
	if (index != std::string::npos) {
		application_path = application_path.substr(0, index + 1);
		char first_symbol = application_path[0];
		#ifdef OS_LINUX
		if (first_symbol != '/' && first_symbol != '.') {
			application_path = "./" + application_path;
		}
		#elif defined(OS_WINDOWS)
		if (first_symbol != '\\' && first_symbol != '.') {
			application_path = ".\\" + application_path;
		}
		#endif
	} else {
		#ifdef OS_LINUX
		application_path = "./";
		#elif defined(OS_WINDOWS)
		application_path = ".\\";
		#endif
	}

	float string_id = ArrayCreateFromString(application_path.c_str());
	return string_id;
}

extern "C" void ProcessError(const std::string& description) {
	std::cerr << description << std::endl;
	std::exit(EXIT_FAILURE);
}
