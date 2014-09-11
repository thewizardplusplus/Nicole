#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>

namespace thewizardplusplus {
namespace wizard_basic {
namespace framework {

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
}
#endif
