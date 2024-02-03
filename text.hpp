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
        void createText(string fontType, SDL_Renderer *renderer, const char* textString);

};

Text::~Text()
{
    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
}

void Text::createText(string fontType, SDL_Renderer *renderer, const char* textString)
{
    font = TTF_OpenFont("fonts/test.ttf", 100);
    if(!font)
        cout << "Font initilization failed: " << TTF_GetError() << "\n";
    

    SDL_Color color = {230, 230, 230};

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textString, color);
    if (!textSurface)
        cout << "Text surface initilization failed: " << TTF_GetError();
    

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    int textWidth, textHeight;

    TTF_SizeText(font, textString, &textWidth, &textHeight);


    SDL_Rect textRect = {0, 0, textWidth, textHeight};

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

}

#endif