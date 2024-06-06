#include "objects.h"
#ifndef GUI_H
#define GUI_H

#define MAP_COLUMNS 20
#define MAP_LINES 15
#define TILE_SIZE 16

typedef enum {
    SCREEN_TITLE,
    SCREEN_TUTORIAL,
    SCREEN_GAME,
    SCREEN_PAUSE,
    SCREEN_DEATH
} GameScreen;

void checkButtonStart (GameScreen *screen); // Ativa o botão de início
void checkButtonRevive (GameScreen *screen, Player *player); // Faz a mecânica de reviver
void checkButtonLifePlus (Player *player, Stats *stats); // Ativa o botão de aumentar vida
void checkButtonAttackPlus (Player *player, Stats *stats); // Ativa o botão de aumentar ataque

void drawScore (Player *player);
void drawLevelIndicator (Level *level);
void drawStats (Stats *stats, Texture2D *tileset);

void drawBackgroundTitle (Texture2D texture);
void drawTutorialScreen (Texture2D *tileset, Texture2D *speechBaloon);

void drawCursor (Texture2D cursorTexture);
void drawAttackPlusButton (Stats *stats, Texture2D tileset);
void drawLifePlusButton (Stats *stats, Texture2D tileset);

#endif