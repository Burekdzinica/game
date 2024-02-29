#include <iostream>
#include <SDL2/SDL.h>

#include "headers/game.hpp"


using namespace std;

int main(int argc, char *argv[])
{
    const int fps = 60;
    int frameDelay = 1000 / fps;
    Uint32 frameStart;
    int frameTime;

    Game game;
    game.setup();
    Game game;
    game.setup();

    while (game.isOpen())
    while (game.isOpen())
    {
       frameStart = SDL_GetTicks();

        if (Data::isPlayerAlive)
            game.update();

        else
        {
            const Uint8* keystate = SDL_GetKeyboardState(nullptr);

            SDL_Event event;
            if (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    return EXIT_SUCCESS;
    
                if (keystate[SDL_SCANCODE_R])
                    game.restart();
            }
        }

        game.render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    return EXIT_SUCCESS;
}