#pragma once

#include <iostream>
#include "Character.h"
#include "Terrain.h"

#define OFFSET_X 512
#define OFFSET_Y 384

class Camera {
    private:
    Character *character;
    Terrain *terrain;
    int lastX, lastY;

    public:
    Camera(Character *character, Terrain *terrain) {
        this->character = character;
        this->terrain = terrain;

        Position charPos = this->character->getPosition();
        this->lastX = (charPos.x - OFFSET_X)/32;
        this->lastY = (charPos.y - OFFSET_Y)/32;
    }

    void drawNextFrame() {
        Position charPos = this->character->getPosition();
        std::cout << "Character position: (" << charPos.x << ", " << charPos.y << ")" << std::endl;

        bool isOutOfBounds = this->terrain->isOutOfBounds((charPos.x - OFFSET_X)/32, (charPos.y - OFFSET_Y)/32);
        std::cout << "Is out of bounds: " << isOutOfBounds << std::endl;
        if (isOutOfBounds) {
            //this->character->locked = false;
            this->terrain->drawTerrain(this->lastX, this->lastY);
            return;
        }
        //this->character
        this->terrain->drawTerrain((charPos.x - OFFSET_X)/32, (charPos.y - OFFSET_Y)/32);
        std::cout << "Drawing terrain at: (" << (charPos.x - OFFSET_X)/32 << ", " << (charPos.y - OFFSET_Y)/32 << ")" << std::endl;
        this->lastX = (charPos.x - OFFSET_X)/32;
        this->lastY = (charPos.y - OFFSET_Y)/32;
    }
};