#include <iostream>
#include <SDL2/SDL.h>

#include "headers/game.hpp"
#include "headers/startScreen.hpp"
#include "headers/pauseScreen.hpp"


using namespace std;

int main(int argc, char *argv[])
{
    const int FPS = 60;
    int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    Game game;
    game.setup();

    StartScreen startscreen(Data::renderer);
    PauseScreen pausescreen;

    while (game.isOpen())
    {
       frameStart = SDL_GetTicks();

        if (Data::inStartScreen)
            startscreen.run(Data::renderer);
        
        else if (Data::inPauseScreen)
            pausescreen.run(Data::renderer);
    
        else
        {
            if (Data::isPlayerAlive)
            game.update();

            else if (!Data::isPlayerAlive)
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
        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    return EXIT_SUCCESS;
}