#pragma once
#include <fstream>
#include <sstream>
#include "GamesEngineeringBase.h"
#include "Position.h"
#include "RigidBody.h"
#include "Terrain.h"

#define NUMBER_OF_OBJECTS 1
#define TILE_SIZE 32

// Map class composed of a Terrain and an array of RigidBody objects
class Map {
    private:
    GamesEngineeringBase::Window *canvas;
    Terrain *terrain;
    RigidBody **objects;
    
    public:
    int numberOfObjects = 0;
    Map(GamesEngineeringBase::Window *canvas, std::string mapNumber) {
        this->canvas = canvas;

        Terrain *terrain = new Terrain(canvas);
        terrain->loadTerrain("assets/terrains/" + mapNumber + ".terrain");

        this->terrain = terrain;
        this->loadMapObjects(mapNumber);
    }

    void loadMapObjects(std::string mapNumber) {
        std::ifstream infile("assets/map-objects/" + mapNumber + ".objects");
        std::string line;

        std::getline(infile, line);
        std::istringstream stringStream(line);

        stringStream >> this->numberOfObjects;
        this->objects = new RigidBody*[this->numberOfObjects];

        for (int j=0; j<this->numberOfObjects; j++)
        {
            std::getline(infile, line);
            std::istringstream stringStream(line);

            std::string object, positionX, positionY;
            stringStream >> object >> positionX >> positionY;

            GamesEngineeringBase::Image *objectImage = new GamesEngineeringBase::Image();
            objectImage->load("assets/objects/" + object + ".png");
            this->objects[j] = new RigidBody(this->canvas, std::stoi(positionX), std::stoi(positionY), objectImage);
        }
    }

    int getWidthInPixels() {
        return this->terrain->width * TILE_SIZE;
    }

    int getHeightInPixels() {
        return this->terrain->height * TILE_SIZE;
    }

    Terrain* getTerrain() {
        return this->terrain;
    }

    RigidBody** getObjects() {
        return this->objects;
    }

    void draw(Position cameraPosition) {
        this->terrain->drawTerrain(cameraPosition);

        for (int i=0; i<this->numberOfObjects; i++) {
            this->objects[i]->draw(cameraPosition);
        }
    }
};