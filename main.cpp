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
    Arena arena(3, {rand()% WIDTH, rand()% HEIGHT, 200, 200});

    Player player(3, {500, 100, 180, 216});
    Enemy enemy({rand()% WIDTH, rand()% HEIGHT, 225, 225});

    Text text;

    SDL_Rect healthRect = {1, 0, 588, 102};

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

        cout << player.getAsset().x << " " << player.getAsset().y << "\n";

        if (arena.isPlayerNearby(player.getAsset(), arena.getAsset(), 100))
            cout << "Player is nearby \n"; 
        
        if (arena.isPlayerTouching(player.getAsset()))
        {
            cout << "Player is touching arena \n";
        }

        // collision detected
        if (enemy.isPlayerTouching(player.getAsset()))
        {
            enemy.setX(rand()%400);
            enemy.setY(rand()%540);
            player.changeHealth(-1);
            points += 100;
        }

        window.clear();
        
        //draws arena if player is near and deletes it when player touches arena
        if (arena.isPlayerNearby(player.getAsset(), arena.getAsset(), 100) && !(arena.getLvlDone()))
        {
            window.draw(window.getRenderer(), arena.getAsset(), "assets/arena.png");
            cout << "Press E to rescue bulls\n";

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

        window.draw(window.getRenderer(), player.getAsset(), "assets/player.png");
        
        window.draw(window.getRenderer(), enemy.getAsset(), "assets/enemy.png");

        text.createText(window.getRenderer(), textString, WIDTH - text.getTextWidth(), 0);
        switch (player.getHealth())
        {
            case 3:
                window.draw(window.getRenderer(), healthRect, "assets/3_hearts.png");
                break;
            case 2:
                window.draw(window.getRenderer(), healthRect, "assets/2_hearts.png");
                break;
            case 1:
                window.draw(window.getRenderer(), healthRect, "assets/1_hearts.png");
                break;
        }

        window.present();
        
    }

    text.~Text();
    window.~Window();

    return EXIT_SUCCESS;
}
