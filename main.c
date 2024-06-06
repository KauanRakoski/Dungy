#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include "mapgenerator.h"
#include "objects.h"
#include "gui.h"
#include "resources.h"

int map_width = MAP_COLUMNS * 3 * TILE_SIZE;
int map_height = MAP_LINES * 3 * TILE_SIZE;

int main (void){
    // Inicializamos o aparelho de áudio e carregamos os arquivos de áudio
    Music soundtrack;
    Sound playerAttack, monsterAttack;
    
    InitAudioDevice();
    loadSounds(&soundtrack, &playerAttack, &monsterAttack);    

    // Abrimos o arquivo que contém dados do mapa
    FILE *mapa = fopen("./resources/map.txt", "r");
    if (mapa == NULL) return 1;

    // Inicializamos a janela e com o contexto de OpenGL carregamos as texturas
    InitWindow(map_width, map_height, "Dungy");

    Texture2D playerTexture, tileset, bg, speechBaloon;
    loadTextures(&tileset, &playerTexture, &bg, &speechBaloon);

    // Inicializamos as variáveis de jogo
    GameScreen screen = SCREEN_TITLE;

    Player player = {.position = (Vector2){300, 200}, 
        .speed = 200.0f, 
        .life = 1,
        .score = 0,
        .isAttacking = {0, 0, 0, 0},
        .isBeingAttacked = false,

        .texture = playerTexture,
        .currentFrame = 0,
        .frameCount = 8,
        .frameTime = 0.05,
        .frameSize = 64,
        .frameCounter = 0.0f
        };
    
    Enemy enemies[MAX_ENEMIES];
    initEnemies(enemies, &tileset);
    
    Colisor colisors[] = {
        // margem esquerda do mapa
        {.rect = {20, -50, 10, map_height + 100}, .blockedSide = 'r'},
        // margem superior do mapa
        {.rect = {-20, -20, map_width + 100, 10}, .blockedSide = 'd'},
        // margem inferior do mapa
        {.rect = {0, map_height, map_width, 10}, .blockedSide = 'u'},
        // margem direita do mapa
        {.rect = {map_width + 10, -20, 10, map_height + 20}, .blockedSide = 'l'},
        // muros esquerdos superiores
        {.rect = {5, -20, calcTilesSize(9) - 40, 8 * TILE_SIZE}, .blockedSide = 'd'},
        {.rect = {calcTilesSize(8), -30,  calcTilesSize(1) + 5, calcTilesSize(3) - 40}, .blockedSide = 'r'}
    };

    int num_colisors = sizeof(colisors) / sizeof(colisors[0]);

    Potion potions[] = {
        {.type = 0, .potionTimer = 0.0f, .potionInterval = 15.0f, .potionTexture = tileset, .tilePosition = (Vector2){8, 10}, .active = false}
    };

    Level level = {.level=1, .levelInterval = 10, .levelTimer = 0.0f};
    Stats stats = {.lifeStats = 1, .attackStats = 1};

    Texture2D cursorUI = LoadTexture("./resources/cursor_p.png");

    // Renderizamos o mapa de fundo (para evitar redesenhar todas tiles a cada atualização)
    RenderTexture2D mapTexture = LoadRenderTexture(MAP_WIDTH * TILE_SIZE * 3, MAP_HEIGHT * TILE_SIZE * 3);
    BeginTextureMode(mapTexture);
    ClearBackground(BLANK);
    FiletoMap(mapa, tileset);
    EndTextureMode();

    // Criamos a textura pra a tela de fundo da tela de início
    RenderTexture2D bgstart = LoadRenderTexture(MAP_WIDTH * TILE_SIZE * 3, MAP_HEIGHT * TILE_SIZE * 3);
    BeginTextureMode(bgstart);
    ClearBackground(BLANK);
    drawBackgroundTitle(bg);
    EndTextureMode();

    
    HideCursor();
    SetExitKey(0);

    float deltaTime;
    SetTargetFPS(60);

    PlayMusicStream(soundtrack);
    SetMusicVolume(soundtrack, 0.2f);

    while(!WindowShouldClose()){
        deltaTime = GetFrameTime();

        if (screen == SCREEN_TITLE){
            checkButtonStart(&screen);
        }
        if (screen == SCREEN_GAME){
            UpdateMusicStream(soundtrack);
            updatePlayer(&player, deltaTime, &playerAttack);
            CheckColisors(colisors, num_colisors, &player, deltaTime);

            generatePotion(&potions[0], deltaTime);
            checkPotionCollection (&potions[0], &player);

            updateEnemies(enemies, &level, deltaTime);
            moveEnemies(enemies, &player, &level, deltaTime);
            
            checkEnemyAttack(&player, enemies, &level, &stats, monsterAttack);
            checkEnemyColisors (colisors, num_colisors, enemies, deltaTime);

            updateLevel(&level, deltaTime);

            if(IsKeyPressed(KEY_ESCAPE)){
                screen = SCREEN_PAUSE;
            }
        }
        if (screen == SCREEN_TUTORIAL){
             if(IsKeyPressed(KEY_ENTER)){
                screen = SCREEN_GAME;
            }
        }
        if (screen == SCREEN_DEATH){
            checkButtonRevive(&screen, &player);
        }
        if (screen == SCREEN_PAUSE){
            if (IsKeyReleased(KEY_ENTER))
                screen = SCREEN_GAME;
            
            checkButtonLifePlus (&player, &stats);
            checkButtonAttackPlus (&player, &stats);
        }

        if (player.life <= 0) screen = SCREEN_DEATH;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (screen == SCREEN_TITLE){
            DrawTexturePro(bgstart.texture, 
            (Rectangle){ 0, 0, bgstart.texture.width, -bgstart.texture.height },
            (Rectangle){ 0, 0, mapTexture.texture.width, mapTexture.texture.height },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);            

            DrawText("Dungy", 310, 330, 100, BLACK);
            DrawRectangle(310, 515, 300, 50, BLACK);
            DrawText("Iniciar", 430, 530, 20, WHITE);

            drawCursor(cursorUI);
        }
        if (screen == SCREEN_GAME || screen == SCREEN_PAUSE){
            drawMap(mapTexture);

            drawLevelIndicator(&level);

            drawPotion(&potions[0]);
            
            drawPlayer(&player);
            drawEnemies(enemies);

            drawLifeBar(&player);
            drawStats(&stats, &tileset);
            drawScore(&player);

            if (screen == SCREEN_PAUSE){
                DrawRectangle(130, 100, 700, 500, (Color){26, 33, 48, 240});
                DrawText("Inventário", 330, 150, 50, WHITE);

                drawLifePlusButton(&stats, tileset);
                drawAttackPlusButton(&stats, tileset);

                drawCursor(cursorUI);
            }
        }
        if (screen == SCREEN_TUTORIAL){
            ClearBackground((Color){167, 230, 255});
            drawTutorialScreen(&tileset, &speechBaloon);
        }
         if (screen == SCREEN_DEATH){
            DrawTexturePro(bgstart.texture, 
            (Rectangle){ 0, 0, bgstart.texture.width, -bgstart.texture.height },
            (Rectangle){ 0, 0, mapTexture.texture.width, mapTexture.texture.height },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);

            DrawText("Você morreu!", 260, 300, 60, RED);
            DrawRectangle(310, 515, 300, 50, BLACK);
            DrawText("Reiniciar", 420, 530, 20, WHITE);
            drawCursor(cursorUI);
        }
      
        EndDrawing();
    }

    
    fclose(mapa);

    UnloadTexture(tileset);
    UnloadTexture(playerTexture);
    UnloadTexture(cursorUI);
    UnloadRenderTexture(mapTexture);

    unloadSounds(&soundtrack, &playerAttack, &monsterAttack);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}