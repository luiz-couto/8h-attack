#include <iostream>
#include "GamesEngineeringBase.h"
#include "Manager.h"
#include "Debug.h"
#include "Menu.h"
#include "GameState.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define GAME_NAME "8H Attack!"

int main() {
    GamesEngineeringBase::Window canvas;
    canvas.create(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    bool running = true;

    GAME_STATE gameState = GAME_STATE::MAIN_MENU;

    Manager *manager = new Manager(&canvas, &gameState);
    Menu *menu = new Menu(&canvas, manager, &gameState);

    while (running)
    {
        // Check for input (key presses or window events)
        if (gameState == GAME_STATE::IN_GAME) {
            menu->manager->update();
        } else {
            menu->update();
        }

        // Clear the window for the next frame rendering
        canvas.clear();
        
        // Update game logic
        // Draw();

        if (gameState == GAME_STATE::IN_GAME) {
            menu->manager->draw();
        } else {
            if (gameState == GAME_STATE::PAUSE_MENU) {
                menu->showPauseMenu();
            } else if (gameState == GAME_STATE::GAME_OVER) {
                menu->showGameOverMenu();
            } else {
                menu->showMainMenu();
            }
        }

        // Display the frame on the screen. This must be called once the frame
        //is finished in order to display the frame.
        canvas.present();
    }
    return 0;
}