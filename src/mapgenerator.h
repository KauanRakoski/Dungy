#include <stdio.h>

#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#define MAP_WIDTH 20
#define MAP_HEIGHT 15

#define TILE_SIZE 16

int calcTile (int n);
int calcTilesSize (int n);
void FiletoMap(FILE *f, Texture2D tileset);
void drawMap(RenderTexture2D mapTexture);

#endif