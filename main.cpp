#include <iostream>
#include <unordered_map>
#include <time.h>
#include <chrono>
#include <thread>
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

    unordered_map <int, Arena> arenaList;

    Level level;
    level.setArenaCounter(3 + rand() % 2);

    for (int i=0; i < level.getArenaCounter(); i++)
    {
        arenaList.insert({i, Arena({max((rand() % WIDTH - 200), 0), max((rand() % HEIGHT - 200), 0), 200, 200})});
    }

    Player player(3, {max((rand() % WIDTH - 180), 0), max((rand() % HEIGHT - 216), 0), 180, 216});
    Enemy enemy({max((rand()% WIDTH - 274), 0), max((rand()% HEIGHT - 208), 0), 274, 208});

    Text text;

    Ladder ladder({max((rand() % WIDTH - 150), 0), max((rand() % HEIGHT - 150), 0), 150, 150});

    window.init();

    int fps = 60;
    int frameDelay = 1000 / fps;
    Uint32 frameStart;
    int frameTime;

    int isCloseTo = -1;

    bool isPlayerNearArena;



    while (player.getIsPlayerAlive())
    {
        int startLoop = SDL_GetTicks();

        isPlayerNearArena =false;

        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
                break;
            if (event.type == SDL_KEYDOWN) 
                player.movePlayer(event.key.keysym.sym, WIDTH, HEIGHT); 
            else if (event.type == SDL_KEYUP)
                player.setIsMoving(false);
        }

        // cout << player.getAsset().x << " " << player.getAsset().y << "\n";
        // cout << enemy.getAsset().x << " " << enemy.getAsset().y << "\n";

        // if (player.isNearby(player.getAsset(), arena.getAsset(), 100))
        //     cout << "Player is nearby \n"; 

        // if (arena.isPlayerTouching(player.getAsset()))
        // {
        //     cout << "Player is touching arena \n";
        // }

        window.clear();

        for (const auto& entry : arenaList)
        {
            const Arena& currentArena = entry.second;

            if (player.isNearby(player.getAsset(), currentArena.getAsset(), 1200))
            {
                window.draw(window.getRenderer(), currentArena.getAsset(), "assets/arena.png");

                isCloseTo = entry.first;

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

        enemy.updateEnemyAI(player, 500);
        player.updatePlayerAnimation(200);

        player.draw(window.getRenderer(), player.getAsset(), "assets/player_remastared.png", player.getFlip());
        // window.drawAnimation(window.getRenderer(), player.getSrcRect(), player.getAsset(), "assets/player_remastared.png", player.getFlip());
        window.draw(window.getRenderer(), enemy.getAsset(), "assets/enemy.png");

        if (arenaList.size() < level.getArenaCounter())
            text.createText(window.getRenderer(), ("Remaining arenas: " + to_string(arenaList.size())).c_str(), WIDTH, 50);
        
        if (isPlayerNearArena)
            text.createText(window.getRenderer(), "Press [E]", WIDTH / 2, HEIGHT - 50);

        if (player.isNearby(player.getAsset(), ladder.getAsset(), 250))
            text.createText(window.getRenderer(), ("Press [E]"), WIDTH / 2, HEIGHT - 50);

        text.createText(window.getRenderer(), ("Level: " + to_string(level.getLevel())).c_str(), WIDTH / 2, 0);
        text.createText(window.getRenderer(), ("Points: " + to_string(points)).c_str(), WIDTH, 0);

        window.drawPlayerHealth(player.getHealth());


        //Game over
        if (!(player.getIsPlayerAlive()))
        {
            SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 0);
            SDL_RenderClear(window.getRenderer());

            text.createText(window.getRenderer(), "Game over", WIDTH / 2, HEIGHT / 2);

            window.present();

            SDL_Delay(1000);
        }

        window.present();  

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    text.~Text();
    window.~Window();

    return EXIT_SUCCESS;
}
