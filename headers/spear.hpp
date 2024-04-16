#ifndef SPEAR_HPP
#define SPEAR_HPP

#include "entity.hpp"

using namespace std;

const int SPEAR_WIDTH = 83, SPEAR_HIGHT = 16;

class Spear : public Entity
{
    private:
        bool spearTouched;

    public:
        Spear();
        ~Spear();
        void spawnSpear(Grid_t& grid);
        bool isPlayerTouching(const SDL_Rect& player);
        void setSpearTouched();
        bool isSpearTouched();
};

/**
 * @brief Default constructor for Spear
*/
Spear::Spear()
{
    this->asset = {0, 0, SPEAR_WIDTH, SPEAR_HIGHT};
    this->spearTouched = false;
}

/**
 * @brief Destructor for Spear
*/
Spear::~Spear()
{
   
}

/**
 * @brief Spawns the spear
 * @param grid The grid for spawn
*/
void Spear::spawnSpear(Grid_t& grid)
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

/**
 * @brief True if player is touching spear
 * @param player The object for intersection
 * @return True or false
*/
bool Spear::isPlayerTouching(const SDL_Rect& player)
{
    return SDL_HasIntersection(&player, &asset) == SDL_TRUE;
}

/**
 * @brief Sets true if spear was touched
*/
void Spear::setSpearTouched()
{
    this->spearTouched = true;
}

/**
 * @brief Returns spearTouched
 * @returns True or false
*/
bool Spear::isSpearTouched()
{
    return this->spearTouched;
}

#endif