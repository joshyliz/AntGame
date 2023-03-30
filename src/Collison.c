#include "Collison.h"
#include "Renderer.h"

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

bool TopCollision(SDL_Rect rec1, SDL_FPoint velocity1, SDL_Rect rec2)
{
    float Top1 = rec1.y;
    float Bottom1 = rec1.y + rec1.h;
    float Left1 = rec1.x;
    float Right1 = rec1.x + rec1.w;

    float Top2 = rec2.y;
    float Left2 = rec2.x;
    float Right2 = rec2.x + rec2.w;


    if (Bottom1 + velocity1.y * GetDeltaTime() > Top2 && Right1 > Left2 && Left1 < Right2
        && Top1 <= Top2)
        return true;
    else return false;
}

bool BottomCollision(SDL_Rect rec1, SDL_FPoint velocity1, SDL_Rect rec2)
{
    float Top1 = rec1.y;
    float Bottom1 = rec1.y + rec1.h;
    float Left1 = rec1.x;
    float Right1 = rec1.x + rec1.w; 

    float Bottom2 = rec2.y + rec2.h;
    float Left2 = rec2.x;
    float Right2 = rec2.x + rec2.w; 
       
    if (Top1 + velocity1.y * GetDeltaTime() < Bottom2 && Bottom1 > Bottom2
        && Right1 > Left2 && Left1 < Right2)
        return true;
    else return false;
}

bool LeftCollision(SDL_Rect rec1, SDL_FPoint velocity1, SDL_Rect rec2)
{
    float Top1 = rec1.y;
    float Bottom1 = rec1.y + rec1.h;
    float Left1 = rec1.x;
    float Right1 = rec1.x + rec1.w;

    float Top2 = rec2.y;
    float Bottom2 = rec2.y + rec2.h;
    float Left2 = rec2.x;


    if (Right1 + velocity1.x * GetDeltaTime() > Left2 && Left1 < Left2
        && Bottom1 > Top2 && Top1 < Bottom2)
        return true;
    else return false;
}

bool RightCollsion(SDL_Rect rec1, SDL_FPoint velocity1, SDL_Rect rec2)
{
    float Top1 = rec1.y;
    float Bottom1 = rec1.y + rec1.h; 
    float Left1 = rec1.x; 
    float Right1 = rec1.x + rec1.w;

    float Top2 = rec2.y;
    float Bottom2 = rec2.y + rec2.h; 
    float Right2 = rec2.x + rec2.w;

    if (Left1 + velocity1.x * GetDeltaTime() < Right2 && Right1 > Right2
        && Bottom1 > Top2 && Top1 < Bottom2)
        return true;
    else return false;
}
