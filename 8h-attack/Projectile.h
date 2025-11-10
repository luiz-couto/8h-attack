#pragma once

#include "RigidBody.h"
#include "Debug.h"
#include "GamesEngineeringBase.h"

#define DEFAULT_SPEED 10
#define DEFAULT_PROJECTILE_DAMAGE 10
#define PROJECTILE_SIZE 10

class Projectile : public RigidBody {
    private:
    float dirX, dirY;
    int speed;
    int damage;

    public:
    Projectile(GamesEngineeringBase::Window* canvas, int x, int y, int targetX, int targetY, std::string name, int damage, int speed = DEFAULT_SPEED)
     : RigidBody(canvas, x, y) {
        this->setTarget(targetX, targetY);
        this->speed = speed;
        this->damage = damage;

        GamesEngineeringBase::Image *projectileImage = new GamesEngineeringBase::Image();
        projectileImage->load("assets/projectiles/" + name + ".png");
        this->currentFrame = projectileImage;
    }

    void setTarget(float targetX, float targetY) {        
        float dx = targetX - this->position.x;
        float dy = targetY - this->position.y;
        float distance = sqrt(dx * dx + dy * dy);

        this->dirX = dx / distance;
        this->dirY = dy / distance;
    }

    void update() {
        this->position.x += this->dirX * this->speed;
        this->position.y += this->dirY * this->speed;
    }

    bool isOutOfBounds(int terrainWidth, int terrainHeight) {
        if (this->position.y < 0 || this->position.y > terrainHeight) return true;
        if (this->position.x < 0 || this->position.x > terrainWidth) return true;
        return false;
    }

    int getDamage() {
        return this->damage;
    }
};