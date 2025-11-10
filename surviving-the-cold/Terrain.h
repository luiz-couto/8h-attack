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

int IMPASSABLE_TILES[IMPASSABLE_TILES_COUNT] = {2,3}; // Water, tower

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
// The terrain colors are pre-computed and stored directly for performance
class Terrain {
    private:
    GamesEngineeringBase::Window* canvas;
    unsigned char*** terrainColors;
    GamesEngineeringBase::Image* tilesData[NUMBER_OF_TILES];
    GameImage* gameImage;
    Vector<ImpassableTile*> impassableTiles;

    public:
    int width, height;
    Terrain(GamesEngineeringBase::Window *canvas) {
        this->canvas = canvas;
        this->terrainColors = nullptr;
        this->gameImage = new GameImage(canvas);
        for (int i=0; i<NUMBER_OF_TILES; i++) {
            this->tilesData[i] = new GamesEngineeringBase::Image();
        }
    }

    void allocateTerrainMemory() {
        freeTerrainMemory();

        int totalPixelsHeight = this->height * TILE_SIZE;
        int totalPixelsWidth = this->width * TILE_SIZE;
        this->terrainColors = new unsigned char**[totalPixelsHeight];

        for (int i = 0; i < totalPixelsHeight; ++i) {
            this->terrainColors[i] = new unsigned char*[totalPixelsWidth];
            for (int j = 0; j < totalPixelsWidth; ++j) {
                this->terrainColors[i][j] = new unsigned char[4]; // RGBA
            }
        }
    }

    void freeTerrainMemory() {
        if (terrainColors != NULL) {
            int totalPixelsHeight = this->height * TILE_SIZE;
            for (int i = 0; i < totalPixelsHeight; i++) {
                int totalPixelsWidth = this->width * TILE_SIZE;
                for (int j = 0; j < totalPixelsWidth; j++) {
                    delete [] terrainColors[i][j];
                }
                delete [] terrainColors[i];
            }
            delete [] terrainColors;
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

                GamesEngineeringBase::Image* tileImage = getTileImage(tileKind);
                for (int ty = 0; ty < TILE_SIZE; ty++) {
                    for (int tx = 0; tx < TILE_SIZE; tx++) {
                        int pixelY = j * TILE_SIZE + ty;
                        int pixelX = i * TILE_SIZE + tx;
                        this->terrainColors[pixelY][pixelX] = tileImage->at(tx, ty);
                    }
                }
                
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

                unsigned char* color = this->terrainColors[worldPixelY][worldPixelX];
                canvas->draw(j, i, color);
            }
        }
    }

    ~Terrain() {
        this->freeTerrainMemory();
    }
};