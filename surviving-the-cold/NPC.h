#pragma once
#include <string>
#include "Position.h"
#include "GamesEngineeringBase.h"
#include "GameImage.h"
#include "RigidBody.h"

#define START_SPEED 2
#define TIME_BETWEEN_FRAMES 0.25f

class NPC : public RigidBody {
    private:
    std::string name;
    GamesEngineeringBase::Image *frames[3];
    int frameCount = 0;
    int speed = START_SPEED;

    public:
    NPC(GamesEngineeringBase::Window* canvas, std::string name, int x, int y) : RigidBody(canvas, x, y) {
        this->name = name;
        loadFrames();
    }

    void loadFrames() {
        for (int i=0; i<3; i++) {
            this->frames[i] = new GamesEngineeringBase::Image();
            this->frames[i]->load("assets/characters/" + this->name + "/" + std::to_string(i) + ".png");
        }
    }

    void update(Position *playerPos) {
        this->timeElapsed += this->timer.dt();
        if (this->timeElapsed > TIME_BETWEEN_FRAMES) {
            this->frameCount++;
            if (this->frameCount == 3) {
                this->frameCount = 0;
            }
            this->timeElapsed = 0.0f;
        }

        this->currentFrame = this->frames[this->frameCount];

        int dx = playerPos->x - this->position.x;
        int dy = playerPos->y - this->position.y;

        float distance = sqrt(pow(dx, 2) +  pow(dy, 2));

        if (distance > this->speed) {
            float unitX = dx / distance;
            float unitY = dy / distance;

            this->position.x += floor(unitX * this->speed);
            this->position.y += floor(unitY * this->speed);
            return;
        }
    }
};