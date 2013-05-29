#include "system.h"
#include "os.h"
#ifdef OS_LINUX
#include <sys/time.h>
#elif defined(OS_WINDOWS)
#include <windows.h>
#endif
#include <cstdlib>

#ifdef OS_LINUX
static timeval       start_time = {0};
#elif defined(OS_WINDOWS)
static LARGE_INTEGER frequency =  {0};
static LARGE_INTEGER start_time = {0};
#endif

extern "C" void TimerStart(void) {
	#ifdef OS_LINUX
	gettimeofday(&start_time, NULL);
	#elif defined(OS_WINDOWS)
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start_time);
	#endif
}

extern "C" float TimerGetElapsedTime(void) {
	#ifdef OS_LINUX
	timeval current_time;
	gettimeofday(&current_time, NULL);
	return 1000.0f * (current_time.tv_sec - start_time.tv_sec) + (current_time
		.tv_usec - start_time.tv_usec) / 1000.0f;
	#elif defined(OS_WINDOWS)
	LARGE_INTEGER current_time;
	QueryPerformanceCounter(&current_time);
	return 1000.0f * (current_time.QuadPart - start_time.QuadPart) /
		frequency.QuadPart;
	#endif
}

extern "C" void Exit(void) {
	std::exit(EXIT_SUCCESS);
}
