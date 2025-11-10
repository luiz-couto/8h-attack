#include <iostream>
#include <chrono>
#include <iostream>
#include "GamesEngineeringBase.h"
#include "Manager.h"
#include "Debug.h"
#include "Menu.h"
#include "GameState.h"

using Clock = std::chrono::high_resolution_clock;

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define NUMBER_OF_FRAMES_FOR_NEXT_FPS_CALCULATION 20

#define GAME_NAME "8H Attack!"

#define TARGET_FPS 60
#define TARGET_FRAME_TIME (1.0 / TARGET_FPS)

int main() {
    GamesEngineeringBase::Window canvas;
    canvas.create(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    bool running = true;

    GAME_STATE gameState = GAME_STATE::MAIN_MENU;

    Manager *manager = new Manager(&canvas, &gameState);
    Menu *menu = new Menu(&canvas, manager, &gameState);

    auto lastTime = Clock::now();
    double deltaTime = 0.0;
    double fps = 0.0;
    int frameCount = 0;
    double totalTime = 0.0;
    double deltaSum = 0.0;
    int frameSinceLastUpdate = 0;

    // Frame accumulator for 60 FPS updates
    double frameAccumulator = 0.0;

    while (running)
    {   

        auto currentTime = Clock::now();
        deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
        lastTime = currentTime;

        totalTime += deltaTime;
        frameCount++;

        // accumulate frame times for FPS calculation
        deltaSum += deltaTime;
        frameSinceLastUpdate++;

        if (frameSinceLastUpdate >= NUMBER_OF_FRAMES_FOR_NEXT_FPS_CALCULATION) {
            double avgDelta = deltaSum / frameSinceLastUpdate;
            fps = 1.0 / avgDelta;

            frameSinceLastUpdate = 0;
            deltaSum = 0.0;
        }

        // Accumulate time for 60 FPS updates
        frameAccumulator += deltaTime;
        bool shouldUpdate = false;
        
        // Check if we should update (at 60 FPS rate)
        if (frameAccumulator >= TARGET_FRAME_TIME) {
            shouldUpdate = true;
            frameAccumulator -= TARGET_FRAME_TIME;
            
            // Prevent spiral of death: if far behind, reset
            if (frameAccumulator > TARGET_FRAME_TIME * 5) {
                frameAccumulator = 0.0;
            }
        }

        // Check for input and updates
        if (gameState == GAME_STATE::IN_GAME) {
            menu->manager->update(shouldUpdate);
        } else {
            menu->update();
        }

        // Clear the window for the next frame rendering
        canvas.clear();

        // Draw the current frame
        if (gameState == GAME_STATE::IN_GAME) {
            menu->manager->draw(fps);
        } else {
            if (gameState == GAME_STATE::PAUSE_MENU) {
                menu->showPauseMenu();
            } else if (gameState == GAME_STATE::GAME_OVER) {
                menu->showGameOverMenu(fps);
            } else {
                menu->showMainMenu();
            }
        }

        // Display the frame on the screen. This must be called once the frame
        // is finished in order to display the frame.
        canvas.present();
    }
    return 0;
}