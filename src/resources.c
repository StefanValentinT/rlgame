#ifndef RESOURCES_H
#define RESOURCES_H

#include "rl/raylib.h"
#include <stdint.h>

#define TEXTURE_SIZE 16
#define TEXTURE_SIZE_SQUARED TEXTURE_SIZE *TEXTURE_SIZE

#define R 255, 50, 50, 255,
#define G 0, 228, 48, 255,
#define L 20, 200, 60, 255,

typedef enum : uint8_t
{
	P_RED,
	P_GREEN,
	P_LIGHTGREEN,
} Palette;

Color palette_to_color(Palette p);

Texture2D load_texture();

#endif
#if __INCLUDE_LEVEL__ == 0

#include "rl/raylib.h"

// clang-format off
const unsigned char TEST_TEX_DATA[TEXTURE_SIZE_SQUARED * 4] = {
    R G G G G G G G G G G G G G G R 
    G G G G G G G G G G G G G G G G 
    G G R R R R R R R R R R R R G G 
    G G R R R R R R R R R R R R G G 
    G G R R R R R R R R R R R R G G 
    G G G G G G R R R R G G G G G G 
    G G G G G G R R R R G G G G G G 
    G G G G G G R R R R G G G G G G 
    G G G G G G R R R R G G G G G G 
    G G G G G G R R R R G G G G G G 
    G G G G G G R R R R G G G G G G 
    G G G G G G R R R R G G G G G G 
    G G G G G G R R R R G G G G G G 
    R R R G G G R R R R G G G R R R 
    G G R G G G G G G G G G G R G G  
    R G R G G G G G G G G G G R G R
};
// clang-format on

Color palette_to_color(Palette p)
{
	switch (p)
	{
	case P_RED:
		return (Color){R};
		break;
	case P_GREEN:
		return (Color){G};
		break;
	case P_LIGHTGREEN:
		return (Color){L};
		break;
	}
}

Texture2D load_texture()
{
	unsigned char flipped_data[TEXTURE_SIZE_SQUARED * 4];

	for (int y = 0; y < TEXTURE_SIZE; y++)
	{
		for (int x = 0; x < TEXTURE_SIZE; x++)
		{
			int src_x = x;
			int dst_x = TEXTURE_SIZE - 1 - x;

			int src_idx = (y * TEXTURE_SIZE + src_x) * 4;
			int dst_idx = (y * TEXTURE_SIZE + dst_x) * 4;

			flipped_data[dst_idx + 0] = TEST_TEX_DATA[src_idx + 0]; // R
			flipped_data[dst_idx + 1] = TEST_TEX_DATA[src_idx + 1]; // G
			flipped_data[dst_idx + 2] = TEST_TEX_DATA[src_idx + 2]; // B
			flipped_data[dst_idx + 3] = TEST_TEX_DATA[src_idx + 3]; // A
		}
	}

	Image img = {
	    .data = flipped_data,
	    .width = TEXTURE_SIZE,
	    .height = TEXTURE_SIZE,
	    .mipmaps = 1,
	    .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
	};

	return LoadTextureFromImage(img);
}
#endif
