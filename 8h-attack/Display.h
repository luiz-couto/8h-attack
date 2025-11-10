#pragma once

#include <string>
#include "GamesEngineeringBase.h"
#include "GameImage.h"

#define MAX_DIGITS 6

// Display class to render numbers on the screen using images for each digit
class Display {
    private:
    GameImage* gameImage;
    GamesEngineeringBase::Image *digits[11]; // 0-9 and comma

    public:
    Display(GamesEngineeringBase::Window *canvas) {
        this->gameImage = new GameImage(canvas);
        for (int i=0; i<11; i++) {
            this->digits[i] = new GamesEngineeringBase::Image();
            std::string filename = "assets/display/" + std::to_string(i) + ".png";
            if (i == 10) {
                filename = "assets/display/comma.png";
            }
            this->digits[i]->load(filename);
        }
    }

    template<typename T>
    void drawNumber(T number, int positionX, int positionY) {
        std::string numberStr = std::to_string(number);
        int offsetX = 0;
        int digitCount = 0;
        for (char c : numberStr) {
            if (digitCount >= MAX_DIGITS) {
                break;
            }
            int digit = c - '0';
            if (c == '.') {
                digit = 10; // use comma image for decimal point
            }
            this->gameImage->drawImage(this->digits[digit], positionX + offsetX, positionY);
            offsetX += this->digits[digit]->width;
            digitCount++;
        }
    }

};