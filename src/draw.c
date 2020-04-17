#include "draw.h"

void draw(Texture* tex) {
    uint32_t *pixel = tex->memory;
    for (size_t y = 0; y < tex->height; y++) {
	for (size_t x = 0; x < tex->width; x++) {
	    *pixel++ = y << 16 | x;
	}
    }
}

