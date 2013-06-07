TARGET = nicole_compiler
CONFIG += console
CONFIG += warn_on
CONFIG -= qt
SOURCES += \
	source/thewizardplusplus/nicole/compiler/main.cpp
OTHER_FILES += \
	docs/grammar.txt \
	docs/examples/pi.ni \
	docs/examples/mandelbrot_set.ni \
	docs/examples/maze.ni \
	docs/syntax_highlighting/nicole_kate.xml \
	docs/syntax_highlighting/nicole_notepadpp.xml \
	docs/examples/test.ni
QMAKE_CXXFLAGS += -std=c++98 -pedantic -Wall -W -O3
win32 {
	QMAKE_CXXFLAGS += -U__STRICT_ANSI__
}
