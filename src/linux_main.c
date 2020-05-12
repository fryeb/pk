#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))

#include <stdio.h>

// Stub main so we can test on linux
int main(int argc, char** argv) {
	printf("Hello, world!\n");
	return 0;
}

#endif
