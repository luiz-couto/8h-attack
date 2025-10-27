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
        for (int i=0; i < image->width; i++) {
            for (int j=0; j < image->height; j++) {
                if (i + positionX < 0 || i + positionX >= canvas->getWidth() ||
                    j + positionY < 0 || j + positionY >= canvas->getHeight()) {
                    continue;
                }
                if (image->alphaAt(i, j) > 0) {
                    canvas->draw(i + positionX, j + positionY, image->at(i, j));
                }
            }
        }
    }
};