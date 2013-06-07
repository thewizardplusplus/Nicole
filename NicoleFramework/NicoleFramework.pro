TARGET = NicoleFramework
TEMPLATE = lib
CONFIG += staticlib
CONFIG += warn_on
CONFIG -= qt
HEADERS += \
	source/thewizardplusplus/nicole/framework/os.h \
	source/thewizardplusplus/nicole/framework/array.h \
	source/thewizardplusplus/nicole/framework/math.h \
	source/thewizardplusplus/nicole/framework/system.h \
	source/thewizardplusplus/nicole/framework/utils.h \
	source/thewizardplusplus/nicole/framework/input_output.h
SOURCES += \
	source/thewizardplusplus/nicole/framework/array.cpp \
	source/thewizardplusplus/nicole/framework/math.cpp \
	source/thewizardplusplus/nicole/framework/system.cpp \
	source/thewizardplusplus/nicole/framework/utils.cpp \
	source/thewizardplusplus/nicole/framework/input_output.cpp
QMAKE_CXXFLAGS += -std=c++98 -pedantic -Wall -W -O3 -m32
win32 {
	QMAKE_CXXFLAGS += -U__STRICT_ANSI__
}

OTHER_FILES += \
    docs/export_functions.sh
