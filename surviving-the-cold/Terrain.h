#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "Position.h"
#include "RigidBody.h"
#include "GamesEngineeringBase.h"
#include "GameImage.h"
#include "Debug.h"
#include "Vector.h"

#define TILE_SIZE 32
#define NUMBER_OF_TILES 10
#define IMPASSABLE_TILES_COUNT 2

enum TileKind {
    FULL_GRASS,
    FULL_EARTH,
    FULL_WATER,
};

std::string getTileFile(int tileKind) {
    return "assets/tiles/" + std::to_string(tileKind) + ".png";
}

int IMPASSABLE_TILES[IMPASSABLE_TILES_COUNT] = {2,3}; // Water

class ImpassableTile {
    private:
    Position position;
    int width;
    int height;

    public:
    ImpassableTile(int x, int y, int width, int height) {
        this->position = Position{x, y};
        this->width = width;
        this->height = height;
    }

    bool detectCollision(RigidBody *rigidBody) {
        Position rigidBodyPos = rigidBody->getPosition();
        if(this->position.x < rigidBodyPos.x + rigidBody->getWidth() &&
            this->position.x + this->width > rigidBodyPos.x &&
            this->position.y < rigidBodyPos.y + rigidBody->getHeight() &&
            this->position.y + this->height > rigidBodyPos.y) 
        {
            return true;
        }
        return false;
    }

};

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
    Vector<ImpassableTile*> impassableTiles;

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
                int tileKind;
                stringStream >> tileKind;
                this->terrain[j][i] = tileKind;
                if (isImpassable(tileKind)) {
                    ImpassableTile* impTile = new ImpassableTile(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                    this->impassableTiles.add(impTile);
                }
            }
        }
    }

    bool isImpassable(int tileKind) {
        for (int i=0; i<IMPASSABLE_TILES_COUNT; i++) {
            if (tileKind == IMPASSABLE_TILES[i]) {
                return true;
            }
        }
        return false;
    }

    Vector<ImpassableTile*> getImpassableTiles() {
        return this->impassableTiles;
    }

    void drawTerrain(Position cameraPosition) {
        for (int i=0; i < this->canvas->getHeight(); i++) {
            for (int j=0; j < this->canvas->getWidth(); j++) {
                int worldPixelX = cameraPosition.x + j;
                int worldPixelY = cameraPosition.y + i;

                int tileX = worldPixelX / TILE_SIZE;
                int tileY = worldPixelY / TILE_SIZE;

                int tileKind = this->terrain[tileY][tileX];
                GamesEngineeringBase::Image* tileImage = getTileImage(tileKind);

                int tileImageX = worldPixelX % TILE_SIZE;
                int tileImageY = worldPixelY % TILE_SIZE;

                if (tileImage->alphaAt(tileImageX, tileImageY) > 0) {
                    canvas->draw(j, i, tileImage->at(tileImageX, tileImageY));
                }
            }
        }
    }

    void drawTerrainInfinite(Position cameraPosition) {
        for (int i=0; i < this->canvas->getHeight(); i++) {
            for (int j=0; j < this->canvas->getWidth(); j++) {
                int worldPixelX = cameraPosition.x + j;
                int worldPixelY = cameraPosition.y + i;

                int tileX = (worldPixelX / TILE_SIZE) % this->width;
                int tileY = (worldPixelY / TILE_SIZE) % this->height;
                if (tileX < 0) tileX += this->width;
                if (tileY < 0) tileY += this->height;

                int tileKind = this->terrain[tileY][tileX];
                GamesEngineeringBase::Image* tileImage = getTileImage(tileKind);

                int tileImageX = worldPixelX % TILE_SIZE;
                int tileImageY = worldPixelY % TILE_SIZE;
                if (tileImageX < 0) tileImageX += TILE_SIZE;
                if (tileImageY < 0) tileImageY += TILE_SIZE;

                if (tileImage->alphaAt(tileImageX, tileImageY) > 0) {
                    canvas->draw(j, i, tileImage->at(tileImageX, tileImageY));
                }
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