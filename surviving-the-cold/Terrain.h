#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "GamesEngineeringBase.h"
#include "GameImage.h"
#include "Debug.h"

#define TILE_SIZE 32
#define NUMBER_OF_TILES 10

enum TileKind {
    FULL_GRASS,
    FULL_EARTH,
    FULL_WATER,
};

std::string getTileFile(int tileKind) {
    return "assets/tiles/" + std::to_string(tileKind) + ".png";
}

// Terrain file = *.terrain
// First line indicates the size MxN
// The others indicate which tiles to use
// The tiles data is stored to avoid loading them again all the time
class Terrain {
    private:
    GamesEngineeringBase::Window* canvas;
    int** terrain;
    GamesEngineeringBase::Image* tilesData[NUMBER_OF_TILES];
    GameImage* gameImage;
    
    public:
    int width, height;
    Terrain(GamesEngineeringBase::Window *canvas) {
        this->canvas = canvas;
        this->terrain = nullptr;
        this->gameImage = new GameImage(canvas);
        for (int i=0; i<NUMBER_OF_TILES; i++) {
            this->tilesData[i] = new GamesEngineeringBase::Image();
        }
    }

    void allocateTerrainMemory() {
        freeTerrainMemory();
        this->terrain = new int*[this->height];
        for (int i = 0; i < this->height; ++i)
            this->terrain[i] = new int[this->width];
    }

    void freeTerrainMemory() {
        if (terrain != NULL) {
            for (int i = 0; i < this->height; ++i)
                delete [] terrain[i];
            delete [] terrain;
        }
    }

    GamesEngineeringBase::Image* getTileImage(int tileKind) {
        GamesEngineeringBase::Image* tileImage = this->tilesData[tileKind];
        if (tileImage->data == NULL) {
            std::string tileFilename = getTileFile(tileKind);
            tileImage->load(tileFilename);
        }
        return tileImage;
    }

    void loadTerrain(std::string terrainFile) {
        std::ifstream infile(terrainFile);
        std::string line;

        std::getline(infile, line);
        std::istringstream stringStream(line);

        stringStream >> this->width >> this->height;
        allocateTerrainMemory();

        for (int j=0; j<this->height; j++)
        {
            std::getline(infile, line);
            std::istringstream stringStream(line);
            for (int i=0; i<this->width; i++) {
                stringStream >> this->terrain[j][i];
            }
        }
    }

    void drawTerrain(int cameraX, int cameraY) {
        for (int i=0; i < this->canvas->getHeight(); i++) {
            for (int j=0; j < this->canvas->getWidth(); j++) {
                int worldPixelX = cameraX + j;
                int worldPixelY = cameraY + i;

                int tileX = worldPixelX / TILE_SIZE;
                int tileY = worldPixelY / TILE_SIZE;

                int tileKind = this->terrain[tileY][tileX];
                GamesEngineeringBase::Image* tileImage = getTileImage(tileKind);

                int tileImageX = worldPixelX % TILE_SIZE;
                int tileImageY = worldPixelY % TILE_SIZE;

                canvas->draw(j, i, tileImage->at(tileImageX, tileImageY));
            }
        }
    }

    void printTerrainRaw() {
        for (int i=0; i<this->height; i++) {
            for (int j=0; j<this->width; j++) {
                std::cout << this->terrain[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }

    ~Terrain() {
        this->freeTerrainMemory();
    }
};