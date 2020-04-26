#include <utest/utest.h>
#include "../src/font.h"

UTEST(font, example) {
	ASSERT_EQ(2+2, 4);
}

UTEST(font, create) {
	Font font = createFont("../res/Inconsolata-VF.ttf", 14);
	ASSERT_NE(font.codepoints, NULL);
}

UTEST_MAIN();
