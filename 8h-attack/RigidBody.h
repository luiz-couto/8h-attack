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
    TERRAIN_COLLISION,
    PROJECTILE_COLLISION,
    AOE_PLAYER_ATTACK,
    POWERUP_COLLISION
};

// RigidBody base class for all "physical" entities in the game
// It provides position, collision detection, distance calculation and drawing capabilities
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

    Position getCenterPosition() {
        Position centerPos;
        centerPos.x = this->position.x + this->getWidth() / 2;
        centerPos.y = this->position.y + this->getHeight() / 2;
        return centerPos;
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

    float getDistanceTo(RigidBody *rigidBody) {
        Position rigidBodyPos = rigidBody->getCenterPosition();
        Position thisCenterPos = this->getCenterPosition();
        float dx = thisCenterPos.x - rigidBodyPos.x;
        float dy = thisCenterPos.y - rigidBodyPos.y;
        return sqrt(dx * dx + dy * dy);
    }

    void draw(Position cameraPosition) {
        int drawPositionX = this->position.x - cameraPosition.x;
        int drawPositionY = this->position.y - cameraPosition.y;

        this->gameImage->drawImage(this->currentFrame, drawPositionX, drawPositionY);
    }

    bool isOnCameraView(Position cameraPosition) {
        int drawPositionX = this->position.x - cameraPosition.x;
        int drawPositionY = this->position.y - cameraPosition.y;

        if (drawPositionX + this->getWidth() < 0 || drawPositionX >= this->canvas->getWidth() ||
            drawPositionY + this->getHeight() < 0 || drawPositionY >= this->canvas->getHeight()) {
            return false;
        }
        return true;
    }

    void processCollision(COLLISION_KIND kind, RigidBody *rigidBody) {};
};