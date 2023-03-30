#include "ReadMap.h"
#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void GetMapFromFile(const char *path, int **dest)
{
	int width = WINDOW_WIDTH / TILE_WIDTH;
	int height = WINDOW_HEIGHT / TILE_HEIGHT;

	FILE *file = fopen(path, "r");

	if(file == NULL)
	{
		printf("Failed to find %s level file\n", path);
		exit(0);
	}

	for(int x = 0; x < width; x++)
		for(int y = 0; y < height; y++)
		{
			 fread(&dest[x][y], sizeof(int), 1, file);
		}



	fclose(file);
}
