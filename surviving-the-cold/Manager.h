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
#include "Menu.h"

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
    PDList<NPC, NPCS_NUMBER> *npcs = new PDList<NPC, NPCS_NUMBER>();

    GamesEngineeringBase::Timer timer = GamesEngineeringBase::Timer();
    float timeElapsedNPCs = 0.0f;
    float npcCooldown = NPC_DEFAULT_COOLDOWN;

    GAME_STATE *gameState;

    public:
    Manager(GamesEngineeringBase::Window *canvas, GAME_STATE *gameState) {
        this->canvas = canvas;
        this->gameState = gameState;
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
    }

    void update() {
        if (this->canvas->keyPressed('P')) {
            *this->gameState = GAME_STATE::PAUSE_MENU;
            return;
        }

        float timeElapsed = this->timer.dt();

        Position playerPos = this->player->getPosition();
        RigidBody **mapObjects = this->map->getObjects();
        
        // update camera
        this->camera->update(playerPos, this->map->getWidthInPixels(), this->map->getHeightInPixels());

        // generate new NPCs
        this->timeElapsedNPCs += timeElapsed;
        if (this->timeElapsedNPCs > this->npcCooldown) {
            this->npcs->add(new NPC(
                this->canvas,
                "flames",
                RandomInt(1, 4).generate(),
                100,
                1,
                RandomInt(0, this->map->getWidthInPixels()).generate(),
                RandomInt(0, this->map->getHeightInPixels()).generate()
            ));
            this->timeElapsedNPCs = 0.0f;
        }

        // update NPCs
        PDList<NPC, NPCS_NUMBER> *npcs = this->npcs;
        npcs->forEach([&playerPos, &npcs](NPC &npc, int idx) {
            if (!npc.isAlive()) {
                npcs->deleteByIdx(idx);
            } else {
                npc.update(&playerPos);
            }
        });
        
        NPC *nearestNPC = this->npcs->at(0);
        if (nearestNPC != nullptr) {
            Player *player = this->player;
            npcs->forEach([&nearestNPC, &player](NPC &npc, int idx) {
                if (player->detectCollision(&npc)) {
                    player->processCollision(NPC_COLLISION, &npc);
                }

                if (player->getDistanceTo(&npc) < player->getDistanceTo(nearestNPC)) {
                    nearestNPC = &npc;
                }

                PDList<Projectile> *playerProjectiles = player->getProjectilesArray();
                playerProjectiles->forEach([&npc, &playerProjectiles](Projectile &projectile, int j) {
                    if (npc.detectCollision(&projectile)) {
                        npc.processCollision(PROJECTILE_COLLISION, &projectile);
                        playerProjectiles->deleteByIdx(j);
                    }
                });
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

        this->npcs->forEach([&cameraPosition](NPC &npc, int idx) {
            npc.draw(cameraPosition);
        });
    }

};