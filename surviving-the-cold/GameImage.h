#pragma once

#include <iostream>
#include "GamesEngineeringBase.h"

// GameImage class to safely draw images into the window canvas
class GameImage {
    private:
    GamesEngineeringBase::Window* canvas;

    public:
    GameImage(GamesEngineeringBase::Window *canvas) {
        this->canvas = canvas;
    }

    void drawImage(GamesEngineeringBase::Image *image, int positionX, int positionY, int brigthness = 0, bool drawBorders = false) {
        for (int i=0; i < image->width; i++) {
            for (int j=0; j < image->height; j++) {
                if (i + positionX < 0 || i + positionX >= canvas->getWidth() ||
                    j + positionY < 0 || j + positionY >= canvas->getHeight()) {
                    continue;
                }
                if (image->alphaAt(i, j) > 0) {
                    int r = min(255, image->at(i, j, 0) + brigthness);
                    int g = min(255, image->at(i, j, 1) + brigthness);
                    int b = min(255, image->at(i, j, 2) + brigthness);
                    canvas->draw(i + positionX, j + positionY, r, g, b);
                    continue;
                }
                if (drawBorders) {
                    if (i == 0 || j == 0 || i == image->width - 1 || j == image->height - 1) {
                        canvas->draw(i + positionX, j + positionY, 255,0,0); // red border
                    }
                }
            }
        }
    }
};