#pragma once
#include "font.h"
#include "stdafx.h"

typedef struct {
	uint32_t width;
	uint32_t height;
	uint32_t *memory;
	uint32_t pitch;
} Texture;

typedef struct {
	uint32_t clearColor;
	uint32_t textColor;
	const char *mainFontPath;
	int32_t mainFontSize;
} DrawConfig;

typedef struct {
	Font mainFont;
} DrawResources;

// TODO: Load DrawConfig from file
// TODO: Adapt to dpi
DrawResources loadDrawResources(const DrawConfig *config);
void freeDrawResources(DrawResources *drawResources);
void draw(Texture *tex, const DrawConfig *config,
          const DrawResources *drawResources);
