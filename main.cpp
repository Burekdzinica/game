#include <iostream>
#include <unordered_map>
#include <time.h>
#include <chrono>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "headers/player.hpp"
#include "headers/window.hpp"
#include "headers/arena.hpp"
#include "headers/enemy.hpp"
#include "headers/text.hpp"
#include "headers/ladder.hpp"
#include "headers/startScreen.hpp"
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

    game.clean();
    
    return EXIT_SUCCESS;
}
