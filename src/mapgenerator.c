#include "raylib.h"
#include <stdio.h>
#include "mapgenerator.h"

#define MAP_WIDTH 20
#define MAP_HEIGHT 15

#define TILE_SIZE 16

int calcTile(int n){
    return (n - 1) * (TILE_SIZE + 1);
}

int calcTilesSize (int n){
    return n * 3 * TILE_SIZE;
}

void FiletoMap(FILE *f, Texture2D tileset){
    char tileChar;
    Rectangle tileRect = {.width = TILE_SIZE, .height = TILE_SIZE}, placeRect = {.width = TILE_SIZE * 3, .height = TILE_SIZE * 3};
    int x = 0, y =0;

    // Desenhamos chão para por objetos
    DrawTexturePro(tileset, 
        (Rectangle){.x = calcTile(1), .y = calcTile(5), .width = TILE_SIZE, .height = TILE_SIZE},
        (Rectangle){.x = 0, 0, .width = MAP_WIDTH * 3 * TILE_SIZE, .height = MAP_HEIGHT * 3 * TILE_SIZE},
        (Vector2){0, 0}, 0.0f, WHITE);

    rewind(f);

    for (int t = 0; t < (MAP_WIDTH + 1) * MAP_HEIGHT; t++){
        tileChar = fgetc(f);

        switch(tileChar){
            // Topo Muro Frente
            case 'T':
                tileRect.x = calcTile(3);
                tileRect.y = calcTile(1);
                break;
            // Terra sem detalhes
            case 'S':
                tileRect.x = calcTile(1);
                tileRect.y = calcTile(1);
                break;
            // Muro lado esquerdo
            case 'E':
                tileRect.x = calcTile(10);
                tileRect.y = calcTile(5);
                break;
            // Muro
            case 'M':
                tileRect.x = calcTile(5);
                tileRect.y = calcTile(4);
                break;
            // Pedras na areia
            case 'O':
                tileRect.x = calcTile(2);
                tileRect.y = calcTile(5);
                break;
            // Areia sem detalhes
            case 'X':
                tileRect.x = calcTile(1);
                tileRect.y = calcTile(5);
                break;
            // Esquina direita inferior muro
            case 'C':
                tileRect.x = calcTile(6);
                tileRect.y = calcTile(2);
                break;
            // Margem direita muro
            case 'D':
                tileRect.x = calcTile(12);
                tileRect.y = calcTile(5);
                break;
            // Muro subir terra lado esquerdo
            case 'U':
                tileRect.x = calcTile(2);
                tileRect.y = calcTile(2);
                break;
            // Muro esquina sobe->direita
            case '>':
                tileRect.x = calcTile(2);
                tileRect.y = calcTile(1);
                break;
            // Sombra muro frente
            case '~':
                tileRect.x = calcTile(3);
                tileRect.y = calcTile(5);
                break;
            // Sombra esquina sobe
            case '^':
                tileRect.x = calcTile(6);
                tileRect.y = calcTile(5);
                break;
            // porta
            case 'P':
                tileRect.x = calcTile(10);
                tileRect.y = calcTile(4);
                break;
            // Barril
            case 'B':
                tileRect.x = calcTile(11);
                tileRect.y = calcTile(7);
                break;
            // Bigorna
            case 'H':
                tileRect.x = calcTile(3);
                tileRect.y = calcTile(7);
                break;
            // Banner
            case '-':
                tileRect.x = calcTile(6);
                tileRect.y = calcTile(3);
                break;
            // Cerca
            case 'Y':
                tileRect.x = calcTile(6);
                tileRect.y = calcTile(7);
                break;
            // Lápide
            case 'L':
                tileRect.x = calcTile(6);
                tileRect.y = calcTile(6);
                break;
            // Túmulo
            case 'V':
                tileRect.x = calcTile(7);
                tileRect.y = calcTile(6);
                break;
            default:
                break;
        }
        
        placeRect.x = x * TILE_SIZE * 3;
        placeRect.y = y * TILE_SIZE * 3;
        
        DrawTexturePro(tileset, tileRect, placeRect, (Vector2){0, 0}, 0, WHITE);

        x++;

        if (tileChar == '\n'){
            x = 0;
            y++;
        };

        if (x >= MAP_WIDTH + 1){
            x = 0;
            y ++;
        }
    }
}

void drawMap(RenderTexture2D mapTexture){
    DrawTexturePro(mapTexture.texture, 
        (Rectangle){ 0, 0, mapTexture.texture.width, -mapTexture.texture.height },
        (Rectangle){ 0, 0, mapTexture.texture.width, mapTexture.texture.height },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);
}