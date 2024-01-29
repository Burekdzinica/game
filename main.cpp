#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#include "player.hpp"
#include "window.hpp"
#include "arena.hpp"
#include "enemy.hpp"

using namespace std;

const int WIDTH = 1200, HEIGHT = 700;

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int position_x = rand() % 800;
    int position_y = rand() % 800;

    Window window("Reševanje bikca Ferdinanda", WIDTH, HEIGHT);

    Player player(3, {500, 100, 50, 50});
    Arena arena({rand()%800, rand()%800, 50, 50});
    Enemy enemy({position_x, position_y, 100, 100});
    
    bool playerTouchedArena = false;
    bool playerTouchedEnemy = false;
    bool playerAlive = true;

    while (playerAlive)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
            {
                break;
            }

            if (event.type == SDL_KEYDOWN)
            {
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
        }

        cout << player.getAsset().x << " " << player.getAsset().y << "\n";

        if (arena.isPlayerNearby(player.getAsset(), arena.getAsset(), 100))
            cout << "Player is nearby"; 
        
        if (arena.isPlayerTouching(player.getAsset()))
        {
            cout << "Player is touching arena \n";
            playerTouchedArena = true;
        }

        if (enemy.isPlayerTouching(player.getAsset()))
        {
            playerTouchedEnemy = true;
            playerAlive = false;
        }


        window.clear();
        //draws arena if player is near and deletes it when player touches arena
        if (!playerTouchedArena && arena.isPlayerNearby(player.getAsset(), arena.getAsset(), 100))
        {
            window.drawArena(arena.getAsset());
        }

        //deletes player if it touches enemy
        if (!playerTouchedEnemy)
        {
            window.drawPlayer(player.getAsset());
        }

        window.drawEnemy(enemy.getAsset());
        window.present();
    }

    window.~Window();

    return EXIT_SUCCESS;
}
