#pragma once

#include "GamesEngineeringBase.h"
#include "Debug.h"

#define CHARACTER_WIDTH 48
#define CHARACTER_HEIGHT 48
#define CHARACTER_START_VELOCITY 6

struct Position {
    int x,y;
};

class Character {
    private:
    GamesEngineeringBase::Window* canvas;
    Position position;
    Position screenPosition;
    int velocity = CHARACTER_START_VELOCITY;
    
    public:
    Character(GamesEngineeringBase::Window *canvas) {
        this->canvas = canvas;

        Position pos;
        pos.x = 0;
        pos.y = 0;
        this->position = pos;
        this->screenPosition = pos;
    }

    Character(GamesEngineeringBase::Window *canvas, int x, int y) {
        this->canvas = canvas;

        Position pos;
        pos.x = x;
        pos.y = y;
        this->position = pos;
        this->screenPosition = pos;
    }
    
    void setPosition(int x, int y) {
        this->position.x = x;
        this->position.y = y;
    }

    Position getPosition() {
        return this->position;
    }

    void setVelocity(int newVelocity) {
        this->velocity = newVelocity;
    }

    void reactToMovementKeys(int boundaryWidth, int boundaryHeight) {
        if (this->canvas->keyPressed('W')) {
            this->position.y = max(this->position.y - this->velocity, 0);
        }
        if (this->canvas->keyPressed('A')) {
            this->position.x = max(this->position.x - this->velocity, 0);
        }
        if (this->canvas->keyPressed('D')) {
            this->position.x = min(this->position.x + this->velocity, boundaryWidth - CHARACTER_WIDTH);
        }
        if (this->canvas->keyPressed('S')) {
            this->position.y = min(this->position.y + this->velocity, boundaryHeight - CHARACTER_HEIGHT);
        }
    }
    
    void draw(int cameraX, int cameraY) {
        this->screenPosition.x = this->position.x - cameraX;
        this->screenPosition.y = this->position.y - cameraY;

        for (int i=this->screenPosition.x; i < this->screenPosition.x + CHARACTER_WIDTH; i++) {
            for (int j=this->screenPosition.y; j < this->screenPosition.y + CHARACTER_HEIGHT; j++) {
                if (i >=0 && i < this->canvas->getWidth() && j >=0 && j < this->canvas->getHeight()) {
                    canvas->draw(i, j, 255, 255, 255);
                }
            }
        }
    }

};