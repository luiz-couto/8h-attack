#pragma once
#include <iostream>
#include <fstream>
#include "GamesEngineeringBase.h"
#include "GameImage.h"
#include "Position.h"
#include "Manager.h"
#include "GameState.h"
#include "Display.h"

#define MINIMUM_TIME_BETWEEN_FRAMES 0.09f

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

void startNewGame(Menu* menu);
void continueGame(Menu* menu);
void loadGame(Menu* menu);
void actionQuit(Menu* menu);
void saveAndExitToMainMenu(Menu* menu);
void emptyAction(Menu* menu);
void exitToMainMenu(Menu* menu);

MenuActions MainMenu = {
    3,
    startNewGame,
    loadGame,
    actionQuit
};

MenuActions PauseMenu = {
    2,
    continueGame,
    saveAndExitToMainMenu,
    emptyAction
};

MenuActions GameOverMenu = {
    2,
    startNewGame,
    exitToMainMenu,
    emptyAction
};

class Menu {
    private:
    GameImage *gameImage;
    
    GamesEngineeringBase::Image *mainMenuImage;
    GamesEngineeringBase::Image *pauseMenuImage;
    GamesEngineeringBase::Image *gameOverMenuImage;
    
    GamesEngineeringBase::Image *arrowImage;
    int arrowPositionIndex = 0;
    
    GamesEngineeringBase::Timer timer = GamesEngineeringBase::Timer();
    float timeElapsed = 0.0f;
    Display *display;
    
    MenuActions *menuActions = &MainMenu;
    
    public:
    GamesEngineeringBase::Window *canvas;
    Manager *manager;
    GAME_STATE *gameState;
    Menu(GamesEngineeringBase::Window *canvas, Manager *manager, GAME_STATE *gameState) {
        this->canvas = canvas;
        this->manager = manager;
        this->gameState = gameState;
        this->gameImage = new GameImage(this->canvas);
        this->display = new Display(this->canvas);

        this->mainMenuImage = new GamesEngineeringBase::Image();
        this->mainMenuImage->load("assets/menu/mainMenu.png");

        this->pauseMenuImage = new GamesEngineeringBase::Image();
        this->pauseMenuImage->load("assets/menu/pauseMenu.png");

        this->gameOverMenuImage = new GamesEngineeringBase::Image();
        this->gameOverMenuImage->load("assets/menu/gameOverMenu.png");

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
            case GAME_STATE::GAME_OVER:
                this->menuActions = &GameOverMenu;
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

    void showGameOverMenu(double averageFPS) {
        this->gameImage->drawImage(gameOverMenuImage, 0, 0);
        this->gameImage->drawImage(arrowImage, arrowPositions[arrowPositionIndex].x, arrowPositions[arrowPositionIndex].y);

        this->display->drawNumber<int>(this->manager->getScore(), 385, 480);
        this->display->drawNumber<double>(averageFPS, 362, 555);
    }
};

void startNewGame(Menu* menu) {
    Manager *manager = new Manager(menu->canvas, menu->gameState);
    delete menu->manager;
    menu->manager = manager;
    *menu->gameState = GAME_STATE::IN_GAME;
}

void continueGame(Menu* menu) {
    *menu->gameState = GAME_STATE::IN_GAME;
}

void actionQuit(Menu* menu) {
    exit(0);
}

void emptyAction(Menu *menu) {
    // Do nothing
}

void saveGame(Menu* menu) {
    std::cout << "Saving game..." << "\n";
    std::ofstream gameState("assets/saves/0.gamestate");

    // first line is map number and score
    gameState << menu->manager->getMapNumber() << " " << menu->manager->getScore() << "\n";

    // second line is camera position
    Position cameraPos = menu->manager->getCameraPosition();
    gameState << cameraPos.x << " " << cameraPos.y << "\n";

    // third line is player information
    Player* player = menu->manager->getPlayer();
    std::string playerName = player->getName();
    Position playerPos = player->getPosition();
    gameState 
        << playerName << " "
        << playerPos.x << " " 
        << playerPos.y << " "
        << player->speed << " "
        << player->initialHealth << " "
        << player->health << " "
        << player->damage << "\n";

    // fourth line is number of NPCs
    int numOfNPCs = menu->manager->getNPCs()->size();
    gameState << numOfNPCs << "\n";

    // fifth line is NPC information
    menu->manager->getNPCs()->forEach([&gameState](NPC &npc, int idx) {
        std::string npcName = npc.getName();
        Position npcPos = npc.getPosition();
        gameState 
            << npcName << " " 
            << npcPos.x << " " 
            << npcPos.y << " "
            << npc.speed << " "
            << npc.initialHealth << " "
            << npc.health << " "
            << npc.damage << " "
            << npc.isNPCStatic() << "\n";
    });

    gameState.close();
    std::cout << "Game saved." << "\n";
}

void loadGame(Menu *menu) {
    std::cout << "Loading game..." << "\n";
    std::ifstream gameState("assets/saves/0.gamestate");
    if (!gameState.is_open()) {
        std::cout << "No saved game found." << "\n";
        return;
    }

    std::string mapNumber;
    int score;
    gameState >> mapNumber >> score;

    int cameraX, cameraY;
    gameState >> cameraX >> cameraY;

    std::string playerName;
    float playerX, playerY, playerSpeed,  playerInitialHealth, playerHealth, playerDamage;
    gameState >> playerName >> playerX >> playerY >> playerSpeed >> playerInitialHealth >> playerHealth >> playerDamage;

    std::cout << "Loaded Player: " << playerName << " at (" << playerX << ", " << playerY << ")\n";
    Player* player = new Player(
        menu->canvas,
        playerName,
        playerSpeed,
        playerHealth,
        playerDamage,
        playerX,
        playerY
    );
    player->initialHealth = playerInitialHealth;

    std::cout << "Creating Manager for loaded game...\n";
    std::cout << "Map Number: " << mapNumber << "\n";
    Manager* manager = new Manager(
        menu->canvas,
        mapNumber,
        player,
        new PDList<NPC, NPCS_NUMBER>(),
        new Camera(Position{cameraX, cameraY}),
        menu->gameState,
        score
    );

    int numOfNPCs;
    gameState >> numOfNPCs;

    for (int i = 0; i < numOfNPCs; i++) {
        std::string npcName;
        float npcX, npcY, npcSpeed,  npcInitialHealth, npcHealth, npcDamage, isStatic;
        gameState >> npcName >> npcX >> npcY >> npcSpeed >> npcInitialHealth >> npcHealth >> npcDamage >> isStatic;

        if (isStatic) {
            NPCStatic* staticNPC = new NPCStatic(
                menu->canvas,
                npcName,
                npcHealth,
                npcDamage,
                npcX,
                npcY
            );

            staticNPC->initialHealth = npcInitialHealth;
            manager->getNPCs()->add(staticNPC);
            continue;
        }

        NPC* npc = new NPC(
            menu->canvas,
            npcName,
            npcSpeed,
            npcHealth,
            npcDamage,
            npcX,
            npcY
        );

        npc->initialHealth = npcInitialHealth;
        manager->getNPCs()->add(npc);
    }

    gameState.close();

    delete menu->manager;
    menu->manager = manager;
    *menu->gameState = GAME_STATE::IN_GAME;
    std::cout << "Game loaded." << "\n";
}

void exitToMainMenu(Menu *menu) {
    *menu->gameState = GAME_STATE::MAIN_MENU;
}

void saveAndExitToMainMenu(Menu *menu) {
    saveGame(menu);
    *menu->gameState = GAME_STATE::MAIN_MENU;
}