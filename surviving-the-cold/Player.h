#pragma once

#include <string>
#include "GamesEngineeringBase.h"
#include "GameImage.h"
#include "Position.h"
#include "Debug.h"
#include "RigidBody.h"
#include "NPC.h"
#include "Character.h"
#include "Projectile.h"
#include "PDList.h"

#define LOADING_FRAME 3
#define IDLE_FRAME_TIME 0.3f

#define PROJECTILES_ARR_COUNT 100
#define PROJECTILE_DEFAULT_COOLDOWN 0.2f

struct Rotation {
    GamesEngineeringBase::Image *south[5];
    GamesEngineeringBase::Image *north[5];
    GamesEngineeringBase::Image *east[5];
    GamesEngineeringBase::Image *west[5];
};

class Player : public Character {
    private:
    Rotation rotationImages;
    char lastKeyPressed = 'S';
    int frameCount = 0;
    int loadingFrame = 0;
    Position lastPosition;

    PDList<Projectile> *projectiles = new PDList<Projectile>();
    float projectileCooldown = PROJECTILE_DEFAULT_COOLDOWN;
    float cooldownTimeElapsed = 0.0f;

    public:
    Player(GamesEngineeringBase::Window *canvas, std::string name, int speed, int health, int damage, int x, int y)
        : Character(canvas, name, speed, health, damage, x, y) {

        Rotation rotationImages;
        loadAnimationFrames(rotationImages.south, "south");
        loadAnimationFrames(rotationImages.north, "north");
        loadAnimationFrames(rotationImages.west, "west");
        loadAnimationFrames(rotationImages.east, "east");

        this->rotationImages = rotationImages;
        this->currentFrame = this->rotationImages.south[0];
        this->lastPosition = this->position;
    }

    void fireNextProjectile(int targetX, int targetY) {
        this->projectiles->add(new Projectile(
            this->canvas,
            this->position.x,
            this->position.y,
            targetX,
            targetY
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
        this->currentFrame = group[this->frameCount];
        this->loadingFrame = 0;
    }

    void setPositionAsLastPosition() {
        this->position = this->lastPosition;
    }

    void reactToMovementKeys(int boundaryWidth, int boundaryHeight, NPC *nearestNPC) {
        float frameElapsedTime = this->timer.dt();

        this->cooldownTimeElapsed += frameElapsedTime;
        if (cooldownTimeElapsed > projectileCooldown) {
            if (nearestNPC != nullptr) {
                this->cooldownTimeElapsed = 0.0f;
                this->fireNextProjectile(nearestNPC->getCenterPosition().x, nearestNPC->getCenterPosition().y);
            }
        }

        this->projectiles->forEach([](Projectile &projectile, int idx) {
            projectile.update();
        });

        if (this->canvas->keyPressed('A')) {
            this->position.x = max(this->position.x - this->speed, 0);
            selectNextFrame('A', this->rotationImages.west);
            return;
        }
        if (this->canvas->keyPressed('D')) {
            this->position.x = min(this->position.x + this->speed, boundaryWidth - this->currentFrame->width);
            selectNextFrame('D', this->rotationImages.east);
            return;
        }
        if (this->canvas->keyPressed('W')) {
            this->position.y = max(this->position.y - this->speed, 0);
            selectNextFrame('W', this->rotationImages.north);
            return;
        }
        if (this->canvas->keyPressed('S')) {
            this->position.y = min(this->position.y + this->speed, boundaryHeight - this->currentFrame->height);
            selectNextFrame('S', this->rotationImages.south);
            return;
        }

        this->timeElapsed += frameElapsedTime;
        if (timeElapsed > IDLE_FRAME_TIME) {
            this->frameCount = 0;
            switch (this->lastKeyPressed) {
                case 'A':
                    this->currentFrame = this->rotationImages.west[this->frameCount];
                    break;
                case 'D':
                    this->currentFrame = this->rotationImages.east[this->frameCount];
                    break;
                case 'W':
                    this->currentFrame = this->rotationImages.north[this->frameCount];
                    break;
                case 'S':
                    this->currentFrame = this->rotationImages.south[this->frameCount];
                    break;
            }
            this->timeElapsed = 0.0f;
        }
    }

    void processCollision(COLLISION_KIND kind, RigidBody *rigidBody) {
        switch (kind) {
            case NPC_COLLISION: {
                NPC *npc = static_cast<NPC*>(rigidBody);
                if (this->health > 0) {
                    this->health -= npc->damage;
                }
                this->hadDamage = true;
                break;
            }
            case OBJECT_COLLISION:
                this->setPositionAsLastPosition();
                break;
            case TERRAIN_COLLISION:
                this->setPositionAsLastPosition();
                break;
        }
    }

    void update() {
        this->lastPosition = this->position;
    }
};