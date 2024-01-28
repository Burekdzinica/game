#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.hpp"
#include "window.hpp"

using namespace std;

const int WIDTH = 820, HEIGHT = 700;

int main(int argc, char *argv[])
{
    Window window("Reševanje bikca Ferdinanda", WIDTH, HEIGHT);

    Player player(3, {500, 120, 50, 50});

    while (true)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
            {
                break;
            }

            switch (event.key.keysym.sym)
            {
            case SDLK_a:
                player.setX(-10);
                break;
            case SDLK_d:
                player.setX(10);
                break;
            case SDLK_w:
                player.setY(-10);
                break;
            case SDLK_s:
                player.setY(10);
                break;
            }
        }

        window.clear();
        window.draw(player.getAsset());
        window.present();
    }

    window.~Window();

    return EXIT_SUCCESS;
}
