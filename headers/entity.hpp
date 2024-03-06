#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL2/SDL.h>

using namespace std;

class Entity
{
    protected:
        SDL_Rect asset;

    public:
        SDL_Rect getAsset();
        void setX(int newX);
        void setY(int newY);

};

SDL_Rect Entity::getAsset()
{
    return this->asset;
}

void Entity::setX(int newX)
{
    this->asset.x = newX;
}

void Entity::setY(int newY)
{
    this->asset.y = newY;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class EntityAnimation : public Entity
{
    protected:
        SDL_Rect srcRect;
        SDL_RendererFlip flip;
    
    public:
        void setSrcRect(int x, int y, int w, int h, int frame, int speed);
        SDL_Rect getSrcRect();
        void setFlip(SDL_RendererFlip newflip);
        SDL_RendererFlip getFlip();

};

void EntityAnimation::setSrcRect(int x, int y, int w, int h, int frames, int speed)
{
    this->srcRect.y = y;
    this->srcRect.w = w;
    this->srcRect.h = h;
    this->srcRect.x = srcRect.w * (static_cast<int> ((SDL_GetTicks() / speed) % frames));
}

SDL_Rect EntityAnimation::getSrcRect()
{
    return this->srcRect;
}

SDL_RendererFlip EntityAnimation::getFlip()
{
    return this->flip;
}

void EntityAnimation::setFlip(SDL_RendererFlip newFlip)
{
    this->flip = newFlip;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Menu
{
    protected:
        TTF_Font *font;
        SDL_Color textColor;
        unordered_map<string, TextSize> textCache;

    public:
        Menu();
        void createText(SDL_Renderer *renderer, const char* textString, SDL_Rect &destRect);
        virtual void handleMouseClick(SDL_Renderer *renderer);
};

Menu::Menu()
{
    this->font = TTF_OpenFont("fonts/pixel.ttf", 30);
    this->textColor = {255, 255, 255};
}

void Menu::createText(SDL_Renderer *renderer, const char* textString, SDL_Rect &destRect)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(this->font, textString, textColor);
    if (textSurface == NULL) 
        cout << "Initialization textSurface failed \n";

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL)
        cout << "Initialization textTexture failed \n";

    TextSize textSize;

    auto cacheIt = textCache.find(textString);
    if(cacheIt == textCache.end())
    {
        TTF_SizeText(this->font, textString, &textSize.width, &textSize.height);
        textCache[textString] = textSize;
    }
    else
        textSize = cacheIt->second;
        
    // TTF_SizeText(this->font, textString, &textSize.width, &textSize.height);

    destRect.w = textSize.width;
    destRect.h = textSize.height;

    destRect.x = (GameSettings::WIDTH - destRect.w) / 2;

    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void Menu::handleMouseClick(SDL_Renderer *renderer)
{

}

#endif