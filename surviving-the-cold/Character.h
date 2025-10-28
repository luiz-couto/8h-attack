#pragma once

#include <string>
#include "GamesEngineeringBase.h"
#include "GameImage.h"
#include "Position.h"
#include "Debug.h"
#include "RigidBody.h"

#define CHARACTER_WIDTH 96
#define CHARACTER_HEIGHT 96
#define CHARACTER_START_VELOCITY 7
#define LOADING_FRAME 4
#define IDLE_FRAME_TIME 0.3f

struct Rotation {
    GamesEngineeringBase::Image *south[5];
    GamesEngineeringBase::Image *north[5];
    GamesEngineeringBase::Image *east[5];
    GamesEngineeringBase::Image *west[5];
};

class Character : public RigidBody {
    private:
    Position screenPosition;
    int velocity = CHARACTER_START_VELOCITY;

    std::string name;
    Rotation rotationImages;
    GamesEngineeringBase::Image *current;
    char lastKeyPressed = 'S';
    int frameCount = 0;
    int loadingFrame = 0;

    public:
    Character(GamesEngineeringBase::Window *canvas, std::string name, int x, int y) : RigidBody(canvas, x, y) {
        this->name = name;
        this->screenPosition = this->position;

        Rotation rotationImages;
        loadAnimationFrames(rotationImages.south, "south");
        loadAnimationFrames(rotationImages.north, "north");
        loadAnimationFrames(rotationImages.west, "west");
        loadAnimationFrames(rotationImages.east, "east");

        this->rotationImages = rotationImages;
        this->current = this->rotationImages.south[0];
    }

    void loadAnimationFrames(GamesEngineeringBase::Image *group[5], std::string direction) {
        for (int i=0; i<5; i++) {
            GamesEngineeringBase::Image *frame = new GamesEngineeringBase::Image();
            frame->load("assets/characters/" + this->name + "/" + direction + "/" + std::to_string(i) + ".png");
            group[i] = frame;
        }
    }

    void selectNextFrame(char keyPressed, GamesEngineeringBase::Image *group[5]) {
        this->timeElapsed = 0.0f;
        if (this->lastKeyPressed == keyPressed) {
            if (this->loadingFrame < LOADING_FRAME && this->frameCount != 0) {
                this->loadingFrame++;
                return;
            }
            this->frameCount++;
            if (this->frameCount == 5) {
                this->frameCount = 1;
            }
        } else {
            this->frameCount = 0;
        }

        this->lastKeyPressed = keyPressed;
        this->current = group[this->frameCount];
        this->loadingFrame = 0;
    }

    void reactToMovementKeys(int boundaryWidth, int boundaryHeight) {
        if (this->canvas->keyPressed('A')) {
            this->position.x = max(this->position.x - this->velocity, 0);
            selectNextFrame('A', this->rotationImages.west);
            return;
        }
        if (this->canvas->keyPressed('D')) {
            this->position.x = min(this->position.x + this->velocity, boundaryWidth - CHARACTER_WIDTH);
            selectNextFrame('D', this->rotationImages.east);
            return;
        }
        if (this->canvas->keyPressed('W')) {
            this->position.y = max(this->position.y - this->velocity, 0);
            selectNextFrame('W', this->rotationImages.north);
            return;
        }
        if (this->canvas->keyPressed('S')) {
            this->position.y = min(this->position.y + this->velocity, boundaryHeight - CHARACTER_HEIGHT);
            selectNextFrame('S', this->rotationImages.south);
            return;
        }

        this->timeElapsed += this->timer.dt();
        if (timeElapsed > IDLE_FRAME_TIME) {
            this->frameCount = 0;
            switch (this->lastKeyPressed) {
                case 'A':
                    this->current = this->rotationImages.west[this->frameCount];
                    break;
                case 'D':
                    this->current = this->rotationImages.east[this->frameCount];
                    break;
                case 'W':
                    this->current = this->rotationImages.north[this->frameCount];
                    break;
                case 'S':
                    this->current = this->rotationImages.south[this->frameCount];
                    break;
            }
            this->timeElapsed = 0.0f;
        }
    }

    void draw(Position cameraPosition) {
        this->screenPosition.x = this->position.x - cameraPosition.x;
        this->screenPosition.y = this->position.y - cameraPosition.y;

        this->gameImage->drawImage(this->current, this->screenPosition.x, this->screenPosition.y);
    }
};