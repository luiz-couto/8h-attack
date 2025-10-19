#include "GamesEngineeringBase.h"
#include "Debug.h"

#define CHARACTER_WIDTH 40
#define CHARACTER_HEIGHT 40

struct Position {
    int x,y;
};

enum LockMovement {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Character {
    private:
    GamesEngineeringBase::Window* canvas;
    Position position;
    int velocity = 6;
    LockMovement lockMove = LockMovement::NONE;

    public:
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

    void setVelocity(int newVelocity) {
        this->velocity = newVelocity;
    }

    void move(int xIncrement, int yIncrement) {
        this->position.x = this->position.x + xIncrement;
        this->position.y = this->position.y + yIncrement;
    }

    void reactToMovementKeys() {
        if (this->canvas->keyPressed('W') && this->lockMove != LockMovement::UP) {
            this->move(0, -(this->velocity));
        }
        if (this->canvas->keyPressed('A') && this->lockMove != LockMovement::LEFT) {
            this->move(-(this->velocity), 0);
        }
        if (this->canvas->keyPressed('D') && this->lockMove != LockMovement::RIGHT) {
            this->move(this->velocity, 0);
        }
        if (this->canvas->keyPressed('S') && this->lockMove != LockMovement::DOWN) {
            this->move(0, this->velocity);
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