#include <iostream>
#include <unordered_map>
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

    int randomArenaCounter = rand() % 2;
    int arenaCounter = 3 + randomArenaCounter;

    Arena arena[arenaCounter];
    Arena arenaTracker(arenaCounter, {200, 200, 200, 200});

    unordered_map <int, Arena> hashmap;

    for (int i=0; i<arenaCounter; i++)
    {
        arena[i] = Arena(arenaCounter, {rand() % WIDTH, rand() % HEIGHT, 200, 200});
        hashmap.insert({i, Arena(arenaCounter, {rand() % WIDTH, rand() % HEIGHT, 200, 200})});

        for (int j=0; j<i; j++)
        {
            SDL_Rect rect1 = arena[i].getAsset();
            SDL_Rect rect2 = arena[j].getAsset();

            if (SDL_HasIntersection(&rect1, &rect2))
            {
                arena[i].setX(rand() % WIDTH);
                arena[i].setY(rand() % HEIGHT);
            }
        }
    }

    Player player(3, {500, 100, 180, 216});
    Enemy enemy({rand()% WIDTH, rand()% HEIGHT, 274, 208});

    Text text;

    Ladder ladder({rand() % WIDTH, rand() % HEIGHT, 150, 150});
    Level level;

    window.init();

    int fps = 60;
    int desiredDelta = 1000 / fps;
    int isCloseTo = -1;

    while (player.getIsPlayerAlive())
    {
        int startLoop = SDL_GetTicks();

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

        // cout << player.getAsset().x << " " << player.getAsset().y << "\n";

        // if (player.isNearby(player.getAsset(), arena.getAsset(), 100))
        //     cout << "Player is nearby \n"; 
        
        // if (arena.isPlayerTouching(player.getAsset()))
        // {
        //     cout << "Player is touching arena \n";
        // }


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
        for (int i = 0; i < arenaTracker.getArenaCounter(); i++)
        {
            if (player.isNearby(player.getAsset(), arena[i].getAsset(), 1500) && !(arenaTracker.getLvlDone()) && arena[i].getArenaSpawn() != true)
            {
                window.draw(window.getRenderer(), arena[i].getAsset(), "assets/arena.png");

                isCloseTo = i;
            }
        }

        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_e && isCloseTo >= 0)
            {
                cout << isCloseTo << "\n";
                arena[isCloseTo].setArenaSpawn();

                arenaTracker.changeArenaCounter();
                points += 100;
                
                isCloseTo = -1;
            }
        }
    
        //spawns door for next lvl 
        if (arenaTracker.getLvlDone())
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

        text.createText(window.getRenderer(), "Press [E]", WIDTH / 2, HEIGHT - 50);

        if (arenaTracker.getArenaCounter() != arenaCounter)
            text.createText(window.getRenderer(), ("Remaining arenas: " + to_string(arenaTracker.getArenaCounter())).c_str(), WIDTH, 50);

        text.createText(window.getRenderer(), ("Level: " + to_string(level.getLevel())).c_str(), WIDTH / 2, 0);
        text.createText(window.getRenderer(), ("Points: " + to_string(points)).c_str(), WIDTH, 0);

        window.drawPlayerHealth(player.getHealth());

        window.present();  

        int delta = SDL_GetTicks() - startLoop;

        if (delta < desiredDelta)
            SDL_Delay(desiredDelta - delta);
    }

    text.~Text();
    window.~Window();

    return EXIT_SUCCESS;
}
