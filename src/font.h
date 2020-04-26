#pragma once
#include "stdafx.h"

typedef struct {
	uint8_t* data;
	uint32_t width;
	uint32_t height;
	int32_t offsetX;
	int32_t offsetY;
	int32_t dx;
} Glyph;

typedef struct {
	uint32_t* codepoints;
	Glyph* glyphs;
	size_t glyphCount;

	// TODO: This seems redundant
	float heightInPixels;
	float scale;
	int32_t ascent;
	int32_t descent;
	int32_t lineGap;
	int32_t lineOffset;
} Font;

Font createFont(const char* path, int32_t size);
Glyph findGlyph(const Font* font, int32_t codepoint);
void destroyFont(Font* font);
