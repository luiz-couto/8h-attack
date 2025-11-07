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
#include "GameState.h"

#define MAP_NUMBER "4"

#define PLAYER_NAME "caz"
#define PLAYER_START_SPEED 7
#define PLAYER_START_HEALTH 100
#define PLAYER_START_DAMAGE 10

#define NPC_DEFAULT_COOLDOWN 0.5f
#define NPCS_NUMBER 15
#define DIFFERENT_NPCS_NUM 2

#define BORDERS_OFFSET 150

std::string NPCS_NAMES[DIFFERENT_NPCS_NUM] = { "balle", "flames" };
int NPCS_SPEEDS[DIFFERENT_NPCS_NUM] = { 4, 2 };
int NPCS_DAMAGES[DIFFERENT_NPCS_NUM] = { 5, 10 };
int NPCS_HEALTHS[DIFFERENT_NPCS_NUM] = { 70, 100 };

class Manager {
    private:
    GamesEngineeringBase::Window *canvas;
    Player *player;
    Camera *camera;
    std::string mapNumber = MAP_NUMBER;
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
        this->map = new Map(this->canvas, this->mapNumber);
    }

    Manager(GamesEngineeringBase::Window *canvas, std::string mapNumber, Player *player, PDList<NPC, NPCS_NUMBER> *npcs, Camera *camera, GAME_STATE *gameState) {
        this->canvas = canvas;
        this->gameState = gameState;
        this->player = player;
        this->mapNumber = mapNumber;
        this->npcs = npcs;

        this->camera = camera;
        this->map = new Map(this->canvas, this->mapNumber);
    }

    ~Manager() {
        delete this->player;
        delete this->camera;
        delete this->map;
        delete this->npcs;
    }

    Position generateNewNPCPosition() {
        Position cameraPosition = this->camera->getPosition();
        int possibleXRange[2][2] = {{0 - BORDERS_OFFSET, cameraPosition.x - BORDERS_OFFSET}, {cameraPosition.x + this->canvas->getWidth() + BORDERS_OFFSET, this->map->getWidthInPixels() + BORDERS_OFFSET}};
        int possibleYRange[2][2] = {{0 - BORDERS_OFFSET, cameraPosition.y - BORDERS_OFFSET}, {cameraPosition.y + this->canvas->getHeight() + BORDERS_OFFSET, this->map->getHeightInPixels() + BORDERS_OFFSET}};

        RandomInt rangeSelector(0, 1);
        int xRangeIdx = rangeSelector.generate();
        int yRangeIdx = rangeSelector.generate();
        RandomInt xGenerator(possibleXRange[xRangeIdx][0], possibleXRange[xRangeIdx][1]);
        RandomInt yGenerator(possibleYRange[yRangeIdx][0], possibleYRange[yRangeIdx][1]);
        
        Position npcPosition;
        npcPosition.x = xGenerator.generate();
        npcPosition.y = yGenerator.generate();
        return npcPosition;
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
        int randomNPCIdx = RandomInt(0, DIFFERENT_NPCS_NUM - 1).generate();
        if (this->timeElapsedNPCs > this->npcCooldown) {
            Position npcPosition = this->generateNewNPCPosition();
            this->npcs->add(new NPC(
                this->canvas,
                NPCS_NAMES[randomNPCIdx],
                NPCS_SPEEDS[randomNPCIdx],
                NPCS_HEALTHS[randomNPCIdx],
                NPCS_DAMAGES[randomNPCIdx],
                npcPosition.x,
                npcPosition.y
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
        Camera *camera = this->camera;
        if (nearestNPC != nullptr) {
            Player *player = this->player;
            npcs->forEach([&nearestNPC, &player, &camera](NPC &npc, int idx) {
                if (player->detectCollision(&npc)) {
                    player->processCollision(NPC_COLLISION, &npc);
                }

                if (npc.isOnCameraView(camera->getPosition()) && player->getDistanceTo(&npc) < player->getDistanceTo(nearestNPC)) {
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

        this->player->reactToMovementKeys(this->map->getWidthInPixels(), this->map->getHeightInPixels(), nearestNPC, camera->getPosition());

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

    Player* getPlayer() {
        return this->player;
    }

    std::string getMapNumber() {
        return this->mapNumber;
    }

    PDList<NPC, NPCS_NUMBER> *getNPCs() {
        return this->npcs;
    }

    Position getCameraPosition() {
        return this->camera->getPosition();
    }
};