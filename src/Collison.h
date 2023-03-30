#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* Collsion Variables

    float Top1 = rec1.y;
    float Bottom1 = rec1.y + rec1.height; 
    float Left1 = rec1.x; 
    float Right1 = rec1.x + rec1.width;

    float Top2 = rec2.y;
    float Bottom2 = rec2.y + rec2.height; 
    float Left2 = rec2.x; 
    float Right2 = rec2.x + rec2.width;

*/


//Side Collsion Checks

bool TopCollision(SDL_Rect rec1, SDL_FPoint velocity1, SDL_Rect rec2);

bool BottomCollision(SDL_Rect rec1, SDL_FPoint velocity1, SDL_Rect rec2);

bool LeftCollision(SDL_Rect rec1, SDL_FPoint velocity1, SDL_Rect rec2);

bool RightCollsion(SDL_Rect rec1, SDL_FPoint velocity1, SDL_Rect rec2);

