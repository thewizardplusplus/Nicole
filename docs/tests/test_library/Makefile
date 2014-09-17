OBJECTS = test.o
CXXFLAGS += -std=c++03 -Wpedantic -Wall -Wextra -O2 -m32 -I../../../build/framework/headers/

main: $(OBJECTS)
	$(AR) crs libtest.a $(OBJECTS)

clean:
	find . -name "*.o" -type f -delete
