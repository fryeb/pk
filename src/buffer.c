#pragma once
#include "stdafx.h"
#include "buffer.h"

#include <stdlib.h>

Buffer createBuffer() {	
	Buffer result;
	result.capacity = 8;
	result.bytes = calloc(result.capacity, sizeof(uint8_t));
	result.length = 1;
	result.cursor = 0;
	assert(result.bytes != NULL);
	return result;
}

Buffer createBufferFromFile(const char* path) {
	// Not Implemented
	assert(false);
	return createBuffer();
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
	clone.length = buffer->length;
	clone.cursor = buffer->cursor;

	return clone;
}

// TODO: Unicode
static void move(Buffer* buffer, int32_t offset) {
	size_t cursor = buffer->cursor + offset;
	assert(cursor >= 0);
	assert(cursor <= buffer->length);
	buffer->cursor = cursor;
}

// TODO: Unicode
static void insert(Buffer* buffer, int32_t codepoint) {
	move(buffer, 0); // Check cursor is in range

	if (buffer->length + 1  >= buffer->capacity) {
		size_t capacity = buffer->capacity * 2;
		// TODO: Handle failed allocation
		buffer->bytes = realloc(buffer->bytes, capacity);
		buffer->capacity = capacity;
	}

	buffer->bytes[++buffer->length] = '\0';
	for (size_t i = buffer->cursor; i < buffer->length; i++) {
		int32_t temp = buffer->bytes[i];
		buffer->bytes[i] = codepoint;
		codepoint = temp;
	}
	buffer->cursor++;
}

// TODO: Unicode
static void delete(Buffer* buffer, int32_t codepoint) {
	buffer->length--;
	buffer->cursor--;

	assert(buffer->bytes[buffer->cursor] == (char) codepoint);

	for (size_t i = buffer->cursor; i < buffer->length; i++) {
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
