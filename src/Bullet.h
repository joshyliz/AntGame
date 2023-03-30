#pragma once
#include <SDL2/SDL.h>

typedef enum
{
	BASIC_BULLET,
} BULLET_TYPE;

typedef struct 
{
	SDL_Rect rect;
	SDL_FPoint pos;
	SDL_FPoint vel;
	BULLET_TYPE type;
} Bullet;

void InitBulletManager();

void SpawnBullet(SDL_FPoint pos, SDL_FPoint vel, BULLET_TYPE type);

void SpawnBulletEz(Bullet bullet);

void DeleteBullet(int index);

Bullet* GetBullet(int index);

int GetAmountOfBullets();

void UpdateBulletManager();

void DrawBullets();

