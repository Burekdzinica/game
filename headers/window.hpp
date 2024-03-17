#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>

#include "arena.hpp"
#include "player.hpp"
#include "gameSettings.hpp"

using namespace std;

const extern int WIDTH, HEIGHT;

class Window
{
    private:  
        TTF_Font* font;
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        static SDL_Rect healthRect;

    public:
        Window();
        Window(const string& title, int WIDTH, int HEIGHT, const char* fontName, int fontSize);
        ~Window(); //destructor
        void init();
        static void createText(const char* textString, int x, int y);
        void clear();
        void present();
        SDL_Renderer* getRenderer();
        void draw(SDL_Renderer* renderer, SDL_Rect destRec, SDL_Texture *imgTexture);
        static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
        static void draw(SDL_Texture* tex, SDL_Rect dest); // if no srcRect
        void drawAnimation(SDL_Renderer* renderer, SDL_Rect srcRect, SDL_Rect destRect, SDL_Texture* imgTexture, SDL_RendererFlip flip);
        static void drawPlayerHealth(int playerHealth, SDL_Texture* hearts_1, SDL_Texture* hearts_2, SDL_Texture* hearts_3);
};

SDL_Rect Window::healthRect = {1, 1, 279, 66};

Window::Window()
{
    init();
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Reševanje bikca Ferdinanda", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GameSettings::WIDTH, GameSettings::HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL)
        cout << "Could not create window: " << SDL_GetError() << "\n";

    renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
        cout << "Could not create renderer: " << SDL_GetError() << "\n";

    this->font = TTF_OpenFont("fonts/pixel.ttf", 30);
    if (!font)
        cout << "Font initilization failed " << TTF_GetError() << "\n"; 

    Data::window = window;
    Data::renderer = renderer;
    Data::font = this->font;

    init();
}

Window::Window(const string& title, int WIDTH, int HEIGHT, const char* fontName, int fontSize)
{
    init();
    SDL_Init(SDL_INIT_EVERYTHING);

    if (window == NULL)
        cout << "Could not create window: " << SDL_GetError() << "\n";

    renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        cout << "Could not create renderer: " << SDL_GetError() << "\n";

    this->font = TTF_OpenFont(fontName, fontSize);
    if (!font)
        cout << "Font initilization failed " << TTF_GetError() << "\n"; 

}

//deletes the window/renderer
Window::~Window() 
{  
    TTF_CloseFont(font);
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
        cout << "TTF initilization failed: " << TTF_GetError() << "\n";

    if(IMG_Init(IMG_INIT_PNG) == 0)
        cout << "IMG initialization failed: " << IMG_GetError() << "\n";
}

void Window::createText(const char* textString, int x, int y)
{
    SDL_Color color = {230, 230, 230};

    SDL_Surface* textSurface = TTF_RenderText_Solid(Data::font, textString, color);
    if (!textSurface)
        cout << "Text surface initilization failed: " << TTF_GetError() << "\n";
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Data::renderer, textSurface);
    SDL_FreeSurface(textSurface);

    int textWidth, textHeight;

    TTF_SizeText(Data::font, textString, &textWidth, &textHeight);

    SDL_Rect textRect = {abs(x - textWidth), y , textWidth, textHeight};
    SDL_RenderCopy(Data::renderer, textTexture, NULL, &textRect);

}

void Window::clear()
{
    SDL_RenderClear(renderer);
    // SDL_RenderCopy(renderer, imgTexture, NULL, NULL);
}

void Window::present()
{
    SDL_RenderPresent(renderer);
}

SDL_Renderer* Window::getRenderer()
{
    return renderer;
}

void Window::draw(SDL_Renderer* renderer, SDL_Rect destRect, SDL_Texture *imgTexture)
{
    if (imgTexture == NULL)
        cout << "Cannot create texture\n";

    SDL_RenderCopy(renderer, imgTexture, NULL, &destRect);
}

void Window::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(Data::renderer, tex, &src, &dest);
}
void Window::draw(SDL_Texture* tex, SDL_Rect dest)
{
    SDL_RenderCopy(Data::renderer, tex, NULL, &dest);
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
            Window::draw(hearts_1, healthRect);
            break;
        case 2:
            Window::draw(hearts_2, healthRect);
            break;
        case 1:
            Window::draw(hearts_3, healthRect);
            break;
    }
}

//     SDL_SetRenderDrawColor(renderer, 204, 144, 64, 255);

#endif 