#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include <unordered_map>

#include "arena.hpp"
#include "player.hpp"
#include "gameSettings.hpp"

using namespace std;

const extern int WIDTH, HEIGHT;

class Window
{
    private:  
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_Texture *imgTexture;
        SDL_Rect healthRect;

    public:
        Window();
        Window(const string &title, int width, int height);
        ~Window(); //destructor
        void init();
        void clear();
        void present();
        SDL_Renderer* getRenderer();
        void draw(SDL_Renderer* renderer, SDL_Rect destRec, SDL_Texture *imgTexture);
        void drawAnimation(SDL_Renderer *renderer, SDL_Rect srcRect, SDL_Rect destRect, SDL_Texture *imgTexture, SDL_RendererFlip flip);
        void drawPlayerHealth(int playerHealth, SDL_Texture *hearts_1, SDL_Texture *hearts_2, SDL_Texture *hearts_3);
};

Window::Window()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Reševanje bikca Ferdinanda", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GameSettings::WIDTH, GameSettings::HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL)
        cout << "Could not create window: " << SDL_GetError() << "\n";

    renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
        cout << "Could not create renderer: " << SDL_GetError() << "\n";

    imgTexture = IMG_LoadTexture(renderer, "assets/map_second.png");
    if (imgTexture == NULL)
        cout << "Cannot load image";

    this->healthRect = {1, 1, 279, 66};

    Data::window = window;

    init();
}

Window::Window(const string &title, int WIDTH, int HEIGHT)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    if (window == NULL)
        cout << "Could not create window: " << SDL_GetError() << "\n";

    renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        cout << "Could not create renderer: " << SDL_GetError() << "\n";

    imgTexture = IMG_LoadTexture(renderer, "assets/map_second.png");
    if (imgTexture == NULL)
        cout << "Cannot load image";

    this->healthRect = {1, 1, 279, 66};


}

//deletes the window/renderer
Window::~Window() 
{  
    SDL_DestroyTexture(imgTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

//initializes all libraries
void Window::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        cout << "SDL initialization failed: " << SDL_GetError() << "\n";

    if(TTF_Init() == -1)
    if(TTF_Init() == -1)
        cout << "TTF initilization failed: " << TTF_GetError() << "\n";

    if(IMG_Init(IMG_INIT_PNG) == 0)
        cout << "IMG initialization failed: " << IMG_GetError() << "\n";
}

// //clears the screen // used for moving assets
// void Window::clear() 
// {
//     SDL_SetRenderDrawColor(renderer, 204, 144, 64, 255);
//     SDL_RenderClear(renderer);
// }

void Window::clear()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imgTexture, NULL, NULL);
}

void Window::present()
{
    SDL_RenderPresent(renderer);
}

SDL_Renderer* Window::getRenderer()
{
    return renderer;
}

void Window::draw(SDL_Renderer *renderer, SDL_Rect destRect, SDL_Texture *imgTexture)
{
    if (imgTexture == NULL)
        cout << "Cannot create texture\n";

    SDL_RenderCopy(renderer, imgTexture, NULL, &destRect);
}

void Window::drawAnimation(SDL_Renderer *renderer, SDL_Rect srcRect, SDL_Rect destRect, SDL_Texture *imgTexture, SDL_RendererFlip flip)
{
    if (imgTexture == NULL)
        cout << "Cannot create texture\n";

    SDL_RenderCopyEx(renderer, imgTexture, &srcRect, &destRect, 0, NULL, flip);
}

void Window::drawPlayerHealth(int playerHealth, SDL_Texture *hearts_1, SDL_Texture *hearts_2, SDL_Texture *hearts_3)
{
    switch (playerHealth)
    {
        case 3:
            draw(renderer, healthRect, hearts_1);
            break;
        case 2:
            draw(renderer, healthRect, hearts_2);
            break;
        case 1:
            draw(renderer, healthRect, hearts_3);
            break;
    }
}

#endif 