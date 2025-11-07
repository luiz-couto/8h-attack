#pragma once

#include <string>
#include "GamesEngineeringBase.h"
#include "GameImage.h"
#include "Position.h"
#include "Debug.h"
#include "RigidBody.h"

#define HEALTH_BAR_WIDTH 50
#define HEALTH_BAR_HEIGHT 5

class Character : public RigidBody {
    public:
    std::string name;
    bool hadDamage = false;
    int speed;
    int health;
    int damage;
    int initialHealth;

    Character(GamesEngineeringBase::Window *canvas, std::string name, int speed, int health, int damage, int x, int y)
        : RigidBody(canvas, x, y) {
    
        this->name = name;
        this->speed = speed;
        this->initialHealth = health;
        this->health = health;
        this->damage = damage;
    }

    std::string getName() {
        return this->name;
    }

    void draw(Position cameraPosition) {
        int drawPositionX = this->position.x - cameraPosition.x;
        int drawPositionY = this->position.y - cameraPosition.y;

        int brightness = this->hadDamage ? 70 : 0;
        if (this->hadDamage) {
            this->hadDamage = false;
        }

        this->gameImage->drawImage(this->currentFrame, drawPositionX, drawPositionY, brightness);
        drawHealthBar(cameraPosition);
    }

    bool isAlive() {
        return this->health > 0;
    }

    void drawHealthBar(Position cameraPosition) {
        int barX = this->position.x - cameraPosition.x + (this->currentFrame->width / 2) - (HEALTH_BAR_WIDTH / 2);
        int barY = this->position.y - cameraPosition.y + (this->currentFrame->height);

        // Draw background (red)
        for (int i=0; i<HEALTH_BAR_WIDTH; i++) {
            for (int j=0; j<HEALTH_BAR_HEIGHT; j++) {
                if (barX + i < 0 || barX + i >= this->canvas->getWidth() ||
                    barY + j < 0 || barY + j >= this->canvas->getHeight()) {
                    continue;
                }
                this->canvas->draw(barX + i, barY + j, 255, 0, 0);
            }
        }

        // Draw health (green)
        int healthWidth = static_cast<int>((static_cast<float>(this->health) / static_cast<float>(this->initialHealth)) * HEALTH_BAR_WIDTH);
        for (int i=0; i<healthWidth; i++) {
            for (int j=0; j<HEALTH_BAR_HEIGHT; j++) {
                if (barX + i < 0 || barX + i >= this->canvas->getWidth() ||
                    barY + j < 0 || barY + j >= this->canvas->getHeight()) {
                    continue;
                }
                this->canvas->draw(barX + i, barY + j, 0, 255, 0);
            }
        }
    }
};