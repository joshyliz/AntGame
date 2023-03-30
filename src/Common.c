#include "Common.h"
#include <stdio.h>

void PlayerRectCollision(Player *player, SDL_Rect rect)
{
	if(TopCollision(player->rect, player->vel, rect))
	{
		player->pos.y = rect.y - player->rect.h;
		player->inAir = false;
		player->vel.y = 0;
	}

	if(BottomCollision(player->rect, player->vel, rect))
	{
		player->pos.y = rect.y + rect.h;
		player->vel.y = 0;
	}

	if(LeftCollision(player->rect, player->vel, rect))
	{
		player->pos.x = rect.x - player->rect.w;
		player->vel.x = 0;
	}

	if(RightCollsion(player->rect, player->vel, rect))
	{
		player->pos.x = rect.x + rect.w;
		player->vel.x = 0;
	}
}
