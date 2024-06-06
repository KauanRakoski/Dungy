#include "raylib.h"
#ifndef RESOURCES_H
#define RESOURCES_H

void loadTextures (Texture2D *tileset, Texture2D *playerTexture, Texture2D *initBackground, Texture2D *speechBaloon);
void unloadTextures (Texture2D *tileset, Texture2D *playerTexture, Texture2D *initBackground, Texture2D *speechBaloon);

void loadSounds (Music *music, Sound *playerAttack, Sound *monsterAttack);
void unloadSounds (Music *music, Sound *playerAttack, Sound *monsterAttack);

#endif