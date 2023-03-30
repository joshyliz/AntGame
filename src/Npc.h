#pragma once
#include <SDL2/SDL.h>
#include "structs.h"

typedef enum
{
	DEFUALT_NPC_TYPE,
	DUMB_BOMBER,
} NPC_TYPE;

typedef struct 
{
	SDL_Rect rect;
	SDL_FPoint pos;
	SDL_FPoint vel;
	SDL_FPoint moveTo;
	NPC_TYPE type;
	bool inAir;
} Npc;

void InitNpcManager();

void SpawnNpc(SDL_FPoint pos, NPC_TYPE type);

void KillNpc(int index);

void NpcRectCollsion(Npc *npc, SDL_Rect rect);

Npc* GetNpc(int index);

int GetAmountOfNpcs();

void UpdateNpcs();

void DrawNpcs();
