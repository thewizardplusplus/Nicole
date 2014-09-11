BUILD_BINARIES = ../build/binaries/
OBJECTS = source/main.o
CXXFLAGS += -std=c++03 -Wpedantic -Wall -Wextra -O2

main: $(OBJECTS)
	mkdir -p $(BUILD_BINARIES)
	$(CXX) $(OBJECTS) -o $(BUILD_BINARIES)wbc

clean:
	find . -name "*.o" -type f -delete
