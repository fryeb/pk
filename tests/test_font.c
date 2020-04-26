#include <utest/utest.h>
#include "../src/font.h"

UTEST(font, example) {
	ASSERT_EQ(2+2, 4);
}

UTEST(font, create) {
	// TODO: Make this platform agnostic
	printf("testing font.");
	Font font = createFont("C:\\Windows\\Fonts\\consola.ttf", 14);
	printf("tested font.");
	ASSERT_NE(font.codepoints, NULL);
}

UTEST_MAIN();
