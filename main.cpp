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
    
    unordered_map <int, Arena> arenaList;

    for (int i=0; i<arenaCounter; i++)
    {
        arenaList.insert({i, Arena({rand() % WIDTH - 200, rand() % HEIGHT - 200, 200, 200})});

        // for (int j=0; j<i; j++)
        // {
        //     SDL_Rect rect1 = arena[i].getAsset();
        //     SDL_Rect rect2 = arena[j].getAsset();

        //     if (SDL_HasIntersection(&rect1, &rect2))
        //     {
        //         arena[i].setX(rand() % WIDTH);
        //         arena[i].setY(rand() % HEIGHT);
        //     }
        //}
    }

    Player player(3, {rand() % WIDTH - 180, rand() % HEIGHT - 216, 180, 216});
    // Enemy enemy({rand()% WIDTH - 274, rand()% HEIGHT - 208, 274, 208});

    Enemy enemy(WIDTH, HEIGHT, 274, 208);

    Text text;

    Ladder ladder({rand() % WIDTH - 150, rand() % HEIGHT - 150, 150, 150});
    Level level;

    window.init();

    int fps = 60;
    int frameDelay = 1000 / fps;
    Uint32 frameStart;
    int frameTime;

    int isCloseTo = -1;

    while (player.getIsPlayerAlive())
    {
        int startLoop = SDL_GetTicks();

        bool isPlayerNearArena = false;

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
            enemy.setX(rand() % WIDTH - enemy.getAsset().w);
            enemy.setY(rand() % HEIGHT - enemy.getAsset().h);
            player.changeHealth(-1);

            points += 100;
        }

        for (const auto& entry : arenaList)
        {
            int i = entry.first;
            const Arena& currentArena = entry.second;

            if (player.isNearby(player.getAsset(), currentArena.getAsset(), 1500)) //&& !(arenaTracker.getLvlDone()))
            {
                window.draw(window.getRenderer(), currentArena.getAsset(), "assets/arena.png");

                isCloseTo = i;

                isPlayerNearArena = true;
            }
        }

        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_e && isCloseTo >= 0)
            {
                arenaList.erase(isCloseTo);

                points += 100;
                
                isCloseTo = -1;
            }
        }
    
        //spawns door for next lvl 
        if (arenaList.empty())
        {
            window.draw(window.getRenderer(), ladder.getAsset(), "assets/ladder.png");

            if (player.isNearby(player.getAsset(), ladder.getAsset(), 250))
            {
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_e)
                    {
                        level.resetGame(player, enemy, arenaList, ladder, isCloseTo, player.getHealth(), WIDTH, HEIGHT);
                        level.setLevel();
                    }
                }
            }
        }

        window.draw(window.getRenderer(), player.getAsset(), "assets/player.png");
        window.draw(window.getRenderer(), enemy.getAsset(), "assets/enemy.png");

        if (arenaList.size() < arenaCounter)
            text.createText(window.getRenderer(), ("Remaining arenas: " + to_string(arenaList.size())).c_str(), WIDTH, 50);
        
        if (isPlayerNearArena)
            text.createText(window.getRenderer(), "Press [E]", WIDTH / 2, HEIGHT - 50);

        if (player.isNearby(player.getAsset(), ladder.getAsset(), 250))
            text.createText(window.getRenderer(), ("Press [E]"), WIDTH / 2, HEIGHT - 50);

        text.createText(window.getRenderer(), ("Level: " + to_string(level.getLevel())).c_str(), WIDTH / 2, 0);
        text.createText(window.getRenderer(), ("Points: " + to_string(points)).c_str(), WIDTH, 0);

        window.drawPlayerHealth(player.getHealth());

        window.present();  

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    text.~Text();
    window.~Window();

    return EXIT_SUCCESS;
}
