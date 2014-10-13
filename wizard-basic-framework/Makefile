SOURCE = source/thewizardplusplus/wizard_basic/framework/
OBJECTS = $(SOURCE)array/array.o $(SOURCE)io/io.o $(SOURCE)maths/maths.o $(SOURCE)system/system.o $(SOURCE)utils/utils.o
BUILD = ../build/framework/
BUILD_HEADERS = $(BUILD)headers/wizard_basic_framework/
BUILD_LIBRARIES = $(BUILD)libraries/wizard_basic_framework/
CXXFLAGS += -std=c++03 -Wpedantic -Wall -Wextra -O2 -m32

main: $(OBJECTS)
	mkdir -p $(BUILD_LIBRARIES)
	$(AR) crs $(BUILD_LIBRARIES)libwbf.a $(OBJECTS)

	rm -rf $(BUILD_HEADERS)
	mkdir -p $(BUILD_HEADERS)
	rsync -r --include="*/" --include="*.h" --exclude="*" $(SOURCE) $(BUILD_HEADERS)

clean:
	find . -name "*.o" -type f -delete
