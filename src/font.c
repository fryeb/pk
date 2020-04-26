#include "stdafx.h"
#include "font.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

#include <stdio.h>
#include <stdlib.h>

static void* loadFile(const char* path, size_t* pSize) {
	// TODO: Handle missing file
	FILE* file = fopen(path, "r");
	if (!file) {
		fprintf(stderr, "error: Unable to open file: \"%s\".\n", path);
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	*pSize = ftell(file);
	void* data = calloc(1, *pSize);
	assert(data);
	fseek(file, 0, SEEK_SET);
	size_t elementsRead = fread(data, 1, *pSize, file);
	fclose(file);
	return data;
}

Font createFont(const char* path, int32_t height) {
	size_t fontFileSize = 0;
	uint8_t* fontFileData = loadFile(path, &fontFileSize);

	// Setup font
	int32_t offset = stbtt_GetFontOffsetForIndex(fontFileData, 0);
	assert(offset >= 0);
	stbtt_fontinfo info = {0};
	stbtt_InitFont(&info, fontFileData, offset);	

	Font font = {0};
	font.heightInPixels = (float)height;
	font.scale = stbtt_ScaleForPixelHeight(&info, font.heightInPixels);
	stbtt_GetFontVMetrics(&info, &font.ascent, &font.descent, &font.lineGap);
	font.lineOffset = (int32_t)((font.ascent - font.descent + font.lineGap) * font.scale);

	// TODO: stbtt_fontinfo is supposed to be opaque, can we do this another way?
	// TODO: Use a simple allocator here, rather than allocating individual glyphs
	int32_t numGlyphs = info.numGlyphs;
	font.codepoints = calloc(numGlyphs, sizeof(*font.codepoints));
	font.glyphs = calloc(numGlyphs, sizeof(*font.glyphs));
	assert(numGlyphs > 0);
	int32_t codepoint = -1;
	for (int32_t i = 0; i < numGlyphs; i++) {
		// TODO: Support characters outside the basic multilingual plane (BMP)
		int32_t glyphIndex = 0;
		while (glyphIndex == 0 && codepoint <= UINT16_MAX) {
			assert(codepoint < INT32_MAX);
			codepoint++;
			glyphIndex = stbtt_FindGlyphIndex(&info, codepoint);
		}
		if (glyphIndex <= 0) break;

		font.codepoints[i] = codepoint;
		Glyph glyph = {0};
		glyph.data = stbtt_GetGlyphBitmap(
			&info, 0, font.scale, glyphIndex,
			&glyph.width, &glyph.height, &glyph.offsetX, &glyph.offsetY);
		int32_t dx = 0;
		int32_t leftSideBearing = 0;
		stbtt_GetGlyphHMetrics(&info, glyphIndex, &dx, &leftSideBearing);
	}

	free(fontFileData);
	return font;
}

// TODO: Use something faster than linear search such as:
//	- Binary search
//	- Hash table
Glyph findGlyph(const Font* font, int32_t codepoint) {
	for (int32_t i = 0; i < font->glyphCount; i++) {
		if (font->codepoints[i] == codepoint)
			return font->glyphs[i];
	}

	return (Glyph){0};
}

void destroyFont(Font* font) {
	free(font->codepoints);
	for (size_t i = 0; i < font->glyphCount; i++) {
		//stbtt_FreeBitmap(font->glyphs[i].data, NULL);
	}
	free(font->glyphs);
	*font = (Font){0};
}


