#include <iostream>
#include "GamesEngineeringBase.h"
#include "Character.h"
#include "GameImage.h"
#include "Terrain.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define GAME_NAME "Surviving the Cold"

// pallete
// main color = (100,125,255) - light blue

void renderBackground(GamesEngineeringBase::Window &canvas) {
    unsigned char backgroundColorPixel[3] = {100, 125, 255};
    for (int i=0; i<WINDOW_WIDTH; i++) {
        for (int j=0; j<WINDOW_HEIGHT; j++) {
            canvas.draw(i, j, backgroundColorPixel);
        }
    }
}

int main() {
    GamesEngineeringBase::Window canvas;
    canvas.create(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    bool running = true;

    Character character = Character(&canvas, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    GameImage gameImage = GameImage(&canvas);
    Terrain terrain = Terrain(&canvas);

    terrain.loadTerrain("assets/terrains/1.terrain");
    terrain.printTerrainRaw();

    while (running)
    {
        // Check for input (key presses or window events)
        character.reactToMovementKeys();

        // Clear the window for the next frame rendering
        canvas.clear();
        
        // Update game logic
        // Draw();
        renderBackground(canvas);

        gameImage.drawImage("assets/tiles/0.png", 0, 0);
        character.draw();

        // Display the frame on the screen. This must be called once the frame
        //is finished in order to display the frame.
        canvas.present();
    }
    return 0;
}