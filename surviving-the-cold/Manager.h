#pragma once

#include <string>
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Terrain.h"
#include "Player.h"
#include "NPC.h"
#include "Map.h"
#include "Vector.h"

#define PLAYER_NAME "caz"
#define PLAYER_START_SPEED 7
#define PLAYER_START_HEALTH 100
#define PLAYER_START_DAMAGE 1

#define NPCS_NUMBER 1

class Manager {
    private:
    GamesEngineeringBase::Window *canvas;
    Player *player;
    Camera *camera;
    Map *map;
    NPC *npcs[NPCS_NUMBER];

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
        this->map = new Map(this->canvas, "4");

        for (int i=0; i<NPCS_NUMBER; i++) {
            NPC *flames = new NPC(canvas, "flames", 2, 100, 1, WINDOW_WIDTH/2 + ((i+1)*200), WINDOW_HEIGHT/2);
            this->npcs[i] = flames;
        }
    }

    void update() {
        Position playerPos = this->player->getPosition();
        RigidBody **mapObjects = this->map->getObjects();
        
        // update camera
        this->camera->update(playerPos, this->map->getWidth(), this->map->getHeight());
        
        // update NPCs
        for (int i=0; i<NPCS_NUMBER; i++) {
            this->npcs[i]->update(&playerPos);
        }
        
        // check collisions with NPCs
        for (int i=0; i<NPCS_NUMBER; i++) {
            if (this->player->detectCollision(this->npcs[i])) {
                this->player->processCollision(NPC_COLLISION, this->npcs[i]);
            }
        }

        this->player->reactToMovementKeys(this->map->getWidth() * TILE_SIZE, this->map->getHeight() * TILE_SIZE);
        
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

        for (int i=0; i<NPCS_NUMBER; i++) {
            this->npcs[i]->draw(cameraPosition);
        }
    }

};