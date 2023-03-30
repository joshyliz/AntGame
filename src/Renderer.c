#include "Renderer.h"
#include "util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "write.h"

static SDL_Window *window = NULL;
static SDL_GLContext context;

unsigned int VBO, VAO;
unsigned int shaderProgram; 

unsigned int LINE_VBO, LINE_VAO;

double deltaTime = 0;
Uint64 NOW;
Uint64 LAST;
int fps = 0;
int wantedFps = 60;

bool isRunning;

SDL_FPoint camera = {0, 0};

//Shader var locations
GLint imageLoc; 
GLint colorLoc;
GLint projectionLoc;
GLint modelLoc;
GLint srcSDL_RectLoc;
GLint renderFlatLoc;

float vertices[] = { 
	// pos      // tex
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 

	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f
};

float line[] = {
	0.0, 1.0,
	1.0, 0.0
};


mat4 projection = GLM_MAT4_IDENTITY_INIT;

static unsigned int textureNum = 0;

void StartWindow(const char *title, int width, int height, Uint32 flags)
{
	//Init SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not be initialized\nError Code: %s\n", SDL_GetError());
		isRunning = false;
	}
	else 
	{
		printf("SDL successfully started!\n");
		isRunning = true;
	}

	if(!IMG_Init(IMG_INIT_PNG))
		printf("IMG_Init failed ERROR: %s\n", SDL_GetError());

	if(!IMG_Init(IMG_INIT_JPG))
		printf("IMG_Init failed ERROR: %s\n", SDL_GetError());

	if(TTF_Init() < 0)	
		printf("TTF_Init faile ERROR %s\n", SDL_GetError());


	//Link SDL to opengl
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	//Make SDL Opengl Window
	window = SDL_CreateWindow(title, 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 
			width, 
			height, 
			flags | SDL_WINDOW_OPENGL);

	//Make Opengl Context
	context = SDL_GL_CreateContext(window);
	
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	//Set the viewport
	glViewport(0, 0, width, height);

	//Shaders source code	
	const char *vertexShaderSource = readFile("vert.vs");
	const char *fragmentShaderSource = readFile("frag.fs");

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

	//VBO VAO EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


	glGenVertexArrays(1, &LINE_VAO);
	glGenBuffers(1, &LINE_VBO);
	glBindVertexArray(LINE_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, LINE_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	glEnableVertexAttribArray(0);

	
	//Init Variables
	glUseProgram(shaderProgram);
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//Delta time initialize
	NOW = SDL_GetPerformanceCounter();
	LAST = 0;

	//Enables Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//For text rendering

	//Shader locations
	imageLoc = glGetUniformLocation(shaderProgram, "image"); 
	colorLoc = glGetUniformLocation(shaderProgram, "spriteColor"); 
	projectionLoc = glGetUniformLocation(shaderProgram, "projection"); 
	modelLoc = glGetUniformLocation(shaderProgram, "model"); 
	srcSDL_RectLoc = glGetUniformLocation(shaderProgram, "srcRectangle");
	renderFlatLoc = glGetUniformLocation(shaderProgram, "renderFlat");

	glm_ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f, projection);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)projection);
}

Texture* LoadTexture(const char *filePath)
{
	GLuint TextureID = 0;
	 
	SDL_Surface *Surface = IMG_Load(filePath);

	if(Surface == NULL)
	{
		printf("Failed to load texture: %s\n", filePath);
		return NULL;
	}
	else printf("\"%s\" successfully loaded\n", filePath);

	//flip_surface(Surface); If you want to load image fliped
	 
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 
	int Mode = GL_RGB;
	 
	if(Surface->format->BytesPerPixel == 4) 
	{
		Mode = GL_RGBA;
	}
	 
	glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, 
			GL_UNSIGNED_BYTE, Surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	 
	//Texture texture = {Surface->w, Surface->h, textureNum, TextureID};
	
	Texture *texture = malloc(sizeof(Texture));

	*texture = (Texture){Surface->w, Surface->h, textureNum, TextureID};

	SDL_FreeSurface(Surface);

	glBindTexture(GL_TEXTURE_2D, TextureID);

	textureNum++;

	return texture;
}

Font LoadFont(const char *filePath, int size)
{
	Texture *characters = calloc(128, sizeof(Texture));
	TTF_Font *ttFont = TTF_OpenFont(filePath, size);		
	
	for(unsigned char c = 0; c < 128; c++)
	{
		if(c < 1)
			continue;

		GLuint TextureID = 0;
		char a[] = " ";  

		a[0] = c;


		SDL_Surface *Surface = TTF_RenderUTF8_Blended(ttFont, a, (SDL_Color){255, 255, 255});

		SDL_Surface *interm;

		interm = SDL_CreateRGBSurface(0, Surface->w ^ 2, Surface->h ^ 2, 32, 
				0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

		SDL_BlitSurface(Surface, NULL, interm, NULL);

		//Turn Off Later
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		 
		int Mode = GL_RGB;
		 
		if(Surface->format->BytesPerPixel == 4) 
		{
			Mode = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, Mode, interm->w, interm->h, 0, Mode, 
				GL_UNSIGNED_BYTE, interm->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		characters[c] = (Texture){interm->w, interm->h, textureNum, TextureID};

		
		textureNum++;

		SDL_FreeSurface(Surface);
		SDL_FreeSurface(interm);
	}
	

	TTF_CloseFont(ttFont);
	Font font;
	font.characters = characters;

	return font;
}

void EventChecks()
{
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();
	//deltaTime = ((double)((NOW - LAST) * 1000 / (double) SDL_GetPerformanceFrequency())) * 0.001;
	deltaTime = ((double)((NOW - LAST) * 1000 / (double) SDL_GetPerformanceFrequency()));

	SDL_Event event;
	SDL_PollEvent(&event);

	if(event.type == SDL_QUIT)
		isRunning = false;

	if(deltaTime >! 1000/(double)fps)
	{
		if(wantedFps != 0)
			SDL_Delay(1000 / wantedFps);
	}

	fps = 1000 / deltaTime; 

	int w = 0;
	int h = 0;
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);
}

void BeginDrawing(SDL_Color color)
{
	glClearColor(color.r * ((float)1 / 255), color.g * ((float)1 / 255), color.b * ((float)1 / 255), 1.0f);
	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
}

void EndDrawing()
{
	SDL_GL_SwapWindow(window);
}

void DrawTexture(SDL_Rect rectangle, Texture *texture, SDL_Color color)
{
	rectangle.x = rectangle.x - camera.x;
	rectangle.y = rectangle.y - camera.y;


	vec2 size = {rectangle.w, rectangle.h};

	mat4 model = GLM_MAT4_IDENTITY_INIT;

	glm_translate(model, (vec3){rectangle.x, rectangle.y, 0});
	glm_translate(model, (vec3){0.5f * size[0], 0.5f * size[1], 0.0f});
	glm_rotate(model, glm_rad(0), (vec3){0.0f, 0.0f, 1.0f});
	glm_translate(model, (vec3){-0.5f * size[0], -0.5f * size[1], 0.0f});
	glm_scale(model, (vec3){size[0], size[1], 1.0f});

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
	glUniform3fv(colorLoc, 1, (vec3){color.r * ((float)1 / 255), color.g * ((float)1 / 255), color.b * ((float)1 / 255)});
	glUniform1i(imageLoc, texture->data);
	glUniform4fv(srcSDL_RectLoc, 1, (vec4){0, 0, 1, 1});
	glUniform1i(renderFlatLoc, 0);

	glActiveTexture(GL_TEXTURE0 + texture->data);
	glBindTexture(GL_TEXTURE_2D, texture->ID);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void DrawTextureExtra(SDL_Rect destRect, SDL_Rect srcRect, float angle, Texture *texture, SDL_Color color)
{
	destRect.x = destRect.x - camera.x;
	destRect.y = destRect.y - camera.y;
	vec2 size = {destRect.w, destRect.h};

	mat4 model = GLM_MAT4_IDENTITY_INIT;

	glm_translate(model, (vec3){destRect.x, destRect.y, 0});
	glm_translate(model, (vec3){0.5f * size[0], 0.5f * size[1], 0.0f});//Might Be uselsess

	if(angle != 0)
		glm_rotate(model, glm_rad(angle), (vec3){0.0f, 0.0f, 1.0f});

	glm_translate(model, (vec3){-0.5f * size[0], -0.5f * size[1], 0.0f});
	glm_scale(model, (vec3){size[0], size[1], 1.0f});

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
	glUniform3fv(colorLoc, 1, (vec3){color.r * ((float)1 / 255), color.g * ((float)1 / 255), color.b * ((float)1 / 255)});
	glUniform1i(imageLoc, texture->data);
	glUniform1i(renderFlatLoc, 0);
	
	if(srcRect.w == 0 || srcRect.h == 0)
	{

		glUniform4fv(srcSDL_RectLoc, 1, 
				(vec4){srcRect.x, srcRect.y, 
				1, 1});
	}
	else 
	{
		glUniform4fv(srcSDL_RectLoc, 1, 
				(vec4){srcRect.x, srcRect.y, 
				texture->width / (float)srcRect.w, texture->height / (float)srcRect.h});
	}



	glActiveTexture(GL_TEXTURE0 + texture->data);
	glBindTexture(GL_TEXTURE_2D, texture->ID);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void DrawFont(Font font, SDL_FPoint pos, const char *text, SDL_Color color)
{
	int next = 0;

	for(int i = 0; i < strlen(text); i++)
	{
		SDL_FPoint charPos = {pos.x + next, pos.y};	

		char letter = text[i];

		DrawTexture((SDL_Rect){charPos.x, charPos.y, 
				font.characters[(int)letter].width,
				font.characters[(int)letter].height}, &font.characters[(int)letter], color);

		next += font.characters[(int)letter].width;
	}
}

void DrawLine(SDL_FPoint point1, SDL_FPoint point2, float width, SDL_Color color)
{
	float lines[] = {
		point2.x, point2.y,
		point1.x, point1.y,
	};

	mat4 model = GLM_MAT4_IDENTITY_INIT;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);

	glLineWidth(width);
	glUniform3fv(colorLoc, 1, (vec3){color.r * ((float)1 / 255), color.g * ((float)1 / 255), color.b * ((float)1 / 255)});

	glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
	glUniform1i(renderFlatLoc, 1);
	glBindVertexArray(LINE_VAO); 
    glDrawArrays(GL_LINES, 0, 2);
}

void DrawRectangle(SDL_Rect rect, SDL_Color color)
{
	rect.x = rect.x - camera.x;
	rect.y = rect.y - camera.y;

	vec2 size = {rect.w, rect.h};

	mat4 model = GLM_MAT4_IDENTITY_INIT;

	glm_translate(model, (vec3){rect.x, rect.y, 0});
	glm_translate(model, (vec3){0.5f * size[0], 0.5f * size[1], 0.0f});
	glm_rotate(model, glm_rad(0), (vec3){0.0f, 0.0f, 1.0f});
	glm_translate(model, (vec3){-0.5f * size[0], -0.5f * size[1], 0.0f});
	glm_scale(model, (vec3){size[0], size[1], 1.0f});

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
	glUniform3fv(colorLoc, 1, (vec3){color.r * ((float)1 / 255), color.g * ((float)1 / 255), color.b * ((float)1 / 255)});
	glUniform4fv(srcSDL_RectLoc, 1, (vec4){0, 0, 1, 1});
	glUniform1i(renderFlatLoc, 1);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void DrawRectangleExtra(SDL_Rect rect, SDL_Color color, float angle)
{
	rect.x = rect.x - camera.x;
	rect.y = rect.y - camera.y;


	vec2 size = {rect.w, rect.h};

	mat4 model = GLM_MAT4_IDENTITY_INIT;

	glm_translate(model, (vec3){rect.x, rect.y, 0});
	glm_translate(model, (vec3){0.5f * size[0], 0.5f * size[1], 0.0f});
	glm_rotate(model, glm_rad(angle), (vec3){0.0f, 0.0f, 1.0f});
	glm_translate(model, (vec3){-0.5f * size[0], -0.5f * size[1], 0.0f});
	glm_scale(model, (vec3){size[0], size[1], 1.0f});

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
	glUniform3fv(colorLoc, 1, (vec3){color.r * ((float)1 / 255), color.g * ((float)1 / 255), color.b * ((float)1 / 255)});
	glUniform4fv(srcSDL_RectLoc, 1, (vec4){0, 0, 1, 1});
	glUniform1i(renderFlatLoc, 1);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void DrawRectangleOutLine(SDL_Rect rect, SDL_Color color)
{
	DrawLine((SDL_FPoint){rect.x, rect.y}, (SDL_FPoint){rect.x + rect.w, rect.y}, 1, color);
	DrawLine((SDL_FPoint){rect.x, rect.y + rect.h}, (SDL_FPoint){rect.x + rect.w, rect.y + rect.h}, 1, color);

	DrawLine((SDL_FPoint){rect.x, rect.y}, (SDL_FPoint){rect.x, rect.y + rect.h}, 1, color);
	DrawLine((SDL_FPoint){rect.x + rect.w, rect.y}, (SDL_FPoint){rect.x + rect.w, rect.y + rect.h}, 1, color);
}

double GetDeltaTime()
{
	return deltaTime * 0.001;
}

int GetFps()
{
	return fps;
}

bool IsRunning()
{
	return isRunning; 
}

void SetWantedFps(int fps)
{
	wantedFps = fps;
}

void QuitWindow()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void QuitLoop()
{
	isRunning = false;
}

SDL_Window* GetSDLWindow()
{
	return window;
}

int GetWindowWidth()
{
	int w;
	SDL_GetWindowSize(window, &w, NULL);
	return w;
}

int GetWindowHeight()
{
	int h;
	SDL_GetWindowSize(window, NULL, &h);
	return h;
}

bool CheckCollsion(SDL_Rect rect1, SDL_Rect rect2)
{
	if(rect1.x < rect2.x + rect2.w &&
		rect1.x + rect1.w > rect2.x &&
		rect1.y < rect2.y + rect2.h &&
		rect1.y + rect1.h > rect2.y)
	{
		return true;
	}

	else return false;
}

void SetCameraPos(SDL_FPoint pos)
{
	camera = pos;
}

SDL_FPoint GetCameraPos()
{
	return camera;
}

float RadToDeg(float radians)
{
	return radians * (180 / M_PI);
}
