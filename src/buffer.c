#pragma once
#include "stdafx.h"
#include "buffer.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>

Buffer createBuffer() {	
	Buffer result;
	result.capacity = 8;
	result.bytes = calloc(result.capacity, sizeof(uint8_t));
	result.size = 1;
	result.cursor = 0;
	assert(result.bytes != NULL);
	return result;
}

// TODO: This could probably be constant time with bitwise operations
static size_t nextPow2(size_t x) {
	size_t y = 8;
	while (y < x) y *= 2;
	return y;
}

// TODO: mmap or MapViewOfFile would probably be faster
// TODO: Unicode
Buffer createBufferFromFile(const char* path) {
	Buffer result;
	FILE* file = fopen(path, "r");
	if (!file) {
		logFatal("Unable to open file: \"%s\".\n", path);
		return (Buffer) {0};
	}

	fseek(file, 0, SEEK_END);
	result.size = ftell(file);
	result.capacity = nextPow2(result.size);
	result.cursor = 0;
	result.bytes = calloc(1, result.capacity);

	assert(result.bytes);
	fseek(file, 0, SEEK_SET);
	size_t elementsRead = fread(result.bytes, 1, result.size, file);
	//logFatal("Read %d elements, expected %d.\n", elementsRead, result.size);
	//assert(elementsRead == result.size);
	fclose(file);

	return result;
}

void destroyBuffer(Buffer* buffer) {
	realloc(buffer->bytes, 0);
	*buffer = (Buffer){0};
}

Buffer cloneBuffer(const Buffer* buffer) {
	Buffer clone = {0};
	clone.bytes = calloc(buffer->capacity, 1);
	memcpy(clone.bytes, buffer->bytes, buffer->capacity);
	clone.capacity = buffer->capacity;
	clone.size = buffer->size;
	clone.cursor = buffer->cursor;

	return clone;
}

// TODO: Unicode
static void move(Buffer* buffer, int32_t offset) {
	size_t cursor = buffer->cursor + offset;
	assert(cursor >= 0);
	assert(cursor <= buffer->size);
	buffer->cursor = cursor;
}

// TODO: Unicode
static void insert(Buffer* buffer, int32_t codepoint) {
	move(buffer, 0); // Check cursor is in range

	if (buffer->size + 1  >= buffer->capacity) {
		size_t capacity = buffer->capacity * 2;
		// TODO: Handle failed allocation
		buffer->bytes = realloc(buffer->bytes, capacity);
		buffer->capacity = capacity;
	}

	buffer->bytes[++buffer->size] = '\0';
	for (size_t i = buffer->cursor; i < buffer->size; i++) {
		int32_t temp = buffer->bytes[i];
		buffer->bytes[i] = codepoint;
		codepoint = temp;
	}
	buffer->cursor++;
}

// TODO: Unicode
static void delete(Buffer* buffer, int32_t codepoint) {
	buffer->size--;
	buffer->cursor--;

	assert(buffer->bytes[buffer->cursor] == (char) codepoint);

	for (size_t i = buffer->cursor; i < buffer->size; i++) {
		buffer->bytes[i] = buffer->bytes[i+1];
	}
}

void applyCommand(Buffer* buffer, Command command) {
	switch(command.kind) {
	case COMMAND_INSERT: {
		insert(buffer, command.codepoint);
	} break;
	case COMMAND_MOVE: {
		move(buffer, command.offset);
	} break;
	case COMMAND_DELETE: {
		delete(buffer, command.codepoint);
	} break;
	default: assert(false);
	}
}

void reverseCommand(Buffer* buffer, Command command) {
	switch(command.kind) {
	case COMMAND_INSERT: {
		delete(buffer, command.codepoint);
	} break;
	case COMMAND_MOVE: {
		move(buffer, -command.offset);
	} break;
	case COMMAND_DELETE: {
		insert(buffer, command.codepoint);
	} break;
	default: assert(false);
	}
}
