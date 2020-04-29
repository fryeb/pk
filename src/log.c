#include "stdafx.h"
#include <stdarg.h>
#include <stdlib.h>

#if _WIN32
#include <stb/stb_sprintf.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void logFatal(const char* message, ...) {
	char buffer[512];
	va_list args;
	va_start(args, message);
	stbsp_vsnprintf(buffer, sizeof(buffer), message, args);
	va_end(args);

	MessageBoxA(NULL, buffer, "Fatal Error", MB_OK | MB_ICONERROR);
	exit(EXIT_FAILURE);
}

#else /* _WIN32 */
#include <stdio.h>

static void logMessage(FILE* stream, const char* color, const char* name,
                const char* message, va_list args) {
        fprintf(stream, "%s%s: \x1b[97m", color, name);
        vfprintf(stream, message, args);
        fprintf(stream, "\x1b[0m\n");
}

void logFatal(const char* message, ...) {
        va_list args;
        va_start(args, message);
        logMessage(stderr, "\x1b[91m", "fatal error", message, args);
        va_end(args);
        exit(EXIT_FAILURE);
}
#endif /* _WIN32 */
