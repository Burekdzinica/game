#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>

using namespace std;

class Window
{
    private:  
        SDL_Window *window;
        SDL_Renderer *renderer;

    public:
        Window(const string &title, int width, int height);
        ~Window(); //destructor
        void clear();
        void present();
        SDL_Renderer* getRenderer();
        void draw(SDL_Rect rect); // for rect
        void draw(SDL_Texture *texture, const SDL_Rect *srcRect, const SDL_Rect &destRect); //for sdl image
};

Window::Window(const string &title, int WIDTH, int HEIGHT)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
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
    SDL_Quit();
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

//draw rectangle
 void Window::draw(SDL_Rect rect)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
} 


//draw png
void Window::draw(SDL_Texture *texture, const SDL_Rect *srcRect, const SDL_Rect &destRect)
{
    SDL_RenderCopy(renderer, texture, srcRect, &destRect);
}   


#endif 
