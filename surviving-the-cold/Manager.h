#pragma once

#include <string>
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Terrain.h"
#include "Player.h"
#include "NPC.h"

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
    Terrain *terrain;
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

        this->terrain = new Terrain(canvas);
        this->terrain->loadTerrain("assets/terrains/1.terrain");

        for (int i=0; i<NPCS_NUMBER; i++) {
            NPC *flames = new NPC(canvas, "flames", 2, 100, 1, WINDOW_WIDTH/2 + ((i+1)*200), WINDOW_HEIGHT/2);
            this->npcs[i] = flames;
        }
    }

    void update() {
        Position playerPos = this->player->getPosition();

        // update camera
        this->camera->update(playerPos, this->terrain->width, this->terrain->height);

        // update NPCs
        for (int i=0; i<NPCS_NUMBER; i++) {
            this->npcs[i]->update(&playerPos);
        }

        // check collisions
        for (int i=0; i<NPCS_NUMBER; i++) {
            if (this->player->detectCollision(this->npcs[i])) {
                this->player->processCollision(NPC_COLLISION, this->npcs[i]);
            }
        }

        this->player->reactToMovementKeys(this->terrain->width * TILE_SIZE, this->terrain->height * TILE_SIZE);
    }

    void draw() {
        Position cameraPosition = this->camera->getPosition();

        this->terrain->drawTerrain(cameraPosition);
        this->player->draw(cameraPosition);

        for (int i=0; i<NPCS_NUMBER; i++) {
            this->npcs[i]->draw(cameraPosition);
        }
    }

};