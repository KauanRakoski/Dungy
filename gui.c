#include <stdio.h>
#include "raylib.h"
#include "gui.h"
#include "objects.h"
#include "mapgenerator.h"

// ===============================
//           CURSOR
// ===============================

void drawCursor (Texture2D cursorTexture){
    Vector2 mousePosition = GetMousePosition();

    DrawTexture(cursorTexture, mousePosition.x - cursorTexture.width / 2, mousePosition.y - cursorTexture.height / 2, WHITE);
}

// ===============================
//         TITLE SCREEN
// ===============================

void drawBackgroundTitle (Texture2D texture){
    Rectangle sourceRec = {0, 0, 600, 450};
    Rectangle destRec = {-100, 0, MAP_COLUMNS * 3 * TILE_SIZE + 100, MAP_LINES * 3 * TILE_SIZE};

    DrawTexturePro(texture, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
}

void checkButtonStart (GameScreen *screen){
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                Rectangle button = {310, 515, 300, 50};
                Vector2 mousePosition = GetMousePosition();

                if (CheckCollisionPointRec(mousePosition, button))
                    *screen = SCREEN_TUTORIAL;
    }
}

// ===============================
//         TUTORIAL SCREEN
// ===============================

void drawTutorialScreen (Texture2D *tileset, Texture2D *speechBaloon){
    Rectangle sourceM = {.x = calcTile(1), .y = calcTile (8), TILE_SIZE, TILE_SIZE};
    Rectangle destM = {.x = 200, .y= 300, 200, 200};
    DrawTexturePro(*tileset, sourceM, destM, (Vector2){0, 0}, 0.0f, WHITE);

    DrawTexture(*speechBaloon, 250, 150, WHITE);
}

// ===============================
//         DEATH SCREEN
// ===============================

void checkButtonRevive (GameScreen *screen, Player *player){
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                Rectangle button = {310, 515, 300, 50};
                Vector2 mousePosition = GetMousePosition();

                if (CheckCollisionPointRec(mousePosition, button)){
                    player->life = 1;
                    player->score = 0;
                    *screen = SCREEN_GAME;
                }
    }
}

// ===============================
//         INFO GUI
// ===============================

void drawScore (Player *player){
    char score[10];
    
    sprintf(score, "XP: %d", player->score);

    DrawText(score, 840, 130, 30, BLACK); // 830
}

void drawLevelIndicator (Level *level){
    char levelText[3];

    sprintf(levelText, "%d", level->level);

    DrawText(levelText, 30, 30, 30, WHITE);
}

// ===============================
//            STATS
// ===============================

void checkButtonLifePlus (Player *player, Stats *stats){
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                Rectangle button = {310, 315, 300, 50};
                Vector2 mousePosition = GetMousePosition();

                if (CheckCollisionPointRec(mousePosition, button)){
                    if (player->score >= 20 * stats->lifeStats){
                        player->score -= 20 * stats->lifeStats;
                        stats->lifeStats++;
                    }
                }
    }
}

void checkButtonAttackPlus (Player *player, Stats *stats){
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                Rectangle button = {310, 375, 300, 50};
                Vector2 mousePosition = GetMousePosition();

                if (CheckCollisionPointRec(mousePosition, button)){
                    if (player->score >= 20 * stats->attackStats){
                        player->score -= 20 * stats->attackStats;
                        stats->attackStats++;
                    }
                }
    }
}   

void drawLifePlusButton (Stats *stats, Texture2D tileset){
    char lifeText[30];

    sprintf(lifeText, "Aumentar Vida (-%d XP)", stats->lifeStats * 20);

    DrawRectangle(290, 315, 330, 50, BLACK);
    DrawText(lifeText, 340, 330, 20, WHITE);

    Rectangle source = {.x = calcTile(8), .y = calcTile (10), TILE_SIZE, TILE_SIZE};
    Rectangle dest = {300, 325, 32, 32};
    DrawTexturePro(tileset, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

void drawAttackPlusButton (Stats *stats, Texture2D tileset){
    char attackText[30];

    sprintf(attackText, "Aumentar Ataque (-%d XP)", stats->attackStats * 20);

    DrawRectangle(290, 375, 330, 50, BLACK);
    DrawText(attackText, 340, 390, 20, WHITE);

    Rectangle source = {.x = calcTile(9), .y = calcTile (9), TILE_SIZE, TILE_SIZE};
    Rectangle dest = {300, 385, 32, 32};
    DrawTexturePro(tileset, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

void drawStats (Stats *stats, Texture2D *tileset){
    char life[3], attack[3];

    Vector2 coordinates = {800, 80};
    // Life stats
    Rectangle sourceL = {.x = calcTile(8), .y = calcTile (10), TILE_SIZE, TILE_SIZE};
    Rectangle destL = {coordinates.x, coordinates.y, 32, 32};
    DrawTexturePro(*tileset, sourceL, destL, (Vector2){0, 0}, 0.0f, WHITE);

    sprintf(life, "%d", stats->lifeStats);
    DrawText(life, coordinates.x + 40, coordinates.y, 32, WHITE);

    //Attack Stats
    Rectangle sourceA = {.x = calcTile(9), .y = calcTile (9), TILE_SIZE, TILE_SIZE};
    Rectangle destA = {coordinates.x + 80, coordinates.y, 32, 32};
    DrawTexturePro(*tileset, sourceA, destA, (Vector2){0, 0}, 0.0f, WHITE);

    sprintf(attack, "%d", stats->attackStats);
    DrawText(attack, coordinates.x + 120, coordinates.y, 32, WHITE);
}