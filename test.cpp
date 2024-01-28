#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include "player.hpp"
#include "window.hpp"

using namespace std;

void clear(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 204, 144, 64, 255);
    SDL_RenderClear(renderer);
} 

void draw(SDL_Renderer *renderer, SDL_Rect *rect)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, rect);
    SDL_RenderPresent(renderer);
}

const int WIDTH = 820, HEIGHT = 720;

int main(int argc, char *argv[])
{
    SDL_Surface *imageSurface = NULL;
    SDL_Surface *windowSurface = NULL; 


    SDL_Init(SDL_INIT_EVERYTHING);
    
    SDL_Window *window = SDL_CreateWindow("Reševanje bikca Ferdinanda", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    //windowSurface = SDL_GetWindowSurface(window); 

    if (window == NULL)
        cout << "Could not create window: " << SDL_GetError() << "\n";
        
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 204, 144, 64, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        cout << "Could not initiliaze SDL_image: " << IMG_GetError() << "\n";
        return 1;
    }

    imageSurface = IMG_Load("hero.png"); // ime png
    if (imageSurface == NULL)
        cout << "SDL could not load image! SDL Error: " << SDL_GetError() << "\n";

    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);

    SDL_Event event;
    SDL_Rect rectangle = {410, 360, 50, 50}; // x upper left corner, y; width, height 
    SDL_Rect rectangle2 = {210, 160, 50, 50}; // x upper left corner, y; width, height 
    
    // create player
    Player player = Player(3, rectangle, 300, 300);
    Player player2 = Player(3, rectangle2, 300, 300);

    cout << player.getHealth();

    while (true)
    {
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
            {
                break;
            }

            SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
            SDL_UpdateWindowSurface(window); 
 
            if (SDL_KEYDOWN == event.type)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_a:
                        player.setX(-10);
                        player2.setX(-10);
                        break;
                    case SDLK_d:
                        player.setX(10);
                        player2.setX(10);
                        break;
                    case SDLK_w:
                        player.setY(-10);
                        player2.setY(-10);
                        break;
                    case SDLK_s:
                        player.setY(10);
                        player2.setY(10);
                        break;
                }
            }
        }

        clear(renderer); //clears the rect
        draw(renderer, player.getAsset()); // draws rect
        draw(renderer, player2.getAsset()); // draws rect
        
      /*   SDL_RenderCopy(renderer, imageTexture, NULL, NULL);
        SDL_RenderPresent(renderer); */
    }

    SDL_FreeSurface(imageSurface);
    SDL_FreeSurface(windowSurface);
    
    imageSurface = NULL;
    windowSurface = NULL;

    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}