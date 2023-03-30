#include "Textures.h"
#include "Renderer.h"
#include <stdlib.h>

#define AMOUNT_OF_TEXTURES 13
#define AMOUNT_OF_FONTS 1

static Texture **Textures;
static Font *Fonts;

void LoadTextures()
{
	Textures = calloc(sizeof(Texture), AMOUNT_OF_TEXTURES);		

	Textures[WOOD1] = LoadTexture("recs/wood1.png");
	Textures[WOOD2] = LoadTexture("recs/wood2.png");
	Textures[WOOD3] = LoadTexture("recs/wood3.png");
	Textures[WOOD4] = LoadTexture("recs/wood4.png");
	Textures[WOOD5] = LoadTexture("recs/wood5.png");
	Textures[WOOD6] = LoadTexture("recs/wood6.png");
	Textures[WOOD7] = LoadTexture("recs/wood7.png");
	Textures[WOOD8] = LoadTexture("recs/wood8.png");
	Textures[WOOD9] = LoadTexture("recs/wood9.png");
	Textures[WOOD10] = LoadTexture("recs/wood10.png");
	Textures[WOOD11] = LoadTexture("recs/wood11.png");
	Textures[WOOD12] = LoadTexture("recs/wood12.png");
	Textures[WOOD13] = LoadTexture("recs/wood13.png");
}

void LoadFonts()
{
	Fonts = calloc(sizeof(Font), AMOUNT_OF_FONTS);

	Fonts[ARIAL] = LoadFont("recs/arial.ttf", 48);
}

Texture* GetTexture(int index)
{
	return Textures[index];
}

Font GetFont(int index)
{
	return Fonts[index];
}
