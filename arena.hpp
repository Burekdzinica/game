#ifndef ARENA_HPP
#define ARENA_HPP

#include <SDL2/SDL.h>

using namespace std;

class Arena 
{
    private:
        SDL_Rect asset;

    public:
        Arena(SDL_Rect asset);
        SDL_Rect getAsset();
        bool isPlayerNearby(SDL_Rect player, SDL_Rect arena, int range);
        bool isPlayerTouching(const SDL_Rect& player);

};

Arena::Arena(SDL_Rect asset)
{
    this->asset = asset;
}

SDL_Rect Arena::getAsset()
{
    return this->asset;
}

bool Arena::isPlayerNearby(SDL_Rect player, SDL_Rect arena, int range)
{
    SDL_Rect near;
    near.x = abs(player.x - arena.x);
    near.y = abs(player.y - arena.y);

    if (near.x < range && near.y < range)
        return true;
   
    return false;  
}

bool Arena::isPlayerTouching(const SDL_Rect& player)
{
    return SDL_HasIntersection(&player, &asset) == SDL_TRUE;
}



#endif