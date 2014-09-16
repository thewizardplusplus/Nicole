CXXFLAGS += -std=c++03 -Wpedantic -Wall -Wextra -O2 -m32 -I../../../build/framework/headers/
LDFLAGS += -L../../../build/framework/libraries/ -L/usr/lib/x86_64-linux-gnu
LDLIBS += -lwbf -lcurl

main: file_get_contents.o
	$(CXX) -O2 -m32 $(LDFLAGS) file_get_contents.o $(LDLIBS)
