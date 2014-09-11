# общие настройки
TARGET = wbc
CONFIG += console
CONFIG += warn_on
CONFIG -= qt

# файлы проекта
SOURCES += source/main.cpp

# флаги компилятора
QMAKE_CXXFLAGS += -std=c++03 -Wpedantic -Wall -Wextra -O2
