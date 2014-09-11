# общие настройки
TARGET = wbf
TEMPLATE = lib
CONFIG += staticlib
CONFIG += warn_on
CONFIG -= qt

# файлы проекта
HEADERS += \
	source/thewizardplusplus/wizard_basic/framework/os.h \
	source/thewizardplusplus/wizard_basic/framework/array.h \
	source/thewizardplusplus/wizard_basic/framework/math.h \
	source/thewizardplusplus/wizard_basic/framework/system.h \
	source/thewizardplusplus/wizard_basic/framework/utils.h \
	source/thewizardplusplus/wizard_basic/framework/input_output.h
SOURCES += \
	source/thewizardplusplus/wizard_basic/framework/array.cpp \
	source/thewizardplusplus/wizard_basic/framework/math.cpp \
	source/thewizardplusplus/wizard_basic/framework/system.cpp \
	source/thewizardplusplus/wizard_basic/framework/utils.cpp \
	source/thewizardplusplus/wizard_basic/framework/input_output.cpp

# флаги компилятора
QMAKE_CXXFLAGS += -std=c++03 -Wpedantic -Wall -Wextra -O2 -m32
