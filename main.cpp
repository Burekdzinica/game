#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 


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

const int WIDTH = 800, HEIGHT = 700;

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
    SDL_Rect rectangle = {10, 10, 50, 50}; // x upper left corner, y; width, height 

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
                        rectangle.x -= 10;
                        break;
                    case SDLK_d:
                        rectangle.x += 10;
                        break;
                    case SDLK_w:
                        rectangle.y -= 10;
                        break;
                    case SDLK_s:
                        rectangle.y += 10;
                        break;
                }
            }
        }


        clear(renderer); //clears the rect
        draw(renderer, &rectangle); // draws rect
        
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