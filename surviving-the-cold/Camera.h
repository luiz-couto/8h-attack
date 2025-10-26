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
        int terrainPixelWidth = this->terrain->width * TILE_SIZE;
        if (possibleX >= 0 && (possibleX + WINDOW_WIDTH) <= terrainPixelWidth) {
            this->x = possibleX;
        }

        int possibleY = charPos.y - OFFSET_Y;
        int terrainPixelHeight = this->terrain->height * TILE_SIZE;
        if (possibleY >= 0 && (possibleY + WINDOW_HEIGHT) <= terrainPixelHeight) {
            this->y = possibleY;
        }

        this->terrain->drawTerrain(this->x/TILE_SIZE, this->y/TILE_SIZE);
        this->character->draw(this->x, this->y);
    }
};