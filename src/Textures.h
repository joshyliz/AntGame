#pragma once
#include "Renderer.h"

typedef enum
{
	WOOD1,
	WOOD2,
	WOOD3,
	WOOD4,
	WOOD5,
	WOOD6,
	WOOD7,
	WOOD8,
	WOOD9,
	WOOD10,
	WOOD11,
	WOOD12,
	WOOD13,
} TEXTURE_INDEX;

typedef enum
{
	ARIAL,
} Font_Index;

void LoadTextures();

void LoadFonts();

Texture* GetTexture(int index);

Font GetFont(int index);
