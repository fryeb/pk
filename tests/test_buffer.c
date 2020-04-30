#pragma once
#include "../src/buffer.h"

#include <utest/utest.h>
#include <string.h>

static Command insert(int32_t codepoint) {
	Command result = {0};
	result.kind = COMMAND_INSERT;
	result.codepoint = codepoint;
	return result;
}

// TODO: Unicode
static void insertString(Buffer* buffer, const char* str) {
	for (; *str != '\0'; str++) {
		applyCommand(buffer, insert((int32_t) *str));
	}
}

static Command move(int32_t offset) {
	Command result = {0};
	result.kind = COMMAND_MOVE;
	result.offset = offset;
	return result;
}

static Command delete(int32_t codepoint) {
	Command result = {0};
	result.kind = COMMAND_DELETE;
	result.codepoint = codepoint;
	return result;
}

UTEST(buffer, zero) {
	Buffer buffer = createBuffer();
	destroyBuffer(&buffer);
}

UTEST(buffer, hello) {
	Buffer buffer = createBuffer();
	const char* str = "Hello, world!\n";
	insertString(&buffer, str);
	ASSERT_STREQ(str, buffer.bytes);
	destroyBuffer(&buffer);
}

UTEST(buffer, clone) {
	Buffer buffer = createBuffer();
	const char* str = "Hello, world!\n";
	insertString(&buffer, str);
	Buffer clone = cloneBuffer(&buffer);
	ASSERT_STREQ(buffer.bytes, clone.bytes);
}

UTEST(buffer, move) {
	Buffer buffer = createBuffer();
	insertString(&buffer, "world");
	applyCommand(&buffer, move(-5));
	insertString(&buffer, "Hello, ");
	ASSERT_STREQ("Hello, world", buffer.bytes);
	applyCommand(&buffer, move(5));
	insertString(&buffer, "!\n");
	ASSERT_STREQ("Hello, world!\n", buffer.bytes);
}

UTEST(buffer, delete) {
	Buffer buffer = createBuffer();
	insertString(&buffer, "Hello, world!\n");
	applyCommand(&buffer, delete('\n'));
	applyCommand(&buffer, delete('!'));
	ASSERT_STREQ("Hello, world", buffer.bytes);
	applyCommand(&buffer, move(-6));
	applyCommand(&buffer, delete(','));
	ASSERT_STREQ("Hello world", buffer.bytes);
}

UTEST(buffer, undo_insert) {
	Buffer buffer = createBuffer();
	insertString(&buffer, "Hello, world!\n");
	ASSERT_STREQ("Hello, world!\n", buffer.bytes);
	reverseCommand(&buffer, insert('\n'));
	reverseCommand(&buffer, insert('!'));
	ASSERT_STREQ("Hello, world", buffer.bytes);
}

UTEST(buffer, undo_move_delete) {
	Buffer buffer = createBuffer();
	insertString(&buffer, "Hello, world!\n");
	ASSERT_STREQ("Hello, world!\n", buffer.bytes);
	applyCommand(&buffer, move(-8));
	applyCommand(&buffer, delete(','));
	applyCommand(&buffer, move(8));
	ASSERT_STREQ("Hello world!\n", buffer.bytes);
	reverseCommand(&buffer, move(8));
	reverseCommand(&buffer, delete(','));
	reverseCommand(&buffer, move(-8));
	ASSERT_STREQ("Hello, world!\n", buffer.bytes);
}

UTEST_MAIN();

