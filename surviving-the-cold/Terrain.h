#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include "GamesEngineeringBase.h"
#include "GameImage.h"
#include "Debug.h"

enum TileKind {
    FULL_GRASS,
};

// Terrain file = *.terrain
// First line is relative to the size MxN
// The others indicate which tiles to use
class Terrain {
    private:
    GamesEngineeringBase::Window* canvas;
    int width, height;
    int** terrain;

    public:
    Terrain(GamesEngineeringBase::Window *canvas) {
        this->canvas = canvas;
    }

    void allocateTerrainMemory() {
        Debug::print(1);
        this->terrain = new int*[this->height];
        for (int i = 0; i < this->height; ++i)
            this->terrain[i] = new int[this->width];
    }

    void freeTerrainMemory() {
        for (int i = 0; i < this->height; ++i)
            delete [] terrain[i];
        delete [] terrain;
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