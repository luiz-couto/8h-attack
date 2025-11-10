#pragma once

#include "NPC.h"
#include "PDList.h"
#include "Position.h"
#include "Player.h"

#define PROJECTILE_DEFAULT_COOLDOWN 1.0f
#define PROJECTILE_SPEED 4

// NPCStatic class derived from NPC that fires projectiles at the player
class NPCStatic : public NPC {
    private:
    PDList<Projectile> *projectiles = new PDList<Projectile>();
    float projectileCooldown = PROJECTILE_DEFAULT_COOLDOWN;
    float cooldownTimeElapsed = 0.0f;

    public:
    NPCStatic(GamesEngineeringBase::Window* canvas, std::string name, int health, int damage, int x, int y) 
    : NPC(canvas, name, 0, health, damage, x, y) {
        this->isStatic = true;
    }

    void fireNextProjectile(Player *player) {
        this->projectiles->add(new Projectile(
            this->canvas,
            this->getCenterPosition().x,
            this->getCenterPosition().y,
            player->getCenterPosition().x,
            player->getCenterPosition().y,
            "0",
            this->damage,
            PROJECTILE_SPEED
        ));
    }

    void drawProjectiles(Position cameraPosition, int terrainWidth, int terrainHeight) {
        PDList<Projectile> *projectiles = this->projectiles;
        projectiles->forEach([&projectiles, &cameraPosition, &terrainWidth, &terrainHeight](Projectile &projectile, int idx) {
            if (projectile.isOutOfBounds(terrainWidth, terrainHeight)) {
                projectiles->deleteByIdx(idx);
            } else {
                projectile.draw(cameraPosition);
            }
        });
    }

    PDList<Projectile>* getProjectilesArray() {
        return this->projectiles;
    }

    void update(Player *player) {
        float frameElapsedTime = this->timer.dt();
        
        this->cooldownTimeElapsed += frameElapsedTime;
        if (cooldownTimeElapsed > projectileCooldown) {
            this->cooldownTimeElapsed = 0.0f;
            this->fireNextProjectile(player);
        }

        this->projectiles->forEach([](Projectile &projectile, int idx) {
            projectile.update();
        });

        this->timeElapsed += frameElapsedTime;
        if (this->timeElapsed > TIME_BETWEEN_FRAMES) {
            this->frameCount++;
            if (this->frameCount == 3) {
                this->frameCount = 0;
            }
            this->timeElapsed = 0.0f;
        }

        this->currentFrame = this->frames[this->frameCount];
    }
};