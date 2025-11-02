#pragma once

#include <string>
#include "Position.h"
#include "GamesEngineeringBase.h"
#include "GameImage.h"

#define COLLISION_THRESHOLD 20

enum COLLISION_KIND {
    NPC_COLLISION,
    PLAYER_COLLISION,
    OBJECT_COLLISION,
    TERRAIN_COLLISION
};

class RigidBody {
    protected:
    GamesEngineeringBase::Window* canvas;
    GameImage* gameImage;
    Position position;
    GamesEngineeringBase::Image *currentFrame;

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

    RigidBody(GamesEngineeringBase::Window* canvas, int x, int y, GamesEngineeringBase::Image *currentFrame) {
        this->canvas = canvas;
        this->gameImage = new GameImage(canvas);

        Position pos;
        pos.x = x;
        pos.y = y;
        this->position = pos;

        this->currentFrame = currentFrame;
    }

    Position getPosition() {
        return this->position;
    }

    int getCollisionWidth() {
        return this->currentFrame->width - COLLISION_THRESHOLD;
    }

    int getCollisionHeight() {
        return this->currentFrame->height - COLLISION_THRESHOLD;
    }

    int getWidth() {
        return this->currentFrame->width;
    }

    int getHeight() {
        return this->currentFrame->height;
    }

    bool detectCollision(RigidBody *rigidBody) {
        Position rigidBodyPos = rigidBody->getPosition();
        if(this->position.x < rigidBodyPos.x + rigidBody->getCollisionWidth() &&
            this->position.x + this->getCollisionWidth() > rigidBodyPos.x &&
            this->position.y < rigidBodyPos.y + rigidBody->getCollisionHeight() &&
            this->position.y + this->getCollisionHeight() > rigidBodyPos.y) 
        {
            return true;
        }
        return false;
    }

    void draw(Position cameraPosition) {
        int drawPositionX = this->position.x - cameraPosition.x;
        int drawPositionY = this->position.y - cameraPosition.y;

        this->gameImage->drawImage(this->currentFrame, drawPositionX, drawPositionY);
    }

    void processCollision(COLLISION_KIND kind, RigidBody *rigidBody) {};
};