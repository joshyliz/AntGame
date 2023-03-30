#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define RECT_ZERO (SDL_Rect){0, 0, 0, 0}
#define POINTF_ZERO (SDL_FPoint){0, 0}
#define POINT_ZERO (SDL_Point){0, 0}

#define WHITE (SDL_Color){255, 255, 255, 255}
#define BLACK (SDL_Color){0, 0, 0, 255}

typedef struct 
{
	unsigned int width;
	unsigned int height;
	unsigned int data;
	unsigned int ID;
} Texture;

typedef struct
{
	Texture *texture;	
	char letter;
} Character;

typedef struct
{
	Texture *characters;
} Font;

void StartWindow(const char *title, int width, int height, Uint32 flags);

Texture* LoadTexture(const char *filePath);

Font LoadFont(const char *filePath, int size);

void EventChecks();

void BeginDrawing(SDL_Color color);

void EndDrawing();

void DrawTexture(SDL_Rect rectangle, Texture *texture, SDL_Color color);

void DrawTextureExtra(SDL_Rect destRect, SDL_Rect srcRect, float angle, Texture *texture, SDL_Color color);

void DrawFont(Font font, SDL_FPoint pos, const char *text, SDL_Color color);

void DrawLine(SDL_FPoint point1, SDL_FPoint point2, float width, SDL_Color color);

void DrawRectangle(SDL_Rect rect, SDL_Color color);

void DrawRectangleExtra(SDL_Rect rect, SDL_Color color, float angle);

void DrawRectangleOutLine(SDL_Rect rect, SDL_Color color);

double GetDeltaTime();

int GetFps();

bool IsRunning();

void SetWantedFps(int fps);

void QuitWindow();

void QuitLoop();

SDL_Window* GetSDLWindow();

int GetWindowWidth();

int GetWindowHeight();

void SetCameraPos(SDL_FPoint pos);

SDL_FPoint GetCameraPos();

float RadToDeg(float radians);

