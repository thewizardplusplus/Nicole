#include "system.h"
#include "../utils/os.h"
#ifdef OS_LINUX
	#include <sys/time.h>
#elif defined(OS_WINDOWS)
	#include <windows.h>
#endif
#include <cstdlib>

using namespace thewizardplusplus::wizard_basic::framework::system;

#ifdef OS_LINUX
	static timeval start_time = {0, 0};
#elif defined(OS_WINDOWS)
	static LARGE_INTEGER frequency = {0UL, 0L};
	static LARGE_INTEGER start_time = {0UL, 0L};
#endif

extern "C" float GetOs(void) {
	#ifdef OS_LINUX
		return PLATFORM_LINUX;
	#elif defined(OS_WINDOWS)
		return PLATFORM_WINDOWS;
	#endif
}

extern "C" void TimerStart(void) {
	#ifdef OS_LINUX
		gettimeofday(&start_time, NULL);
	#elif defined(OS_WINDOWS)
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start_time);
	#endif
}

extern "C" float TimerGetElapsedTimeInUs(void) {
	#ifdef OS_LINUX
		timeval current_time;
		gettimeofday(&current_time, NULL);

		return
			1000000.0f * (current_time.tv_sec - start_time.tv_sec)
			+ (current_time.tv_usec - start_time.tv_usec);
	#elif defined(OS_WINDOWS)
		LARGE_INTEGER current_time;
		QueryPerformanceCounter(&current_time);

		return
			1000000.0f
			* (current_time.QuadPart - start_time.QuadPart)
			/ frequency.QuadPart;
	#endif
}

extern "C" void Exit(void) {
	std::exit(EXIT_SUCCESS);
}
