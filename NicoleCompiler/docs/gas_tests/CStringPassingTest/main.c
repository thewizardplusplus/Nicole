#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const char* ReturningFunction(void) {
	const char* test_string = "Test string.";
	char* result = (char*)malloc(strlen(test_string) + 1);
	strcpy(result, test_string);
	return result;
}

void GettingFunction(const char* string) {
	puts(string);
}

void TestFunction() {
	GettingFunction(ReturningFunction());
}

/*int main(void) {
	GettingFunction(ReturningFunction());
	return EXIT_SUCCESS;
}*/
