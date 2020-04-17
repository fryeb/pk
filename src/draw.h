#pragma once
#include "stdafx.h"

typedef struct {
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint32_t *memory;
} Texture;

void draw(Texture* tex);
