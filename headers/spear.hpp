#ifndef SPEAR_HPP
#define SPEAR_HPP

#include <SDL2/SDL.h>
#include <unordered_map>

#include "entity.hpp"
#include "ladder.hpp"

using namespace std;

const int SPEAR_WIDTH = 83, SPEAR_HIGHT = 16;

class Spear : public Entity
{
    private:
        bool spearTouched;

    public:
        Spear();
        ~Spear();
        void spawnSpear(unordered_map<pair<int, int>, bool, PairHash>& grid);
        bool isPlayerTouching(const SDL_Rect& player);
        void setSpearTouched();
        bool isSpearTouched();
};

Spear::Spear()
{
    this->asset = {0, 0, SPEAR_WIDTH, SPEAR_HIGHT};
    this->spearTouched = false;
}

Spear::~Spear()
{

}

void Spear::spawnSpear(unordered_map<pair<int, int>, bool, PairHash>& grid)
{
    int xSpear, ySpear;
    do
    {
        auto it = begin(grid);
        advance(it, rand() % grid.size());
        pair<int, int> randomCell = it->first;

        xSpear = randomCell.first * ENEMY_WIDTH;
        ySpear = randomCell.second * ENEMY_HEIGHT;

        if (!grid[randomCell])
        {
            grid[randomCell] = true;
            break;
        }
    } while(true);

    this->asset.x = xSpear;
    this->asset.y = ySpear;
}

bool Spear::isPlayerTouching(const SDL_Rect& player)
{
    return SDL_HasIntersection(&player, &asset) == SDL_TRUE;
}

void Spear::setSpearTouched()
{
    this->spearTouched = true;
}

bool Spear::isSpearTouched()
{
    return this->spearTouched;
}

#endif