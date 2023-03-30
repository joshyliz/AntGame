#pragma once
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//My libs
#include "util.h"
#include "Renderer.h"
#include "Collison.h"

//3rd party libs
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

void Start();

void Update();

void Render();

