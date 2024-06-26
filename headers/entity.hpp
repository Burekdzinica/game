#ifndef ENTITY_HPP
#define ENTITY_HPP

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

/**
 * @brief Gets rectangle asset
 * @returns rectangle asset
*/
SDL_Rect Entity::getAsset()
{
    return this->asset;
}

/**
 * @brief Sets the x position
 * @param newX The new x position
*/
void Entity::setX(int newX)
{
    this->asset.x = newX;
}

/**
 * @brief Sets the y position
 * @param newY The new y position
*/
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
        void setFlip(const SDL_RendererFlip& newflip);
        SDL_RendererFlip getFlip();

};

/**
 * @brief Sets src rect for animations(sprite sheet)
 * @param x The x position of source rectangle
 * @param y The y position of source rectangle
 * @param w The width of source rectangle
 * @param h The height of the source rectangle
 * @param frames The frames of specified animation
 * @param speed The animation speed
*/
void EntityAnimation::setSrcRect(int x, int y, int w, int h, int frames, int speed)
{
    this->srcRect.y = y;
    this->srcRect.w = w;
    this->srcRect.h = h;
    this->srcRect.x = srcRect.w * (static_cast<int> ((SDL_GetTicks() / speed) % frames));
}

/**
 * @brief Gets source rectangle
 * @returns Source rectangle
*/
SDL_Rect EntityAnimation::getSrcRect()
{
    return this->srcRect;
}

/**
 * @brief Gets flip
 * @return flip
*/
SDL_RendererFlip EntityAnimation::getFlip()
{
    return this->flip;
}

/**
 * @brief Sets the flip
 * @param newFlip The new flip
*/
void EntityAnimation::setFlip(const SDL_RendererFlip& newFlip)
{
    this->flip = newFlip;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Menu
{
    protected:
        TTF_Font* font;
        SDL_Color textColor;
        unordered_map<string, TextSize> textCache;

    public:
        Menu();
        void createText(SDL_Renderer *renderer, const char* textString, SDL_Rect &destRect);
        virtual void handleMouseClick();
};

/**
 * @brief Default constructor for Menu
*/
Menu::Menu()
{
    this->font = TTF_OpenFont("fonts/pixel.ttf", 30);
    this->textColor = {255, 255, 255};
}

/**
 * @brief
 * @param renderer The renderer
 * @param textString The text to draw
 * @param destRect Rectangle to draw to
*/
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

/**
 * @brief Virtual method
*/
void Menu::handleMouseClick()
{

}

#endif