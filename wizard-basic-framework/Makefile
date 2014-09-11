SOURCE = source/thewizardplusplus/wizard_basic/framework/
OBJECTS = $(SOURCE)array.o $(SOURCE)input_output.o $(SOURCE)math.o $(SOURCE)system.o $(SOURCE)utils.o
BUILD = ../build/framework/
BUILD_HEADERS = $(BUILD)headers/
BUILD_LIBRARIES = $(BUILD)libraries/
CXXFLAGS += -std=c++03 -Wpedantic -Wall -Wextra -O2 -m32

main: $(OBJECTS)
	mkdir -p $(BUILD_LIBRARIES)
	$(AR) crs $(BUILD_LIBRARIES)libwbf.a $(OBJECTS)

	mkdir -p $(BUILD_HEADERS)
	find $(BUILD_HEADERS) -name "*.h" -type f -delete
	cp $(SOURCE)*.h $(BUILD_HEADERS)

clean:
	find . -name "*.o" -type f -delete
