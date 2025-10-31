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

    Position getPosition() {
        return this->position;
    }

    bool detectCollision(RigidBody *rigidBody) {
        // Rigid Body 1 properties
        float radius1 = this->currentFrame->height / 2.0f;
        float center1X = this->position.x + (this->currentFrame->width / 2.0f);
        float center1Y = this->position.y + radius1;

        // Rigid Body 2 properties
        float radius2 = rigidBody->currentFrame->height / 2.0f;
        float center2X = rigidBody->position.x + (rigidBody->currentFrame->width / 2.0f);
        float center2Y = rigidBody->position.y + radius2;

        // Calculate the distance between the two centers
        float dx = center2X - center1X;
        float dy = center2Y - center1Y;
        float distanceBetweenCenters = sqrt(pow(dx, 2) + pow(dy, 2));

        // Check for collision
        if (distanceBetweenCenters <= (radius1 + radius2) - COLLISION_THRESHOLD) {
            return true;
        }

        return false;
    }

    void draw(Position cameraPosition) {
        int drawPositionX = this->position.x - cameraPosition.x;
        int drawPositionY = this->position.y - cameraPosition.y;

        this->gameImage->drawImage(this->currentFrame, drawPositionX, drawPositionY);
    }
};