#include "raylib.h"
#ifndef OBJECTS_H
#define OBJECTS_H

#define PLAYER_TILE_SIZE 64
#define PLAYER_ACTUAL_SIZE 80

#define MAX_ENEMIES 6
#define CHASE_DISTANCE 150
#define ENEMY_BASE_SPEED 200.0f

#define POTION_SIZE 32

typedef struct Player {
    Vector2 position;
    Texture2D texture;
    int animLine;
    int currentFrame;
    int frameCount;
    float frameTime;
    float frameCounter;
    int frameSize;

    int attackStats;
    int lifeStats;
    float speed;
    float life;
    int score;
    int isAttacking[4]; // esquerda, cima, direita, baixo
    bool isBeingAttacked;
} Player;

typedef struct Colisor {
    Rectangle rect;
    char blockedSide;
} Colisor;

typedef struct Enemy {
    Vector2 position;
    Texture2D *texture;
    Vector2 direction;
    int life;
    float speed;
    bool active;
    bool canMove;
    bool isBeingAttacked;
} Enemy;

typedef struct Potion {
    Vector2 position;
    int type;
    float potionTimer;
    float potionInterval;
    Texture2D potionTexture;
    Vector2 tilePosition;
    bool active;
} Potion;

typedef struct Level {
    int level;
    float levelTimer;
    float levelInterval;
} Level;

typedef struct Stats {
    int lifeStats;
    int attackStats;
} Stats;

void updatePlayer (Player *player, float deltaTime,  Sound *playerAttack);
void drawPlayer (Player *player);
void drawLifeBar(Player *player);
void CheckColisors(Colisor *colisors, int colisors_size, Player *player, int deltaTime);
void initEnemies (Enemy *enemies, Texture2D *enemyTexture);
void updateEnemies (Enemy *enemies, Level *level, float deltaTime);
void drawEnemies(Enemy *enemies);
void moveEnemies (Enemy *enemies, Player *player, Level *level, float deltaTime);
void checkEnemyAttack(Player *player, Enemy *enemies, Level *level, Stats *stats, Sound damageSound);
void checkEnemyColisors (Colisor *colisors, int colisors_size, Enemy *enemies, int deltaTime);
void generatePotion (Potion *potion, float deltaTime);
void drawPotion (Potion *potion);
void checkPotionCollection (Potion *potion, Player *player);
void updateLevel (Level *level, float deltaTime);

#endif