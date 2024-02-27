#include <iostream>
#include <SDL2/SDL.h>

#include "headers/game.hpp"


using namespace std;

int main(int argc, char *argv[])
{
    const int fps = 120;
    int frameDelay = 1000 / fps;
    Uint32 frameStart;
    int frameTime;

    Game game;
    game.setup();

    while (game.isOpen())
    {
       frameStart = SDL_GetTicks();

        game.update();
        game.render();


        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    return EXIT_SUCCESS;
}