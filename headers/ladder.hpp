#ifndef LADDER_HPP
#define LADDER_HPP

#include <SDL2/SDL.h>
#include <time.h>
#include <vector>
#include <unordered_map>

#include "player.hpp"
#include "gameSettings.hpp"
#include "enemy.hpp"
#include "arena.hpp"

using namespace std;

extern const int ENEMY_WIDTH, ENEMY_HEIGHT;
extern const int ARENA_WIDTH, ARENA_HEIGHT;
extern const int PLAYER_WIDTH, PLAYER_HEIGHT;

extern const int MIN_DISTANCE_BETWEEN_ARENAS;

struct PairHash
{
    template <class T1, class T2>
    size_t operator () (const pair<T1, T2> &p) const
    {
        auto h1 = hash<T1> {} (p.first);
        auto h2 = hash<T2> {} (p.second);

        return h1 ^ h2;
    }
};

class Ladder 
{
    private:
        SDL_Rect asset;

    public:
        Ladder() = default;
        Ladder(const SDL_Rect& asset);
        SDL_Rect getAsset();
        void setX(int x);
        void setY(int y);

};

Ladder::Ladder(const SDL_Rect& asset)
{
    this->asset = asset;
}

SDL_Rect Ladder::getAsset()
{
    return this->asset;
}

void Ladder::setX(int x)
{
    this->asset.x  = x;
}

void Ladder::setY(int y)
{
    this->asset.y  = y;
}

#endif