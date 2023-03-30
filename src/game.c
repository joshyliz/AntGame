#include "game.h"
#include "GameManager.h"
#include "defines.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Textures.h"
#include "ReadMap.h"
#include "Bullet.h"
#include "Npc.h"

static Player player;

double timer = 0.1;

char fpsCounter[10];

float allTime = 0;

char timeCount[100];

void Start()
{
	StartWindow("Ants Get Nasty", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

	StartGameManager(&player);
	InitNpcManager();
	InitBulletManager();
	

	SpawnNpc((SDL_FPoint){100, 300}, DUMB_BOMBER);

	player.pos.x = 350;
	player.pos.y = 100;
	player.vel.x = 0;
	player.vel.y = 0;
	player.acc.x = 350;
	player.acc.y = 450;
	player.health = 100;

	player.rect = (SDL_Rect){(int)player.pos.x, (int)player.pos.y, 15, 15};

	player.inAir = false;

	//Load Images and Fonts
	LoadTextures();
	LoadFonts();
}

void Update()
{
	EventChecks();
	const Uint8 *key = SDL_GetKeyboardState(NULL);

	if(key[SDL_SCANCODE_ESCAPE])
		QuitLoop();	

	static SDL_FPoint BulletDir = {0, 0};

	int x = 0; 
	int y = 0;

	if(SDL_GetMouseState(&x, &y) == SDL_BUTTON_LMASK)
	{
		BulletDir = (SDL_FPoint){x - player.pos.x, y - player.pos.y};

		float w = sqrt(pow(BulletDir.x, 2) + pow(BulletDir.y, 2));
		BulletDir.x /= w;
		BulletDir.y /= w;
		BulletDir.x *= 100;
		BulletDir.y *= 100;
		SpawnBullet(player.pos, BulletDir, BASIC_BULLET);
	}

	
	allTime += (float)GetDeltaTime();
	UpdateNpcs();
	UpdateGameManager();
	UpdateBulletManager();
}

void Render()
{
	BeginDrawing((SDL_Color){70, 70, 70});

	timer -= GetDeltaTime();
	
	if(timer < 0)
	{
		sprintf(fpsCounter, "%d", GetFps()); 
		timer = 0.1; 
	}

	sprintf(timeCount, "%f", allTime);

	DrawGameObjects();

	DrawTexture(player.rect, GetTexture(WOOD1), WHITE);

	DrawBullets();

	DrawNpcs();

	DrawFont(GetFont(ARIAL), GetCameraPos(), fpsCounter, WHITE);

	DrawFont(GetFont(ARIAL), (SDL_FPoint){GetCameraPos().x, GetCameraPos().y + 48}, timeCount, WHITE);

	EndDrawing();
}

