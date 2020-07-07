#include "../src/editor.h"

#include <utest/utest.h>
#include <string.h>

UTEST(editor, zero) {
	Editor editor = {
		.buffer = createBuffer()
	};

	destroyEditor(&editor);
}

UTEST_MAIN();
