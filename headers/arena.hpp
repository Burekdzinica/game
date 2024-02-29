#ifndef ARENA_HPP
#define ARENA_HPP

#include <SDL2/SDL.h>

using namespace std;

class Arena 
{
    private:
        SDL_Rect asset;
        bool isLvlDone;
        bool didArenaSpawn;
        bool isVisible;
        bool forcedVisibilty;

    public:
        Arena();
        Arena(SDL_Rect asset);
        bool getArenaSpawn() const;
        int getArenaCounter();
        bool getLvlDone();
        SDL_Rect getAsset() const;
        bool getVisible() const;
        bool isForcedVisible();
        bool isPlayerTouching(const SDL_Rect& player);
        void setForcedVisibility(bool newForcedVisibility);
        void setArenaSpawn();
        void setVisible(bool newVisible);
        void setX(int newX);
        void setY(int newY);

};

Arena::Arena()
{
    this->asset = {0, 0, 0, 0};  
    this->isLvlDone = false;
    this->didArenaSpawn = false;
    this->isVisible = true;
}

Arena::Arena(SDL_Rect asset)
{
    this->asset = asset;
    this->isLvlDone = false;
    this->isVisible = false;
    this->forcedVisibilty = false; 
}

bool Arena::getArenaSpawn() const
{
    return this->didArenaSpawn;
}

bool Arena::getLvlDone()
{
    return this->isLvlDone;
}

SDL_Rect Arena::getAsset() const
{
    return this->asset;
}

bool Arena::getVisible() const
{
    return this->isVisible;
}

bool Arena::isForcedVisible()
{
    return this->forcedVisibilty;
}

bool Arena::isPlayerTouching(const SDL_Rect& player)
{
    return SDL_HasIntersection(&player, &asset) == SDL_TRUE;
}

void Arena::setForcedVisibility(bool newForcedVisibility)
{
    this->forcedVisibilty = newForcedVisibility;
}

void Arena::setArenaSpawn()
{
    this->didArenaSpawn = true;
}

void Arena::setVisible(bool newVisible)
{
    this->isVisible = newVisible;
}

void Arena::setX(int newX)
{
    this->asset.x = newX;
}

void Arena::setY(int newY)
{
    this->asset.y = newY;
}




#endif