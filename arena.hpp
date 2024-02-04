#ifndef ARENA_HPP
#define ARENA_HPP

#include <SDL2/SDL.h>

using namespace std;

class Arena 
{
    private:
        SDL_Rect asset;
        int arenaCounter;
        bool isLvlDone;

    public:
        Arena(int arenaCounter, SDL_Rect asset);
        int getArenaCounter();
        bool getLvlDone();
        void changeArenaCounter();
        SDL_Rect getAsset();
        bool isPlayerTouching(const SDL_Rect& player);
        void setX(int newX);
        void setY(int newY);

};

Arena::Arena(int arenaCounter, SDL_Rect asset)
{
    this->arenaCounter = arenaCounter;
    this->asset = asset;
    this->isLvlDone = false;
}

int Arena::getArenaCounter()
{
    return this->arenaCounter;
}

bool Arena::getLvlDone()
{
    return this->isLvlDone;
}

void Arena::changeArenaCounter()
{
    this->arenaCounter--;
    this->isLvlDone = this->arenaCounter <= 0;
}

SDL_Rect Arena::getAsset()
{
    return this->asset;
}

bool Arena::isPlayerTouching(const SDL_Rect& player)
{
    return SDL_HasIntersection(&player, &asset) == SDL_TRUE;
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