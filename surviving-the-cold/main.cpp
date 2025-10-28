#include <iostream>
#include "GamesEngineeringBase.h"
#include "Manager.h"
#include "Debug.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define GAME_NAME "8H Attack!"

int main() {
    GamesEngineeringBase::Window canvas;
    canvas.create(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    bool running = true;

    Manager *manager = new Manager(&canvas);
   
    while (running)
    {
        // Check for input (key presses or window events)
        manager->update();

        // Clear the window for the next frame rendering
        canvas.clear();
        
        // Update game logic
        // Draw();
        manager->draw();

        // Display the frame on the screen. This must be called once the frame
        //is finished in order to display the frame.
        canvas.present();
    }
    return 0;
}