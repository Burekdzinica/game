#include <iostream>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "player.hpp"
#include "window.hpp"
#include "arena.hpp"
#include "enemy.hpp"
#include "text.hpp"

using namespace std;

const int WIDTH = 1700, HEIGHT = 820;

int points = 0;

int main(int argc, char *argv[])
{
    srand(time(NULL));

    Window window("Reševanje bikca Ferdinanda", WIDTH, HEIGHT);

    Player player(3, {500, 100, 200, 200});
    Arena arena({rand()%1000, rand()%820, 200, 200});
    Enemy enemy({rand()%1040, rand()%540, 200, 200});
    Text text;

    window.init();

    bool playerTouchedArena = false;
    bool playerTouchedEnemy = false;
    while (player.getIsPlayerAlive())
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
        const char* textString = ("Points: " + to_string(points)).c_str();

        //cout << player.getAsset().x << " " << player.getAsset().y << "\n";

        if (arena.isPlayerNearby(player.getAsset(), arena.getAsset(), 100))
            cout << "Player is nearby \n"; 
        
        if (arena.isPlayerTouching(player.getAsset()))
        {
            cout << "Player is touching arena \n";
            playerTouchedArena = true;
        }

        // collision detected
        if (enemy.isPlayerTouching(player.getAsset()))
        {
            // playerTouchedEnemy = true;
            enemy.setX(rand()%400);
            enemy.setY(rand()%540);
            player.changeHealth(-1);
            points += 100;
        }

        window.clear();
        text.createText("ARCADECLASSIC.ttf", window.getRenderer(), textString);
        
        //draws arena if player is near and deletes it when player touches arena
        if (!playerTouchedArena && arena.isPlayerNearby(player.getAsset(), arena.getAsset(), 100))
        {
            window.draw(window.getRenderer(), arena.getAsset(), "assets/arena.png");
        }



        //deletes player if it touches enemy
        if (!playerTouchedEnemy)
        {
            //window.drawPlayer(player.getAsset());
            window.draw(window.getRenderer(), player.getAsset(), "assets/player.png");
        }


        window.draw(window.getRenderer(), enemy.getAsset(), "assets/enemy.png");
        window.present();
        
        }

    window.~Window();

    return EXIT_SUCCESS;
}
