#ifndef LADDER_HPP
#define LADDER_HPP

#include <SDL2/SDL.h>

using namespace std;

class Ladder 
{
    private:
        SDL_Rect asset;

    public:
        Ladder(SDL_Rect asset);
        SDL_Rect getAsset();

};

Ladder::Ladder(SDL_Rect asset)
{
    this->asset = asset;
}

SDL_Rect Ladder::getAsset()
{
    return this->asset;
}

class Level
{
    private:
        int lvl;

    public:
        Level();
        int getLevel();
        void setLevel();
};

Level::Level()
{
    this->lvl = 1;
}

int Level::getLevel()
{
    return this->lvl;
}

void Level::setLevel()
{
    this->lvl++;
}

#endif 