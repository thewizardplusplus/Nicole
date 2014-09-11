OBJECTS = source/main.o
CXXFLAGS += -std=c++03 -Wpedantic -Wall -Wextra -O2

main: $(OBJECTS)
	$(CXX) $(OBJECTS) -o wbc

clean:
	find . -name "*.o" -type f -delete
