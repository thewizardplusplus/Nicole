#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>

namespace thewizardplusplus {
namespace nicole_framework {

template<typename Type>
std::string ConvertToString(Type value) {
	std::ostringstream out;
	out << value;

	std::string result = out.str();
	return result;
}

extern "C" float ProcessApplicationPath(const char* path);
extern "C" void ProcessError(const std::string& description);

}
}
#endif
