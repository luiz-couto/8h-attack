#pragma once
#include "GamesEngineeringBase.h"
#include "GameImage.h"
#include "Position.h"

#define MINIMUM_TIME_BETWEEN_FRAMES 0.09f

enum GAME_STATE {
    MAIN_MENU,
    IN_GAME,
    PAUSE_MENU,
    GAME_OVER
};

Position arrowFirstOption = {290, 285};
Position arrowSecondOption = {290, 360};
Position arrowThirdOption = {290, 430};

Position arrowPositions[3] = {
    arrowFirstOption,
    arrowSecondOption,
    arrowThirdOption
};

class Menu {
    private:
    GamesEngineeringBase::Window *canvas;
    GAME_STATE *gameState;
    GameImage *gameImage;

    GamesEngineeringBase::Image *mainMenuImage;

    GamesEngineeringBase::Image *arrowImage;
    int arrowPositionIndex = 0;

    GamesEngineeringBase::Timer timer = GamesEngineeringBase::Timer();
    float timeElapsed = 0.0f;

    public:
    Menu(GamesEngineeringBase::Window *canvas, GAME_STATE *gameState) {
        this->canvas = canvas;
        this->gameState = gameState;
        this->gameImage = new GameImage(this->canvas);

        this->mainMenuImage = new GamesEngineeringBase::Image();
        this->mainMenuImage->load("assets/menu/mainMenu.png");

        this->arrowImage = new GamesEngineeringBase::Image();
        this->arrowImage->load("assets/menu/whiteArrow.png");
        this->arrowPositionIndex = 0;
    }

    void update() {
        this->timeElapsed += this->timer.dt();
        if (this->timeElapsed < MINIMUM_TIME_BETWEEN_FRAMES) {
            return;
        }
        
        if (this->canvas->keyPressed('W')) {
            this->arrowPositionIndex = (this->arrowPositionIndex - 1 + 3) % 3;
        } else if (this->canvas->keyPressed('S')) {
            this->arrowPositionIndex = (this->arrowPositionIndex + 1) % 3;
        } else if (this->canvas->keyPressed('\r')) {
            if (this->arrowPositionIndex == 0) {
                *this->gameState = GAME_STATE::IN_GAME;
            } else if (this->arrowPositionIndex == 1) {
                // Pause Menu (not implemented yet)
            } else if (this->arrowPositionIndex == 2) {
                exit(0);
            }
        }
        this->timeElapsed = 0.0f;
    }

    void showMainMenu() {
        this->gameImage->drawImage(mainMenuImage, 0, 0);
        this->gameImage->drawImage(arrowImage, arrowPositions[arrowPositionIndex].x, arrowPositions[arrowPositionIndex].y);
    }

    void showPauseMenu() {
        // Implementation for showing the pause menu
    }

    void showGameOverMenu() {
        // Implementation for showing the game over menu
    }
};