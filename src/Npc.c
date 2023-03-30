#include "Npc.h"
#include "Renderer.h"
#include "Textures.h"
#include "defines.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Collison.h"

static Npc *npcs;
static int amountOfNpcs = 0; 


void InitNpcManager()
{
	npcs = malloc(0);
}

void SpawnNpc(SDL_FPoint pos, NPC_TYPE type)
{
	amountOfNpcs++;
	npcs = realloc(npcs, sizeof(Npc) * amountOfNpcs);
	
	Npc npc;

	npc.pos = pos;
	npc.moveTo = POINTF_ZERO;
	npc.type = type;
	npc.vel = POINTF_ZERO;
	npc.inAir = false;

	switch(type)
	{
		case DEFUALT_NPC_TYPE:
		npc.rect = (SDL_Rect){pos.x, pos.y, 50, 50};
		break;

		case DUMB_BOMBER:
		npc.rect = (SDL_Rect){pos.x, pos.y, 15, 15};
		break;
	}

	npcs[amountOfNpcs - 1] = npc; 
}

void KillNpc(int index)
{
	for(int i = index; i < amountOfNpcs; i++)
	{
		if(i + 1 != amountOfNpcs)
			npcs[i] = npcs[i + 1];
	}

	if(amountOfNpcs == 1)
	{
		amountOfNpcs--;
		npcs = realloc(npcs, sizeof(Npc));
	}
	else
	{
		amountOfNpcs--;
		npcs = realloc(npcs, sizeof(Npc) * amountOfNpcs);
	}
}

void NpcRectCollsion(Npc *npc, SDL_Rect rect)
{
	if(TopCollision(npc->rect, npc->vel, rect))
	{
		npc->pos.y = rect.y - npc->rect.h;
		npc->inAir = false;
		npc->vel.y = 0;
	}

	if(BottomCollision(npc->rect, npc->vel, rect))
	{
		npc->pos.y = rect.y + rect.h;
		npc->vel.y = 0;
	}

	if(LeftCollision(npc->rect, npc->vel, rect))
	{
		npc->pos.x = rect.x - npc->rect.w;
		if(npc->inAir == false)
		{
			npc->vel.y -= 450;	
			npc->inAir = true;
		}
		npc->vel.x = 0;
	}

	if(RightCollsion(npc->rect, npc->vel, rect))
	{
		npc->pos.x = rect.x + rect.w;
		if(npc->inAir == false)
		{
			npc->vel.y -= 450;	
			npc->inAir = true;
		}
		npc->vel.x = 0;
	}
}

Npc* GetNpc(int index)
{
	return &npcs[index];
}

int GetAmountOfNpcs()
{
	return amountOfNpcs;
}

void UpdateNpcs()
{
	if(amountOfNpcs > 0)
		for(int i = 0; i < amountOfNpcs; i++)
		{
			Npc *npc = &npcs[i];

			npc->rect.x = npc->pos.x;
			npc->rect.y = npc->pos.y;

			npc->vel.x = 0;

			npc->vel.y += 100 * GetDeltaTime();

			SDL_FPoint dir = {npc->moveTo.x - npc->pos.x, npc->moveTo.y - npc->pos.y};
			float w = sqrt(dir.x * dir.x + dir.y * dir.y);

			dir.x /= w;
			dir.y /= w;

			float dist = sqrt(pow(npc->pos.x - npc->moveTo.x, 2) + 
					pow(npc->pos.y - npc->moveTo.y, 2));

			if(!(npc->moveTo.x == 0 && npc->moveTo.y == 0) && dist > 10)
				switch(npc->type)
				{
					case DEFUALT_NPC_TYPE:
					break;

					case DUMB_BOMBER:

					if(dir.x > 0)
					{
						npc->vel.x += 300;
					}
					else if(dir.x < 0)
					{
						npc->vel.x -= 300;
					}

					break;
				}

			
			npc->pos.x += npc->vel.x * GetDeltaTime();
			npc->pos.y += npc->vel.y * GetDeltaTime();
		}
}

void DrawNpcs()
{
	if(amountOfNpcs > 0)
		for(int i = 0; i < amountOfNpcs; i++)
			DrawTexture(npcs[i].rect, GetTexture(1), WHITE);
}

/*
Npc RectCollsion(SDL_Rect rect)
{
	SDL_FPoint dir = {pos.x - npc.pos.x, pos.y - npc.pos.y};

	npc.vel.y += GRAVITY * GetDeltaTime();

	npc.pos.x += (npc.vel.x * 100) * GetDeltaTime();
	npc.pos.y += npc.vel.y * GetDeltaTime();
	npc.rect.x = npc.pos.x;
	npc.rect.y = npc.pos.y;

	if(npc.pos.x != pos.x && npc.pos.y != pos.y)
	{
		npc.vel.x = dir.x;

		float w = sqrt( npc.vel.x * npc.vel.x + npc.vel.y * npc.vel.y);
		npc.vel.x /= w;
		//npc.vel.y /= w;
	}



		if(TopCollision(npc.rect, npc.vel, rect))
		{
			npc.pos.y = rect.y - npc.rect.h;
			npc.vel.y = 0;
		}

		if(BottomCollision(npc.rect, npc.vel, rect))
		{
			npc.pos.y = rect.y + rect.h;
			npc.vel.y = 0;
		}

		if(LeftCollision(npc.rect, npc.vel, rect))
		{
			npc.pos.x = rect.x - npc.rect.w;
			npc.vel.x = 0;
			npc.vel.y -= 1000;
		}

		if(RightCollsion(npc.rect, npc.vel, rect))
		{
			npc.pos.x = rect.x + rect.w;
			npc.vel.x = 0;
			npc.vel.y -= 1000;
		}

}
*/
