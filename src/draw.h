#pragma once
#include "font.h"
#include "stdafx.h"
#include "workbench.h"

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
	uint32_t dpi;
	Font mainFont;
} DrawResources;

// TODO: Load DrawConfig from file
DrawResources loadDrawResources(const DrawConfig *config, uint32_t dpi);
void freeDrawResources(DrawResources *drawResources);
void draw(Texture *tex, const Workbench* bench, const DrawConfig *config,
          const DrawResources *drawResources);
