#include <utest/utest.h>
#include "../src/font.h"

struct FontTestFixture {
	Font font;
};

UTEST_F_SETUP(FontTestFixture) {
	utest_fixture->font = createFont("../res/Inconsolata-VF.ttf", 20);
	ASSERT_NE(utest_fixture->font.codepoints, NULL);
}

UTEST_F_TEARDOWN(FontTestFixture) {
	destroyFont(&utest_fixture->font);
	ASSERT_EQ(utest_fixture->font.codepoints, NULL);
}

UTEST_F(FontTestFixture, search) {
	ASSERT_NE(findGlyph(&utest_fixture->font, 'a').data, 0);
	ASSERT_NE(findGlyph(&utest_fixture->font, 'b').data, 0);
	ASSERT_NE(findGlyph(&utest_fixture->font, 'c').data, 0);
	ASSERT_NE(findGlyph(&utest_fixture->font, 'd').data, 0);
	// Pound symbol
	ASSERT_NE(findGlyph(&utest_fixture->font, 0x000000A3).data, 0);
}


UTEST_MAIN();
