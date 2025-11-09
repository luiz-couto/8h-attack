#pragma once

#include <string>
#include <chrono>
#include <thread>
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Terrain.h"
#include "Player.h"
#include "NPC.h"
#include "NPCStatic.h"
#include "Map.h"
#include "Vector.h"
#include "Random.h"
#include "PDList.h"
#include "GameState.h"
#include "Display.h"
#include "GameImage.h"

#define MAP_NUMBER "3"

#define PLAYER_NAME "caz"
#define PLAYER_START_SPEED 7
#define PLAYER_START_HEALTH 100
#define PLAYER_START_DAMAGE 10

#define NPC_DEFAULT_COOLDOWN 1.5f
#define STATIC_NPC_DEFAULT_COOLDOWN 5.0f

#define NPCS_NUMBER 60
#define DIFFERENT_NPCS_NUM 5

#define BORDERS_OFFSET 150

std::string NPCS_NAMES[DIFFERENT_NPCS_NUM] = { "balle", "green", "red", "purple", "flames" };
bool NPCS_IS_STATIC[DIFFERENT_NPCS_NUM] = { false, false, false, false, true };
int NPCS_SPEEDS[DIFFERENT_NPCS_NUM] = { 3, 2, 2, 4, 0 };
int NPCS_DAMAGES[DIFFERENT_NPCS_NUM] = { 5, 10, 7, 1, 5 };
int NPCS_HEALTHS[DIFFERENT_NPCS_NUM] = { 30, 50, 40, 60, 70 };

class Manager {
    private:
    GamesEngineeringBase::Window *canvas;
    GameImage *gameImage;
    Player *player;
    Camera *camera;
    std::string mapNumber = MAP_NUMBER;
    Map *map;
    PDList<NPC, NPCS_NUMBER> *npcs = new PDList<NPC, NPCS_NUMBER>();

    GamesEngineeringBase::Timer timer = GamesEngineeringBase::Timer();
    
    float timeElapsedNPCs = 0.0f;
    float npcCooldown = NPC_DEFAULT_COOLDOWN;

    float timeElapsedStaticNPCs = 0.0f;
    float staticNpcCooldown = STATIC_NPC_DEFAULT_COOLDOWN;

    GAME_STATE *gameState;
    int score = 0;

    Display *display;
    GamesEngineeringBase::Image *FPSImage = new GamesEngineeringBase::Image();

    public:
    Manager(GamesEngineeringBase::Window *canvas, GAME_STATE *gameState) {
        this->canvas = canvas;
        this->gameState = gameState;
        this->display = new Display(canvas);
        this->FPSImage->load("assets/display/fps.png");
        this->gameImage = new GameImage(canvas);

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
        this->map = new Map(this->canvas, this->mapNumber, true);
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

    Position generateNewNPCPosition(int borderOffset) {
        Position cameraPosition = this->camera->getPosition();
        int possibleXRange[2][2] = {{0 - borderOffset, cameraPosition.x - borderOffset}, {cameraPosition.x + this->canvas->getWidth() + borderOffset, this->map->getWidthInPixels() + borderOffset}};
        int possibleYRange[2][2] = {{0 - borderOffset, cameraPosition.y - borderOffset}, {cameraPosition.y + this->canvas->getHeight() + borderOffset, this->map->getHeightInPixels() + borderOffset}};

        RandomInt rangeSelector(0, 1);
        int xRangeIdx = rangeSelector.generate();
        int yRangeIdx = rangeSelector.generate();

        //RandomInt xGenerator(possibleXRange[xRangeIdx][0], possibleXRange[xRangeIdx][1]);
        //RandomInt yGenerator(possibleYRange[yRangeIdx][0], possibleYRange[yRangeIdx][1]);
        
        RandomInt xGenerator(0, 3000);
        RandomInt yGenerator(0, 3000);

        Position npcPosition;
        npcPosition.x = xGenerator.generate();
        npcPosition.y = yGenerator.generate();
        return npcPosition;
    }

    void update() {
        if (!this->player->isAlive()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1200));
            *this->gameState = GAME_STATE::GAME_OVER;
            return;
        }

        if (this->canvas->keyPressed('P')) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            *this->gameState = GAME_STATE::PAUSE_MENU;
            return;
        }

        float timeElapsed = this->timer.dt();

        Position playerPos = this->player->getPosition();
        RigidBody **mapObjects = this->map->getObjects();
        
        // update camera
        if (this->map->isInfinite) {
            this->camera->updateInfinite(playerPos);
        } else {
            this->camera->update(playerPos, this->map->getWidthInPixels(), this->map->getHeightInPixels());
        }

        // generate new static NPCs
        this->timeElapsedStaticNPCs += timeElapsed;
        int staticNPCIdx = DIFFERENT_NPCS_NUM - 1;
        if (this->timeElapsedStaticNPCs > this->staticNpcCooldown) {            
            Position npcPosition = this->generateNewNPCPosition(-BORDERS_OFFSET);
            this->npcs->add(new NPCStatic(
                this->canvas,
                NPCS_NAMES[staticNPCIdx],
                NPCS_HEALTHS[staticNPCIdx],
                NPCS_DAMAGES[staticNPCIdx],
                npcPosition.x,
                npcPosition.y
            ));
            this->timeElapsedStaticNPCs = 0.0f;
        }

        // generate new NPCs
        this->timeElapsedNPCs += timeElapsed;
        int randomNPCIdx = RandomInt(0, DIFFERENT_NPCS_NUM - 2).generate();
        if (this->timeElapsedNPCs > this->npcCooldown) {            
            Position npcPosition = this->generateNewNPCPosition(BORDERS_OFFSET);
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
            this->npcCooldown *= 0.98f;
        }

        // update NPCs
        PDList<NPC, NPCS_NUMBER> *npcs = this->npcs;
        int &score = this->score;
        Map *map = this->map;
        npcs->forEach([&playerPos, &npcs, &score, &map](NPC &npc, int idx) {
            if (!npc.isAlive()) {
                score += npc.damage;
                npcs->deleteByIdx(idx);
            } else {
                if (npc.isNPCStatic()) {
                    NPCStatic *staticNPC = static_cast<NPCStatic*>(&npc);
                    staticNPC->update(&playerPos, map->isInfinite, map->getWidthInPixels(), map->getHeightInPixels());
                } else {
                    npc.update(&playerPos, map->isInfinite, map->getWidthInPixels(), map->getHeightInPixels());
                }
            }
        });
        
        NPC *nearestNPC = this->npcs->at(0);
        Camera *camera = this->camera;
        if (nearestNPC != nullptr) {
            Player *player = this->player;

            int aoeNPCSAffected = 0;
            if (player->isAOEActivated) {
                aoeNPCSAffected = this->player->aoeNPCSAffected;
                this->npcs->sort([](NPC &a, NPC &b) {
                    return a.health > b.health;
                });
                player->isAOEActivated = false;
            }

            npcs->forEach([&nearestNPC, &player, &camera, aoeNPCSAffected](NPC &npc, int idx) {
                if (idx < aoeNPCSAffected) {
                    npc.processCollision(AOE_PLAYER_ATTACK, player);
                }

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

                if (npc.isNPCStatic()) {
                    NPCStatic *staticNPC = static_cast<NPCStatic*>(&npc);
                    PDList<Projectile> *npcProjectiles = staticNPC->getProjectilesArray();
                    npcProjectiles->forEach([&player, &npcProjectiles](Projectile &projectile, int j) {
                        if (player->detectCollision(&projectile)) {
                            player->processCollision(PROJECTILE_COLLISION, &projectile);
                            npcProjectiles->deleteByIdx(j);
                        }
                    });
                }
            });
        }

        this->player->reactToMovementKeys(
            this->map->getWidthInPixels(), 
            this->map->getHeightInPixels(), 
            nearestNPC, 
            camera->getPosition(),
            this->map->isInfinite
        );

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

    void draw(double fps) {
        Position cameraPosition = this->camera->getPosition();

        this->map->draw(cameraPosition);
        this->player->draw(cameraPosition);


        this->player->drawProjectiles(cameraPosition, this->map->getWidthInPixels(), this->map->getHeightInPixels());

        Map *map = this->map;
        this->npcs->forEach([&cameraPosition, &map](NPC &npc, int idx) {
            npc.draw(cameraPosition);
            if (npc.isNPCStatic()) {
                NPCStatic *staticNPC = static_cast<NPCStatic*>(&npc);
                staticNPC->drawProjectiles(cameraPosition, map->getWidthInPixels(), map->getHeightInPixels());
            }
        });

        this->player->drawAOEBarCharging();
        this->drawFPS(fps);
    }

    void drawFPS(double fps) {
        this->gameImage->drawImage(this->FPSImage, 20, 720);
        this->display->drawNumber<double>(fps, 100, 720);
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

    int getScore() {
        return this->score;
    }
};