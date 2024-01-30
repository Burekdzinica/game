#ifndef TEXT_HPP
#define TEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Text 
{
    private:
        TTF_Font* font;
        SDL_Texture* texture;

    public:
        ~Text();

};

Text::~Text()
{
    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
}

#endif