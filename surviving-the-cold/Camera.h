#pragma once

#include <iostream>
#include "Position.h"

#define OFFSET_X 512
#define OFFSET_Y 384

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define TILE_SIZE 32

class Camera {
    private:
    Position position;

    public:
    Camera(Position position) {
        this->position = position;
    }

    Position getPosition() {
        return this->position;
    }

    void update(Position playerPosition, int terrainPixelWidth, int terrainPixelHeight) {    
        int possibleX = playerPosition.x - OFFSET_X;
        if (possibleX >= 0 && (possibleX + WINDOW_WIDTH) <= terrainPixelWidth) {
            this->position.x = possibleX;
        }

        int possibleY = playerPosition.y - OFFSET_Y;
        if (possibleY >= 0 && (possibleY + WINDOW_HEIGHT) <= terrainPixelHeight) {
            this->position.y = possibleY;
        }
    }
};