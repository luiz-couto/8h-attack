#pragma once

#include <iostream>
#include "Character.h"
#include "Terrain.h"

#define OFFSET_X 512
#define OFFSET_Y 384

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

class Camera {
    private:
    Character *character;
    Terrain *terrain;
    int x, y;

    public:
    Camera(Character *character, Terrain *terrain) {
        this->character = character;
        this->terrain = terrain;

        Position charPos = this->character->getPosition();
        this->x = charPos.x;
        this->y = charPos.y;
    }

    void drawNextFrame() {
        Position charPos = this->character->getPosition();
        
        int possibleX = charPos.x - OFFSET_X;
        int possibleM = possibleX / TILE_SIZE;
        if (possibleM >= 0 && possibleM + (WINDOW_WIDTH / TILE_SIZE) <= this->terrain->width) {
            this->x = possibleX;
            this->character->lockedX = true;
        } else {
            this->character->lockedX = false;
        }

        int possibleY = charPos.y - OFFSET_Y;
        int possibleN = possibleY / TILE_SIZE;
        if (possibleN >= 0 && possibleN + (WINDOW_HEIGHT / TILE_SIZE) <= this->terrain->height) {
            this->y = possibleY;
            this->character->lockedY = true;
        } else {
            this->character->lockedY = false;
        }

        this->terrain->drawTerrain(this->x/TILE_SIZE, this->y/TILE_SIZE);
    }
};