# общие настройки
TARGET = wbc
CONFIG += console
CONFIG += warn_on
CONFIG -= qt
DEFINES += DEBUG_OUTPUT

# файлы проекта
SOURCES += source/main.cpp

# флаги компилятора
QMAKE_CXXFLAGS += -std=c++03 -pedantic -Wall -W -O2
