#include <iostream>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "headers/player.hpp"
#include "headers/window.hpp"
#include "headers/arena.hpp"
#include "headers/enemy.hpp"
#include "headers/text.hpp"
#include "headers/ladder.hpp"

using namespace std;

const int WIDTH = 1700, HEIGHT = 820;

int points = 0;

int main(int argc, char *argv[])
{
    srand(time(NULL));

    Window window("Reševanje bikca Ferdinanda", WIDTH, HEIGHT);
    Arena arena(3, {rand()% WIDTH, rand()% HEIGHT, 200, 200});

    Player player(3, {500, 100, 180, 216});
    Enemy enemy({rand()% WIDTH, rand()% HEIGHT, 274, 208});

    Text text;

    Ladder ladder({rand() % WIDTH, rand() % HEIGHT, 150, 150});
    Level level;

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
                player.movePlayer(event.key.keysym.sym);    
        }

        cout << player.getAsset().x << " " << player.getAsset().y << "\n";

        if (player.isNearby(player.getAsset(), arena.getAsset(), 100))
            cout << "Player is nearby \n"; 
        
        if (arena.isPlayerTouching(player.getAsset()))
        {
            cout << "Player is touching arena \n";
        }

        window.clear();

        // collision detected
        if (enemy.isPlayerTouching(player.getAsset()))
        {
            enemy.setX(rand()%400);
            enemy.setY(rand()%540);
            player.changeHealth(-1);
            points += 100;
        }
        
        //draws arena if player is near and deletes it when player touches arena
        if (player.isNearby(player.getAsset(), arena.getAsset(), 1500) && !(arena.getLvlDone()))
        {
            window.draw(window.getRenderer(), arena.getAsset(), "assets/arena.png");

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_e)
                {
                    arena.changeArenaCounter();
                    arena.setX(rand() % 1000);
                    arena.setY(rand() % 820);
                    points += 100;
                }
            }
        }

        //spawns door for next lvl
        if (arena.getLvlDone())
        {
            window.draw(window.getRenderer(), ladder.getAsset(), "assets/ladder.png");

            if (player.isNearby(player.getAsset(), ladder.getAsset(), 250))
            {
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_e)
                    {
                        level.setLevel();
                    }
                }
            }
        }

        window.draw(window.getRenderer(), player.getAsset(), "assets/player.png");
        window.draw(window.getRenderer(), enemy.getAsset(), "assets/enemy.png");

        text.createText(window.getRenderer(), ("Level: " + to_string(level.getLevel())).c_str(), WIDTH / 2, 0);
        text.createText(window.getRenderer(), ("Points: " + to_string(points)).c_str(), WIDTH, 0);

        window.drawPlayerHealth(player.getHealth());

        window.present();  
    }

    text.~Text();
    window.~Window();

    return EXIT_SUCCESS;
}
