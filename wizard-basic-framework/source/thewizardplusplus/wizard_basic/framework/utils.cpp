#include "utils.h"
#include "os.h"
#include "array.h"
#include <iostream>
#include <cstdlib>

using namespace thewizardplusplus::wizard_basic::framework;

extern "C" float ProcessApplicationPath(const char* path) {
	std::string application_path = path;
	#ifdef OS_LINUX
		size_t last_separator_index = application_path.find_last_of('/');
	#elif defined(OS_WINDOWS)
		size_t last_separator_index = application_path.find_last_of('\\');
	#endif
	if (last_separator_index != std::string::npos) {
		application_path = application_path.substr(0, last_separator_index + 1);
	} else {
		#ifdef OS_LINUX
			application_path = "./";
		#elif defined(OS_WINDOWS)
			application_path = ".\\";
		#endif
	}

	return ArrayCreateFromString(application_path.c_str());
}

extern "C" void ProcessError(const std::string& description) {
	std::cerr << description << std::endl;
	std::exit(EXIT_FAILURE);
}
