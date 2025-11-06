#pragma once

#include <string>
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Terrain.h"
#include "Player.h"
#include "NPC.h"
#include "Map.h"
#include "Vector.h"
#include "Random.h"
#include "PDList.h"

#define MAP_NUMBER "4"

#define PLAYER_NAME "caz"
#define PLAYER_START_SPEED 7
#define PLAYER_START_HEALTH 100
#define PLAYER_START_DAMAGE 1

#define NPC_DEFAULT_COOLDOWN 0.5f
#define NPCS_NUMBER 30

class Manager {
    private:
    GamesEngineeringBase::Window *canvas;
    Player *player;
    Camera *camera;
    Map *map;
    NPC *npcs[NPCS_NUMBER];

    GamesEngineeringBase::Timer timer = GamesEngineeringBase::Timer();
    float timeElapsedNPCs = 0.0f;
    float npcCooldown = NPC_DEFAULT_COOLDOWN;

    public:
    Manager(GamesEngineeringBase::Window *canvas) {
        this->canvas = canvas;
        this->player = new Player(
            canvas,
            PLAYER_NAME,
            PLAYER_START_SPEED, 
            PLAYER_START_HEALTH, 
            PLAYER_START_DAMAGE, 
            canvas->getWidth() / 2, 
            canvas->getHeight() / 2
        );
        this->camera = new Camera(this->player->getPosition());
        this->map = new Map(this->canvas, MAP_NUMBER);

        for (int i=0; i<NPCS_NUMBER; i++) {
            this->npcs[i] = nullptr;
        }
    }

    void update() {
        float timeElapsed = this->timer.dt();

        Position playerPos = this->player->getPosition();
        RigidBody **mapObjects = this->map->getObjects();
        
        // update camera
        this->camera->update(playerPos, this->map->getWidthInPixels(), this->map->getHeightInPixels());

        // generate new NPCs
        this->timeElapsedNPCs += timeElapsed;
        if (this->timeElapsedNPCs > this->npcCooldown) {
            for (int i=0; i<NPCS_NUMBER; i++) {
                if (this->npcs[i] == nullptr) {
                    this->npcs[i] = new NPC(
                        this->canvas,
                        "flames",
                        RandomInt(1, 4).generate(),
                        100,
                        1,
                        RandomInt(0, this->map->getWidthInPixels()).generate(),
                        RandomInt(0, this->map->getHeightInPixels()).generate()
                    );
                    break;
                }
            }
            this->timeElapsedNPCs = 0.0f;
        }
        
        // update NPCs
        for (int i=0; i<NPCS_NUMBER; i++) {
            if (this->npcs[i] != nullptr) {
                if (!this->npcs[i]->isAlive()) {
                    delete this->npcs[i];
                    this->npcs[i] = nullptr;
                    continue;
                }
                this->npcs[i]->update(&playerPos);
            }
        }
        
        NPC *nearestNPC = this->npcs[0];
        for (int i=1; i<NPCS_NUMBER; i++) {
            if (this->npcs[i] == nullptr) {
                continue;
            }
            // check collisions with NPCs
            if (this->player->detectCollision(this->npcs[i])) {
                this->player->processCollision(NPC_COLLISION, this->npcs[i]);
            }
            // find nearest NPC
            if (this->player->getDistanceTo(this->npcs[i]) < this->player->getDistanceTo(nearestNPC)) {
                nearestNPC = this->npcs[i];
            }

            // check collisions between NPCs and player projectiles
            PDList<Projectile> *playerProjectiles = this->player->getProjectilesArray();
            NPC *currentNPC = this->npcs[i];
            playerProjectiles->forEach([&currentNPC, &playerProjectiles](Projectile &projectile, int j) {
                if (currentNPC->detectCollision(&projectile)) {
                    currentNPC->processCollision(PROJECTILE_COLLISION, &projectile);
                    playerProjectiles->deleteByIdx(j);
                }
            });
        }

        this->player->reactToMovementKeys(this->map->getWidthInPixels(), this->map->getHeightInPixels(), nearestNPC);
        
        // check collisions with map objects
        for (int i=0; i<this->map->numberOfObjects; i++) {
            if (this->player->detectCollision(mapObjects[i])) {
                this->player->processCollision(OBJECT_COLLISION, mapObjects[i]);
                break;
            }
        }

        // check collisions with terrain impassable tiles
        Terrain* terrain = this->map->getTerrain();
        Vector<ImpassableTile*> impassableTiles = terrain->getImpassableTiles();
        for (int i=0; i<impassableTiles.getSize(); i++) {
            ImpassableTile* tile = impassableTiles[i];
            if (tile->detectCollision(this->player)) {
                this->player->processCollision(TERRAIN_COLLISION, nullptr);
                break;
            }
        }

        // final player update
        this->player->update();
    }

    void draw() {
        Position cameraPosition = this->camera->getPosition();

        this->map->draw(cameraPosition);
        this->player->draw(cameraPosition);
        this->player->drawProjectiles(cameraPosition, this->map->getWidthInPixels(), this->map->getHeightInPixels());

        for (int i=0; i<NPCS_NUMBER; i++) {
            if (this->npcs[i] == nullptr) {
                continue;
            }
            this->npcs[i]->draw(cameraPosition);
        }
    }

};