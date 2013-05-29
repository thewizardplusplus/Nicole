TARGET = NicoleFramework
TEMPLATE = lib
CONFIG += staticlib
CONFIG += warn_on
CONFIG -= qt
HEADERS += \
	source/thewizardplusplus/nicole_framework/os.h \
	source/thewizardplusplus/nicole_framework/array.h \
	source/thewizardplusplus/nicole_framework/math.h \
	source/thewizardplusplus/nicole_framework/system.h \
	source/thewizardplusplus/nicole_framework/utils.h \
	source/thewizardplusplus/nicole_framework/input_output.h
SOURCES += \
	source/thewizardplusplus/nicole_framework/array.cpp \
	source/thewizardplusplus/nicole_framework/math.cpp \
	source/thewizardplusplus/nicole_framework/system.cpp \
	source/thewizardplusplus/nicole_framework/utils.cpp \
	source/thewizardplusplus/nicole_framework/input_output.cpp
QMAKE_CXXFLAGS += -std=c++98 -pedantic -Wall -W -O3 -m32
win32 {
	QMAKE_CXXFLAGS += -U__STRICT_ANSI__
}
