#include <array/array.h>
#include <ctime>

using namespace thewizardplusplus::wizard_basic::framework::array;

static const size_t BUFFER_SIZE = 1024;

extern "C" float date(const char* format) {
	time_t time;
	std::time(&time);
	std::tm* time_info = localtime(&time);

	char buffer[BUFFER_SIZE];
	size_t buffer_size = std::strftime(buffer, BUFFER_SIZE, format, time_info);

	std::string content(buffer, buffer_size);
	return ArrayCreateFromString(content.c_str());
}
