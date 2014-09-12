#ifndef SYSTEM_H
#define SYSTEM_H

namespace thewizardplusplus {
namespace wizard_basic {
namespace framework {
namespace system {

extern "C" {

/* Значения констант перечисления установлены таким образом, что константа LINUX
 * равняется встроенной константе TRUE, а не FALSE, но при этом стоит на первом
 * месте в перечислении, и больше по значению, чем константа WINDOWS. )
 */
enum Os {
	PLATFORM_LINUX =   1,
	PLATFORM_WINDOWS = 0
};

float GetOs(void);
void TimerStart(void);
float TimerGetElapsedTimeInUs(void);
void Exit(void);

}

}
}
}
}
#endif
