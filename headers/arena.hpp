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

    public:
        Arena();
        Arena(SDL_Rect asset);
        bool getArenaSpawn() const;
        int getArenaCounter();
        bool getLvlDone();
        void changeArenaCounter();
        SDL_Rect getAsset() const;
        bool isPlayerTouching(const SDL_Rect& player);
        void setArenaSpawn();
        void setX(int newX);
        void setY(int newY);

};

Arena::Arena()
{
    this->asset = {0, 0, 0, 0};  
    this->isLvlDone = false;
    this->didArenaSpawn = false;
}

Arena::Arena(SDL_Rect asset)
{
    this->asset = asset;
    this->isLvlDone = false;
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

bool Arena::isPlayerTouching(const SDL_Rect& player)
{
    return SDL_HasIntersection(&player, &asset) == SDL_TRUE;
}

void Arena::setArenaSpawn()
{
    this->didArenaSpawn = true;
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