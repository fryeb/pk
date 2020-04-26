#include "draw.h"

void blitGlyph(Texture* tex, const Glyph* g, uint32_t color, int32_t x, int32_t y) {
	int32_t minx = CLAMP(x, 0, (int32_t)tex->width);
	int32_t miny = CLAMP(y, 0, (int32_t)tex->height);
	int32_t yoff =
	    (y < 0) ? -y : 0; // Handle the case where a character is
	                              // partially scrolled of the screen

	int32_t maxx = CLAMP(x + g->width, 0, (int32_t)tex->width);
	int32_t maxy = CLAMP(y + g->height, 0, (int32_t)tex->height);

	uint32_t fg_rb = color & 0x00FF00FF;
	uint32_t fg_g = color & 0x0000FF00;

	uint8_t* dest_row = (uint8_t*)((uint64_t)tex->memory + minx * 4 +
	                               miny * tex->pitch * sizeof(uint32_t));
	uint8_t* src = (uint8_t*)((uint64_t)g->data + g->width * yoff);

	for (int py = miny; py < maxy; py++) {
		uint32_t* dest = (uint32_t*)dest_row;

		for (int px = minx; px < maxx; px++) {
			// Background
			uint32_t bg = *dest;
			uint32_t bg_rb = bg & 0x00FF00FF;
			uint32_t bg_g = bg & 0x0000FF00;

			uint32_t a = *src;
			uint32_t na = 255 - a;

			uint32_t rb = ((a * fg_rb + na * bg_rb) >> 8) & 0x00FF00FF;
			uint32_t g = ((a * fg_g + na * bg_g) >> 8) & 0x0000FF00;

			*dest++ = rb | g;
			src++;
		}

		dest_row += tex->pitch * sizeof(uint32_t);
	}
}

// TODO: Handle tabs
void blitString(Texture* tex, const Font* font, const char* string,
                uint32_t color, int32_t x, int32_t y) {

	int32_t baseX = x;
	for (const char* c = string; *c != '\0'; c++) {
		Glyph g = findGlyph(font, *c);
		int32_t gx = x + g.offsetX;
		int32_t gy = y + g.offsetY;
		blitGlyph(tex, &g, color, gx, gy);
		x += g.dx;
		if (*c == '\n') {
			x = baseX;
			y += font->lineOffset;
		}
	}
}

// TODO: Adjust to DPI
DrawResources loadDrawResources(const DrawConfig *config) {
	DrawResources result = {0};
	result.mainFont = createFont(config->mainFontPath, config->mainFontSize);
	return result;
}

void freeDrawResources(DrawResources *drawResources) {
	destroyFont(&drawResources->mainFont);
}

void draw(Texture *tex, const DrawConfig *config,
          const DrawResources *drawResources) {

	// Clear screen
	uint32_t *pixel = tex->memory;
	for (size_t y = 0; y < tex->height; y++) {
		for (size_t x = 0; x < tex->width; x++) {
			*pixel++ = config->clearColor;
		}
	}

	blitString(tex, &drawResources->mainFont, "Hello, world!", config->textColor, 200, 200);
}

