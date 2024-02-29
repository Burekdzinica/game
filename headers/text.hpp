#ifndef TEXT_HPP
#define TEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Text 
{
    private:
        TTF_Font* font;

    public:
        Text();
        Text(const char* newFont, int fontSize);
        ~Text();
        void createText(SDL_Renderer *renderer, const char* textString, int x, int y);
};

Text::Text()
{
    this->font = TTF_OpenFont("fonts/test.ttf", 50);
    if (!font)
        cout << "Font initilization failed " << TTF_GetError() << "\n"; 
}

Text::Text(const char* newFont, int fontSize)
{
    this->font = TTF_OpenFont(newFont, fontSize);

    if (!font)
        cout << "Font initilization failed: " << TTF_GetError() << "\n";
}

Text::~Text()
{
    TTF_CloseFont(font);
}

void Text::createText(SDL_Renderer *renderer, const char* textString, int x, int y)
{
    SDL_Color color = {230, 230, 230};

    SDL_Surface* textSurface = TTF_RenderText_Solid(this->font, textString, color);
    if (!textSurface)
        cout << "Text surface initilization failed: " << TTF_GetError() << "\n";
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    int textWidth, textHeight;

    TTF_SizeText(font, textString, &textWidth, &textHeight);

    SDL_Rect textRect = {abs(x - textWidth), y , textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);
}



#endif