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
        // int textWidth;
        // int textHeight;
        const char* textString;

    public:
        ~Text();
        void createText(SDL_Renderer *renderer, const char* textString, int x, int y);
        int getTextWidth();
        int getTextHeigth();

};


Text::~Text()
{
    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
}

void Text::createText(SDL_Renderer *renderer, const char* textString, int x, int y)
{
    font = TTF_OpenFont("fonts/test.ttf", 50);
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

    SDL_Rect textRect = {abs(x - textWidth), y , textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}

// int Text::getTextWidth()
// {
//     return this->textWidth;
// }

// int Text::getTextHeigth()
// {
//     return this->textHeight;
// }

#endif