#include "raylib.h"
#include "resources.h"

void loadTextures (Texture2D *tileset, Texture2D *playerTexture, Texture2D *initBackground, Texture2D *speechBaloon){
    *tileset = LoadTexture("./resources/tilemap.png");
    *playerTexture = LoadTexture("./resources/player_tiles.png");
    *initBackground = LoadTexture("./resources/title_background.png");
    *speechBaloon = LoadTexture("./resources/speech-bubble.png");
}

void unloadTextures (Texture2D *tileset, Texture2D *playerTexture, Texture2D *initBackground, Texture2D *speechBaloon){
    UnloadTexture(*tileset);
    UnloadTexture(*playerTexture);
    UnloadTexture(*initBackground);
    UnloadTexture(*speechBaloon);
}

void loadSounds (Music *music, Sound *playerAttack, Sound *monsterAttack){
    *monsterAttack = LoadSound("./resources/monster_attack.mp3");
    *playerAttack = LoadSound("./resources/sword.mp3");

    *music = LoadMusicStream("./resources/soundtrack.mp3");
}

void unloadSounds (Music *music, Sound *playerAttack, Sound *monsterAttack){
    UnloadMusicStream(*music);
    UnloadSound(*playerAttack);
    UnloadSound(*monsterAttack);
}

