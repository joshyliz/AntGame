#include "GameManager.h"
#include "Renderer.h"
#include "Textures.h"
#include <stdio.h>
#include <stdlib.h>
#include "ReadMap.h"
#include "structs.h"
#include "Npc.h"
#include "Collison.h"
#include "Common.h"
#include "Bullet.h"

Screen *screens;

Texture *woods;

Player *player;


void StartGameManager(Player *playerP)
{
	player = playerP;

	screens = calloc(sizeof(Screen), NUMBER_OF_SCREENS);

	int width = WINDOW_WIDTH / TILE_WIDTH;
	int height = WINDOW_HEIGHT / TILE_HEIGHT;

	int **map = calloc(width, sizeof(int*));

	for(int i = 0; i < width; i++)
		map[i] = calloc(height, sizeof(int));

	GetMapFromFile("Level.mp", map);

	for(int s = 0; s < NUMBER_OF_SCREENS; s++)
	{
		screens[s].pos = (SDL_FPoint){0, -WINDOW_HEIGHT * s};
		screens[s].width = WINDOW_WIDTH / TILE_WIDTH;
		screens[s].height = WINDOW_HEIGHT / TILE_HEIGHT;
		screens[s].tiles = malloc(sizeof(SDL_Rect) * screens[s].width);

		for(int i = 0; i < screens[s].width; i++)
		{
			screens[s].tiles[i] = malloc(sizeof(Tile) * screens[s].height);
		}


		for(int x = 0; x < screens[s].width; x++)
		{
			for(int y = 0; y < screens[s].height; y++)
			{
				screens[s].tiles[x][y].type = map[x][y];
 
				int lol = map[x][y] - 1;

				screens[s].tiles[x][y].rect = (SDL_Rect){TILE_WIDTH * x, screens[s].pos.y + (TILE_HEIGHT * y), TILE_WIDTH, TILE_HEIGHT};

				switch(lol)
				{
					case WOOD1:
					screens[s].tiles[x][y].texture = WOOD1;
					break;

					case WOOD2:
					screens[s].tiles[x][y].texture = WOOD2;
					break;

					case WOOD3:
					screens[s].tiles[x][y].texture = WOOD3;
					break;

					case WOOD4:
					screens[s].tiles[x][y].texture = WOOD4;
					break;

					case WOOD5:
					screens[s].tiles[x][y].texture = WOOD5;
					break;

					case WOOD6:
					screens[s].tiles[x][y].texture = WOOD6;
					break;

					case WOOD7:
					screens[s].tiles[x][y].texture = WOOD7;
					break;

					case WOOD8:
					screens[s].tiles[x][y].texture = WOOD8;
					break;

					case WOOD9:
					screens[s].tiles[x][y].texture = WOOD9;
					break;

					case WOOD10:
					screens[s].tiles[x][y].texture = WOOD10;
					break;

					case WOOD11:
					screens[s].tiles[x][y].texture = WOOD11;
					break;

					case WOOD12:
					screens[s].tiles[x][y].texture = WOOD12;
					break;

					case WOOD13:
					screens[s].tiles[x][y].texture = WOOD13;
					break;
				}
			}
		}

	}

}

void UpdateGameManager()
{
	const Uint8 *key = SDL_GetKeyboardState(NULL);
	for(int i = 0; i < NUMBER_OF_SCREENS; i++)
	{
		SDL_Rect screenBounds = {screens[i].pos.x, screens[i].pos.y, WINDOW_WIDTH, WINDOW_HEIGHT};

		if(SDL_HasIntersection(&player->rect, &screenBounds))
			SetCameraPos(screens[i].pos);

	}

	player->vel.x = 0;

	if(key[SDL_SCANCODE_D])
	{
		player->vel.x += player->acc.x;
	}

	if(key[SDL_SCANCODE_A])
	{
		player->vel.x += -player->acc.x;
	}

	if(key[SDL_SCANCODE_SPACE] && !player->inAir)
	{
		player->vel.y -= player->acc.y;
		player->inAir = true;
	}

	player->vel.y += GRAVITY * GetDeltaTime();

	player->pos.x += player->vel.x * GetDeltaTime();
	player->pos.y += player->vel.y * GetDeltaTime();

	//Change for air jump
	player->inAir = true;

	for(int i = 0; i < NUMBER_OF_SCREENS; i++)
	{
		if(SDL_HasIntersection(&player->rect, &(SDL_Rect){screens[i].pos.x, screens[i].pos.y, WINDOW_WIDTH, WINDOW_HEIGHT}))
		{
			for(int y = 0; y < screens[i].height; y++)
				for(int x = 0; x < screens[i].width; x++)
				{
					SDL_Rect rect = screens[i].tiles[x][y].rect;

					if(screens[i].tiles[x][y].type == EMPTY_TILE)
						continue;

					PlayerRectCollision(player, rect);

					for(int i = 0; i < GetAmountOfNpcs(); i++)
					{
						NpcRectCollsion(GetNpc(i), rect);

						if(SDL_HasIntersection(&GetNpc(i)->rect, &player->rect))
						{
							KillNpc(i);
						}
					}

					for(int i = 0; i < GetAmountOfBullets(); i++)
					{
						if(SDL_HasIntersection(&rect, &GetBullet(i)->rect))
						{
							DeleteBullet(i);
						}
					}



				}
		}

	}

	/* Floor Collsion
	if(player->pos.y + player->rect.h >= 600)
	{
		player->pos.y = 600 - player->rect.h;
		player->vel.y = 0;
		player->inAir = false;
	}
	*/


	player->rect.x = (int)player->pos.x;
	player->rect.y = (int)player->pos.y;
}

void DrawGameObjects()
{
	for(int i = 0; i < NUMBER_OF_SCREENS; i++)
	{
		if(SDL_HasIntersection(&player->rect, &(SDL_Rect){screens[i].pos.x, screens[i].pos.y, WINDOW_WIDTH, WINDOW_HEIGHT}))
		{

			for(int y = 0; y < screens[i].height; y++)
				for(int x = 0; x < screens[i].width; x++)
				{
					if(i + 1 != NUMBER_OF_SCREENS && screens[i].tiles[x][y].type != EMPTY_TILE && screens[i].tiles[x][y].texture >= 0)
						DrawTexture(screens[i + 1].tiles[x][y].rect, GetTexture(screens[i].tiles[x][y].texture), WHITE);

					if(i - 1 != -1 && screens[i].tiles[x][y].type != EMPTY_TILE && screens[i].tiles[x][y].texture >= 0)
						DrawTexture(screens[i - 1].tiles[x][y].rect, GetTexture(screens[i].tiles[x][y].texture), WHITE);

					if(screens[i].tiles[x][y].type != EMPTY_TILE && screens[i].tiles[x][y].texture >= 0)

					if(screens[i].tiles[x][y].type != EMPTY_TILE && screens[i].tiles[x][y].texture >= 0)
						DrawTexture(screens[i].tiles[x][y].rect, GetTexture(screens[i].tiles[x][y].texture), WHITE);


				}
		}
	}
}


