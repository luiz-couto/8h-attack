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

class Menu;

typedef void (*MenuAction)(Menu *);
struct MenuActions {
    int numOfOptions;
    MenuAction first;
    MenuAction second;
    MenuAction third;
};

void startGame(Menu* menu);
void loadGame(Menu* menu);
void actionQuit(Menu* menu);
void exitToMainMenu(Menu* menu);
void emptyAction(Menu* menu);

MenuActions MainMenu = {
    3,
    startGame,
    loadGame,
    actionQuit
};

MenuActions PauseMenu = {
    2,
    startGame,
    exitToMainMenu,
    emptyAction
};

class Menu {
    private:
    GamesEngineeringBase::Window *canvas;
    GameImage *gameImage;
    
    GamesEngineeringBase::Image *mainMenuImage;
    GamesEngineeringBase::Image *pauseMenuImage;
    
    GamesEngineeringBase::Image *arrowImage;
    int arrowPositionIndex = 0;
    
    GamesEngineeringBase::Timer timer = GamesEngineeringBase::Timer();
    float timeElapsed = 0.0f;

    MenuActions *menuActions = &MainMenu;
    
    public:
    GAME_STATE *gameState;
    Menu(GamesEngineeringBase::Window *canvas, GAME_STATE *gameState) {
        this->canvas = canvas;
        this->gameState = gameState;
        this->gameImage = new GameImage(this->canvas);

        this->mainMenuImage = new GamesEngineeringBase::Image();
        this->mainMenuImage->load("assets/menu/mainMenu.png");

        this->pauseMenuImage = new GamesEngineeringBase::Image();
        this->pauseMenuImage->load("assets/menu/pauseMenu.png");

        this->arrowImage = new GamesEngineeringBase::Image();
        this->arrowImage->load("assets/menu/whiteArrow.png");
        this->arrowPositionIndex = 0;
    }

    void switchMenuActions() {
        switch(*this->gameState) {
            case GAME_STATE::MAIN_MENU:
                this->menuActions = &MainMenu;
                break;
            case GAME_STATE::PAUSE_MENU:
                this->menuActions = &PauseMenu;
                break;
            default:
                break;
        }
    }

    void update() {
        this->switchMenuActions();

        this->timeElapsed += this->timer.dt();
        if (this->timeElapsed < MINIMUM_TIME_BETWEEN_FRAMES) {
            return;
        }
        
        if (this->canvas->keyPressed('W')) {
            this->arrowPositionIndex = (this->arrowPositionIndex - 1 + menuActions->numOfOptions) % menuActions->numOfOptions;
        } else if (this->canvas->keyPressed('S')) {
            this->arrowPositionIndex = (this->arrowPositionIndex + 1) % menuActions->numOfOptions;
        } else if (this->canvas->keyPressed('\r')) {
            switch (this->arrowPositionIndex) {
                case 0:
                    this->menuActions->first(this);
                    break;
                case 1:
                    this->menuActions->second(this);
                    break;
                case 2:
                    this->menuActions->third(this);
                    break;
                default:
                    break;
            }
        }
        this->timeElapsed = 0.0f;
    }

    void showMainMenu() {
        this->gameImage->drawImage(mainMenuImage, 0, 0);
        this->gameImage->drawImage(arrowImage, arrowPositions[arrowPositionIndex].x, arrowPositions[arrowPositionIndex].y);
    }

    void showPauseMenu() {
        this->gameImage->drawImage(pauseMenuImage, 0, 0);
        this->gameImage->drawImage(arrowImage, arrowPositions[arrowPositionIndex].x, arrowPositions[arrowPositionIndex].y);
    }

    void showGameOverMenu() {
        // Implementation for showing the game over menu
    }
};

void startGame(Menu* menu) {
    *menu->gameState = GAME_STATE::IN_GAME;
}

void loadGame(Menu* menu) {
    std::cout << "Load Game" << "\n";
}

void actionQuit(Menu* menu) {
    exit(0);
}

void exitToMainMenu(Menu *menu) {
    *menu->gameState = GAME_STATE::MAIN_MENU;
}

void emptyAction(Menu *menu) {
    // Do nothing
}