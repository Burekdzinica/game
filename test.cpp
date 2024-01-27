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
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Reševanje bikca Ferdinanda", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL)
    {
        cout << "Could not create window: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        cout << "Could not initialize SDL_image: " << IMG_GetError() << "\n";
        return 1;
    }

    SDL_Surface *imageSurface = IMG_Load("hero.png");

    if (imageSurface == NULL)
    {
        cout << "SDL could not load image! SDL Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);

    if (imageTexture == NULL)
    {
        cout << "SDL could not create texture! SDL Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_FreeSurface(imageSurface); // Free the surface, as we have the texture now

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

        // Clear the renderer
        clear(renderer);

        // Draw the image and rectangle separately using the renderer
        SDL_RenderCopy(renderer, imageTexture, NULL, NULL);
        draw(renderer, &rectangle);

        // Present the renderer
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
