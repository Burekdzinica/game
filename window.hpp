#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>

class Window
{
    private:  
        SDL_Window *window;
        SDL_Renderer *renderer;

    public:
        Window(int width, int height);
        Window();
        ~Window(); //destructor
        void clear();
        void present();
        SDL_Renderer* getRenderer();

};

#endif 
