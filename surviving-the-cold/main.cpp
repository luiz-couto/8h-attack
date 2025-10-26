#include <iostream>
#include "GamesEngineeringBase.h"
#include "Character.h"
#include "GameImage.h"
#include "Terrain.h"
#include "Camera.h"
#include "Debug.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define GAME_NAME "Surviving the Cold"

// pallete
// main color = (100,125,255) - light blue

int main() {
    GamesEngineeringBase::Window canvas;
    canvas.create(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    bool running = true;

    Character character = Character(&canvas, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    Terrain terrain = Terrain(&canvas);
    Camera camera = Camera(&character, &terrain);

    terrain.loadTerrain("assets/terrains/3.terrain");
    std::cout << "Terrain size: " << terrain.width << " x " << terrain.height << std::endl;
    while (running)
    {
        // Check for input (key presses or window events)
        character.reactToMovementKeys(terrain.width * TILE_SIZE, terrain.height * TILE_SIZE);

        // Clear the window for the next frame rendering
        canvas.clear();
        
        // Update game logic
        // Draw();
        camera.drawNextFrame();

        // Display the frame on the screen. This must be called once the frame
        //is finished in order to display the frame.
        canvas.present();
    }
    return 0;
}