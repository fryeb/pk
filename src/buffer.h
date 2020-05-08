#pragma once
#include "stdafx.h"

// TODO: Implement a compressed form to support undo/redo
typedef enum {
	COMMAND_INSERT,
	COMMAND_DELETE,
	COMMAND_MOVE,
} CommandKind;

typedef struct {
	CommandKind kind;
	union {
		// COMMAND_INSERT or COMMAND_DELETE
		int32_t codepoint;
		// COMMAND_MOVE
		int32_t offset;
	};
} Command;

typedef struct {
	uint8_t* bytes;
	size_t size;
	size_t capacity;
	size_t cursor;
} Buffer;

Buffer createBuffer();
Buffer createBufferFromFile(const char* path);
Buffer cloneBuffer(const Buffer* buffer);
void destroyBuffer(Buffer* buffer);

void applyCommand(Buffer* buffer, Command command);
void reverseCommand(Buffer* buffer, Command command);
