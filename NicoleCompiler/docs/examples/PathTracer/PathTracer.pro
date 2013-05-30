SOURCES += \
	source/thewizardplusplus/path_tracer/main.cpp
QMAKE_CXXFLAGS += -std=c++98 -pedantic -Wall -W -O3
win32 {
	QMAKE_CXXFLAGS += -U__STRICT_ANSI__
}
