#include "Bullet.h"
#include "Textures.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Renderer.h"

static int amountOfBullets = 0;
static Bullet *bullets;

static float angle = 0;

void InitBulletManager()
{
	bullets = malloc(0);			
}

void SpawnBullet(SDL_FPoint pos, SDL_FPoint vel, BULLET_TYPE type)
{
	amountOfBullets++;
	bullets = realloc(bullets, sizeof(Bullet) * amountOfBullets);
	Bullet bullet = 
	{
		(SDL_Rect){pos.x, pos.y, 10, 5},
		pos,
		vel,
		type,
	};
	bullets[amountOfBullets - 1] = bullet; 
}

void SpawnBulletEz(Bullet bullet)
{
	amountOfBullets++;
	bullets = realloc(bullets, sizeof(Bullet) * amountOfBullets);
	bullets[amountOfBullets - 1] = bullet; 
}

void DeleteBullet(int index)
{
	for(int i = index; i < amountOfBullets; i++)
	{
		if(i + 1 != amountOfBullets)
			bullets[i] = bullets[i + 1];
	}

	amountOfBullets--;
	bullets = realloc(bullets, sizeof(Bullet) * amountOfBullets);
}

Bullet* GetBullet(int index)
{
	return &bullets[index];
}

int GetAmountOfBullets()
{
	return amountOfBullets;
}

void UpdateBulletManager()
{
	if(amountOfBullets > 0)
		for(int i = 0; i < amountOfBullets; i++)
		{
			Bullet *bullet = &bullets[i];
			bullet->rect.x = bullet->pos.x;
			bullet->rect.y = bullet->pos.y;
			bullet->pos.x += bullet->vel.x * GetDeltaTime();
			bullet->pos.y += bullet->vel.y * GetDeltaTime();
		}
}

void DrawBullets()
{
	if(amountOfBullets > 0)
		for(int i = 0; i < amountOfBullets; i++)
		{
			//DrawTexture(bullets[i].rect, GetTexture(1), WHITE);
			Bullet bullet = bullets[i];

			if(bullet.vel.y < 0)
				angle = bullet.vel.y - (bullet.vel.x);
			else if(bullet.vel.y > 0)
				angle = bullet.vel.y - -(bullet.vel.x);
				

			angle = RadToDeg(atan2f(bullet.vel.y, bullet.vel.x)); 
			
			DrawTextureExtra(bullet.rect, RECT_ZERO, angle, GetTexture(1), WHITE);
		}
}





