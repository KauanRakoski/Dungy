#include "raylib.h"
#include "objects.h"
#include "raymath.h"
#include "mapgenerator.h"

// ===============================
//       FUNÇÕES AUXILIARES
// ===============================

int checkAttacking (int isAttacking[4]){
    return (isAttacking[0] || isAttacking[1] || isAttacking[2] || isAttacking[3]);
}

int calcAttackTileLine (int isAttacking[4]){
    if (isAttacking[1]) return 55;
    if (isAttacking[0]) return 55 + 80 + 111;
    if (isAttacking[3]) return 55 + 2* (80+111);
    if (isAttacking[2]) return 55 + 3 * (80+111);

    return -1;
}

int calcAttackTileColumn (int FrameCount){
    if (FrameCount == 0) return 55;
    if (FrameCount == 1) return 55 + 80 + 110;
    if (FrameCount == 2) return 55 + 2* (80+110);
    if (FrameCount == 3) return 55 + 3* (80+111);
    if (FrameCount == 4) return 55 + 4* (80+110);
    if (FrameCount == 5) return 55 + 5* (80+110);

    return -1;
}

// ===============================
//           JOGADOR
// ===============================

void updatePlayer (Player *player, float deltaTime, Sound *playerAttack){
    bool moving = false;

    if (IsKeyDown(KEY_A)){
        player->position.x -= player->speed * deltaTime;
        
        player->frameCount = 8;
        moving = true;
        player->animLine = 9;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            PlaySound(*playerAttack);
            player->isAttacking[0] = 1;
            player->currentFrame = 0;
            player->frameCounter = 0;
        }
    }
    if (IsKeyDown(KEY_D)){
        player->position.x += player->speed * deltaTime;

        moving = true;
        player->frameCount = 8;
        player->animLine = 11;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            PlaySound(*playerAttack);
            player->isAttacking[2] = 1;
            player->currentFrame = 0;
            player->frameCounter = 0;
        }
    }
    if (IsKeyDown(KEY_W)){
        player->position.y -= player->speed * deltaTime;

        moving = true;
        player->frameCount = 8;
        player->animLine = 8;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            PlaySound(*playerAttack);
            player->isAttacking[1] = 1;
            player->currentFrame = 0;
            player->frameCounter = 0;
        }
    }
    if (IsKeyDown(KEY_S)){
        player->position.y += player->speed * deltaTime;

        moving = true;
        player->frameCount = 8;
        player->animLine = 10;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            PlaySound(*playerAttack);
            player->isAttacking[3] = 1;
            player->currentFrame = 0;
            player->frameCounter = 0;
        }
    }

    // Atualização da animação
    if (checkAttacking(player->isAttacking) != 0){
        player->frameCounter += deltaTime;

        if (player->frameCounter >= 0.07){
            player->frameCounter = 0.0f;
            player->currentFrame++;

            if (player->currentFrame >= 6){
                player->currentFrame = 0;

                player->isAttacking[0] = 0;
                player->isAttacking[1] = 0;
                player->isAttacking[2] = 0;
                player->isAttacking[3] = 0;
                
            }
        }
    }
    else if (moving){
        player->frameCounter += deltaTime;

        if (player->frameCounter >= player->frameTime) {
            player->frameCounter = 0.0f;
            player->currentFrame++;

            if (player->currentFrame >= player->frameCount) {
                player->currentFrame = 0;
            }   
        }
    }
    else if (!moving) {
        player->animLine = 2;
        player->currentFrame = 0;
    } 
}

void drawPlayer (Player *player){
    Rectangle sourceRec, destRec;

    if (checkAttacking(player->isAttacking)){
        int frameX = calcAttackTileColumn(player->currentFrame);
        int frameY = 21 * PLAYER_TILE_SIZE + calcAttackTileLine(player->isAttacking);

        sourceRec = (Rectangle){.x = frameX, .y = frameY, .width= 80, .height= 80};
        destRec = (Rectangle){.x = player->position.x - 10, .y = player->position.y - 10, .width = 90, .height = 90};
    } else {
        int frameX = player->currentFrame * PLAYER_TILE_SIZE;
        int frameY = player->animLine * PLAYER_TILE_SIZE;

        sourceRec = (Rectangle){.x = frameX, .y = frameY, .width=player->frameSize, .height=player->frameSize};
        destRec = (Rectangle){.x = player->position.x, .y = player->position.y, .width = PLAYER_ACTUAL_SIZE, .height = PLAYER_ACTUAL_SIZE};
    }

    DrawEllipse(player->position.x + PLAYER_ACTUAL_SIZE / 2, player->position.y + 75, 20, 10, (Color){0, 0, 0, 150});
    DrawTexturePro(player->texture, sourceRec, destRec, (Vector2){0, 0}, 0.0f, player->isBeingAttacked ? RED : WHITE);
}

// ==================
//   BARRA DE VIDA
// ==================

void drawLifeBar(Player *player){
    DrawRectangle(740, 30, 200, 30, BLACK);
    DrawRectangle(740, 30, player->life * 200, 30, RED);
}

// ===============================
//           COLISORES
// ===============================

void CheckColisors(Colisor *colisors, int colisors_size, Player *player, int deltaTime){
    bool canMove = true;

    Rectangle playerRect = {.x = player->position.x + player->speed * deltaTime, .y = player->position.y + player->speed * deltaTime, 
    .width = PLAYER_ACTUAL_SIZE, .height = PLAYER_ACTUAL_SIZE};

    for (int i = 0; i < colisors_size; i++){
        
        Colisor *colisor = colisors + i;

        if (CheckCollisionRecs(playerRect, colisor->rect)){
            canMove = false;

            if (colisor->blockedSide == 'r') player->position.x += 0.5;
            if (colisor->blockedSide == 'l') player->position.x -= 0.5;
            if (colisor->blockedSide == 'd') player->position.y += 0.5;
            if (colisor->blockedSide == 'u') player->position.y -= 0.5;
        }
        
        if (!canMove){    
            player->speed = 0;
        } else {
            player->speed = 200.0f;
        }
    }
}

void checkEnemyColisors (Colisor *colisors, int colisors_size, Enemy *enemies, int deltaTime){

    for (int i = 0; i < MAX_ENEMIES; i++){
        Enemy *enemy = enemies + i;
        enemy->canMove = true;

        Rectangle playerRect = {.x = enemy->position.x + enemy->speed * deltaTime, .y = enemy->position.y + enemy->speed * deltaTime, 
        .width = 50, .height = 50};

        for (int i = 0; i < colisors_size; i++){
        
        Colisor *colisor = colisors + i;

        if (CheckCollisionRecs(playerRect, colisor->rect)){
            enemy->canMove = false;

            if (colisor->blockedSide == 'r') enemy->direction = (Vector2){1, 0};
            if (colisor->blockedSide == 'l') enemy->direction = (Vector2){-1, 0};
            if (colisor->blockedSide == 'd') enemy->direction = (Vector2){0, 1};
            if (colisor->blockedSide == 'u') enemy->direction = (Vector2){0, -1};
        }
        
        if (!enemy->canMove){    
            enemy->speed = 0;
        } else {
            enemy->speed = ENEMY_BASE_SPEED;
        }
    }
    }
}

// ===============================
//           INIMIGOS
// ===============================

void initEnemies (Enemy *enemies, Texture2D *enemyTexture){
    for (int i = 0; i < MAX_ENEMIES; i++){
        Enemy *enemy = enemies + i;

        enemy->position = (Vector2){GetRandomValue(50, 910), GetRandomValue(50, 700)};
        enemy->texture = enemyTexture;
        enemy->direction = (Vector2){2, 2};
        enemy->canMove = true;
        enemy->active = false;
    }
}

float spawnTimer = 0.0f;
float spawnInterval = 2.0f;

void updateEnemies (Enemy *enemies, Level *level, float deltaTime){
    spawnTimer += deltaTime;
    
    if (spawnTimer >= spawnInterval){
        spawnTimer = 0.0f;

         for (int i = 0; i < MAX_ENEMIES; i++) {
            Enemy *enemy = enemies + i;

            // Lógica de spawn
            if (!enemy->active) {
                enemy->position = (Vector2){GetRandomValue(50, 910), GetRandomValue(50, 700)};
                enemy->active = true;
                enemy->life = level->level;
                break;
            }
        }
    }
}

void moveEnemies (Enemy *enemies, Player *player, Level *level, float deltaTime){
    // Lógica de movimento

    for (int i = 0; i < MAX_ENEMIES; i++){
        Enemy *enemy = enemies + i;

        float distanceToPlayer = Vector2Distance(player->position, enemy->position);
        Vector2 playerCenter = {.x = player->position.x + PLAYER_ACTUAL_SIZE /3.0, .y = player->position.y + PLAYER_ACTUAL_SIZE /3.0};

        if (distanceToPlayer < CHASE_DISTANCE){
            enemy->direction = Vector2Normalize(Vector2Subtract(playerCenter, enemy->position));
        } else {
            if ((enemy->position.x == 2 && enemy->position.y == 2) || GetRandomValue(0, 100) < 5){
                int position = GetRandomValue(0, 3);

                switch (position){
                    case 0: enemy->direction = (Vector2){1, 0}; break; // direita
                    case 1: enemy->direction = (Vector2){-1, 0}; break; // esquerda
                    case 2: enemy->direction = (Vector2){0, -1}; break; // cima
                    case 3: enemy->direction = (Vector2){0, 1}; break; // baixo
                }
            }
        }

        enemy->position.x += enemy->direction.x * ENEMY_BASE_SPEED * level->level/2.0 * deltaTime;
        enemy->position.y += enemy->direction.y * ENEMY_BASE_SPEED * level->level /2.0 * deltaTime;
    }

}

void drawEnemies(Enemy *enemies){
    for (int i = 0; i < MAX_ENEMIES; i++){
        Enemy *enemy = enemies + i;
        bool isBeingAttacked = enemy->isBeingAttacked;
        
        Rectangle sourceRec = {.x = calcTile(2), .y = calcTile(11), TILE_SIZE, TILE_SIZE};
        Rectangle destRec = {.x = enemy->position.x, .y = enemy->position.y, 50, 50};
        
        if (enemy->active)
            DrawTexturePro(*enemy->texture, sourceRec, destRec, (Vector2){0,0}, 0.0f, isBeingAttacked? RED : WHITE);
    }
}

void checkEnemyAttack(Player *player, Enemy *enemies, Level *level, Stats *stats, Sound damageSound){
    player->isBeingAttacked = false;

    for (int i = 0; i < MAX_ENEMIES; i++){
        Enemy *enemy = enemies + i;
        enemy->isBeingAttacked = false;

        Rectangle playerHitbox = {.x = player->position.x, .y = player->position.y, 
        .width = PLAYER_ACTUAL_SIZE, .height = PLAYER_ACTUAL_SIZE};

        Rectangle enemyHitbox = {.x = enemy->position.x, .y = enemy->position.y,
        .width = 80, .height = 80};

        if (CheckCollisionRecs (playerHitbox, enemyHitbox) && enemy->active){
            player->life -= 0.01 * (stats->lifeStats < level->level ? (level->level - stats->lifeStats) : 1); // * level->level;
            player->isBeingAttacked = true;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                PlaySound(damageSound);
                enemy->isBeingAttacked = true;
                enemy->life -= stats->attackStats;
                
                if (enemy->life <= 0){
                    enemy->active = false;
                    player->score += 10;
                }
            }
        }
    }
}

// ===============================
//             POÇÕES
// ===============================

void generatePotion (Potion *potion, float deltaTime){
    potion->potionTimer += deltaTime;

    if (potion->potionTimer >= potion->potionInterval){
        if (!potion->active){
            potion->position = (Vector2){GetRandomValue(50, 800), GetRandomValue(70, 600)};
            potion->active = true;
        }
    }
}

void drawPotion (Potion *potion){
    if (potion->active){
        Rectangle potionTile =  {calcTile(potion->tilePosition.x), calcTile(potion->tilePosition.y), TILE_SIZE, TILE_SIZE};
        Rectangle potionRec = {potion->position.x, potion->position.y, POTION_SIZE, POTION_SIZE};
        DrawTexturePro (potion->potionTexture, potionTile, potionRec, (Vector2){0, 0}, 0.0f, WHITE);
    }
}

void checkPotionCollection (Potion *potion, Player *player){
    Rectangle potionRec = {potion->position.x, potion->position.y, POTION_SIZE, POTION_SIZE};
    Rectangle playerRec = {.x = player->position.x, .y = player->position.y, 
        .width = PLAYER_ACTUAL_SIZE, .height = PLAYER_ACTUAL_SIZE};

    if (CheckCollisionRecs(potionRec, playerRec) && potion->active == true){
        potion->active = false;
        potion->potionTimer = 0.0f;

        if (potion->type == 0) player->life = 1;
    }
}

// ===============================
//             NÍVEL
// ===============================

void updateLevel (Level *level, float deltaTime){
    level->levelTimer += deltaTime;

    if (level->levelTimer >= level->levelInterval){
        level->level++;
        // Cada vez, demora mais para passar de nível
        level->levelInterval += 30;
    }
}