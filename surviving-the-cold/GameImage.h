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

    void drawImage(GamesEngineeringBase::Image *image, int positionX, int positionY) {
        for (int i=positionX; i < positionX + image->width; i++) {
            for (int j=positionY; j < positionY + image->height; j++) {
                char r = image->at(i%image->width, j%image->height, 0);
                char g = image->at(i%image->width, j%image->height, 1);
                char b = image->at(i%image->width, j%image->height, 2);
                canvas->draw(i, j, r, g, b);
            }
        }
    }
};