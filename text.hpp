#ifndef TEXT_HPP
#define TEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Text 
{
    private:
        TTF_Font* font;
        SDL_Texture* texture;
        string fontType;

    public:
        ~Text();
        void createText(string fontType, SDL_Renderer *renderer);

};

Text::~Text()
{
    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
}

void Text::createText(string fontType, SDL_Renderer *renderer)
{
    font = TTF_OpenFont("fonts/test.ttf", 200);
    if(!font)
    {
        cout << "Font initilization failed: " << TTF_GetError << "\n";
    }

    SDL_Color color = {0, 0, 0};

    const char* textString = "Hello, SDL_ttf!";
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textString, color);
    if (!textSurface)
    {
        cout << "Text surface initilization failed: " << TTF_GetError;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect textRect = {50, 50, 600, 300};

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

}

#endif