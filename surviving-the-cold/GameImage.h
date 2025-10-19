#pragma once

#include <iostream>
#include "GamesEngineeringBase.h"


class GameImage {
    private:
    GamesEngineeringBase::Window* canvas;

    public:
    GameImage(GamesEngineeringBase::Window *canvas) {
        this->canvas = canvas;
    }

    void drawImage(std::string fileName, int positionX, int positionY) {
        GamesEngineeringBase::Image image = GamesEngineeringBase::Image();
        image.load(fileName);

        for (int i=positionX; i < positionX + image.width; i++) {
            for (int j=positionY; j < positionY + image.height; j++) {
                char r = image.at(i, j, 0);
                char g = image.at(i, j, 1);
                char b = image.at(i, j, 2);
                canvas->draw(i, j, r, g, b);
            }
        }
    }

};