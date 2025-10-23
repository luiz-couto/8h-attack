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
    int velocity = CHARACTER_START_VELOCITY;
    
    public:
    bool locked = false;
    Character(GamesEngineeringBase::Window *canvas) {
        this->canvas = canvas;

        Position pos;
        pos.x = 0;
        pos.y = 0;
        this->position = pos;
    }

    Character(GamesEngineeringBase::Window *canvas, int x, int y) {
        this->canvas = canvas;

        Position pos;
        pos.x = x;
        pos.y = y;
        this->position = pos;
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

    void reactToMovementKeys() {
        if (this->locked == true) {
            return;
        }
        //std::cout << "Reacting to movement keys" << std::endl;
        if (this->canvas->keyPressed('W')) {
            this->position.y = max(this->position.y - this->velocity, 0);
        }
        if (this->canvas->keyPressed('A')) {
            this->position.x = max(this->position.x - this->velocity, 0);
        }
        if (this->canvas->keyPressed('D')) {
            this->position.x = min(this->position.x + this->velocity, this->canvas->getWidth() - CHARACTER_WIDTH);
        }
        if (this->canvas->keyPressed('S')) {
            this->position.y = min(this->position.y + this->velocity, this->canvas->getHeight() - CHARACTER_HEIGHT);
        }
    }
    
    void draw() {
        for (int i=this->position.x; i < this->position.x + CHARACTER_WIDTH; i++) {
            for (int j=this->position.y; j < this->position.y + CHARACTER_HEIGHT; j++) {
                canvas->draw(i, j, 255, 255, 255);
            }
        }
    }

};