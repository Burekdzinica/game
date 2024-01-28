#include "window.hpp"

#include <iostream>

using namespace std;

//constructor
Window::Window(int WIDTH, int HEIGHT)
{
    SDL_Window *window = SDL_CreateWindow("Reševanje bikca Ferdinanda", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL)
        cout << "Could not create window: " << SDL_GetError() << "\n";

    renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        cout << "Could not create renderer: " << SDL_GetError() << "\n";
}

//default constructor
Window::Window()
{
    SDL_Window *window = SDL_CreateWindow("Reševanje bikca Ferdinanda", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 720, SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL)
        cout << "Could not create window: " << SDL_GetError() << "\n";


    renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        cout << "Could not create renderer: " << SDL_GetError() << "\n";
}

//deletes the window/renderer
Window::~Window() 
{  
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

//clears the screen // used for moving assets
void Window::clear() 
{
    SDL_SetRenderDrawColor(renderer, 204, 144, 64, 255);
    SDL_RenderClear(renderer);
}

void Window::present()
{
    SDL_RenderPresent(renderer);
}

SDL_Renderer* Window::getRenderer()
{
    return renderer;
}

void Window::draw(SDL_Rect rect)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Window::draw(SDL_Texture *texture, const SDL_Rect *srcRect, const SDL_Rect &destRect)
{
    SDL_RenderCopy(renderer, texture, srcRect, &destRect);
}