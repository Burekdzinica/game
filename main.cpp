#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>

#include "player.hpp"
#include "window.hpp"
#include "arena.hpp"
#include "enemy.hpp"

using namespace std;

const int WIDTH = 1700, HEIGHT = 820;

        TTF_Font* font;
        SDL_Texture *texture, *text;

int main(int argc, char *argv[])
{
    srand(time(NULL));

    Window window("Reševanje bikca Ferdinanda", WIDTH, HEIGHT);

    Player player(3, {500, 100, 50, 50});
    Arena arena({rand()%1040, rand()%540, 50, 50});
    Enemy enemy({rand()%1040, rand()%540, 100, 100});

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            cout << "SDL initialization failed: " << SDL_GetError() << "\n";
            return EXIT_FAILURE;
        }

        if(TTF_Init() == 1)
            cout << "Could no initialize SDL2 ttf, error: " << TTF_GetError;
        

        font = TTF_OpenFont("fonts/test.ttf", 200);
        if(!font)
        {
            cout << "ERROR: " << TTF_GetError << "\n";
            return EXIT_FAILURE;
        }


        SDL_Color color = {255, 255, 255};

        const char* textString = "Hello, SDL_ttf!";
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, "I HATE NIGGERS", color);
        if (!textSurface)
        {
            cout << "ERROR" << TTF_GetError;
            return EXIT_FAILURE;
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(window.getRenderer(), textSurface);
        SDL_FreeSurface(textSurface);

        SDL_Rect textRect = {50, 50, 600, 300};

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



        cout << player.getAsset().x << " " << player.getAsset().y << "\n";

        if (arena.isPlayerNearby(player.getAsset(), arena.getAsset(), 100))
            cout << "Player is nearby"; 
        
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
        }

        window.clear();
        SDL_RenderCopy(window.getRenderer(), textTexture, NULL, &textRect);
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


            TTF_CloseFont(font);
            SDL_DestroyTexture(textTexture);
    window.~Window();

    return EXIT_SUCCESS;
}
