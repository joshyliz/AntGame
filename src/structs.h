#pragma once
#include "Textures.h"

typedef enum
{
	EMPTY_TILE,
	BASIC_TILE
} TILE_TYPE;

typedef struct
{
	SDL_Rect rect;
	TILE_TYPE type;//Add Texture Type Later 
	TEXTURE_INDEX texture;
} Tile;

typedef struct
{
	int width;
	int height;
	SDL_FPoint pos;
	Tile **tiles;
} Screen;

typedef struct 
{
	SDL_FPoint pos;
	SDL_FPoint vel;
	SDL_FPoint acc;
	SDL_Rect rect;
	bool inAir;
	float health;
} Player;







