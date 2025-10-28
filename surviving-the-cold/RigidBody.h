#pragma once

#include <string>
#include "Position.h"
#include "GamesEngineeringBase.h"
#include "GameImage.h"

#define COLLISION_THRESHOLD 2

class RigidBody {
    protected:
    GamesEngineeringBase::Window* canvas;
    GameImage* gameImage;
    Position position;

    GamesEngineeringBase::Timer timer = GamesEngineeringBase::Timer();
    float timeElapsed = 0.0f;

    public:
    RigidBody(GamesEngineeringBase::Window* canvas, int x, int y) {
        this->canvas = canvas;
        this->gameImage = new GameImage(canvas);

        Position pos;
        pos.x = x;
        pos.y = y;
        this->position = pos;
    }

    Position getPosition() {
        return this->position;
    }

    bool detectCollision(RigidBody *rigidBody) {
        int dx = rigidBody->position.x - this->position.x;
        int dy = rigidBody->position.y - this->position.y;

        float distance = sqrt(pow(dx, 2) +  pow(dy, 2));
        if (distance <= COLLISION_THRESHOLD) {
            return true;
        }

        return false;
    }

    virtual void draw(int cameraX, int cameraY) {};
};