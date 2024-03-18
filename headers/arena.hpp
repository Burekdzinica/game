#ifndef ARENA_HPP
#define ARENA_HPP

#include <SDL2/SDL.h>
#include <unordered_map>

#include "ladder.hpp"

using namespace std;

class Arena : public Entity
{
    private:
        bool isLvlDone;
        bool didArenaSpawn;
        bool isVisible;
        bool forcedVisibilty;

    public:
        Arena();
        Arena(const SDL_Rect& asset);
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
        static void generateArenaPositions(int counter, unordered_map<pair<int, int>, bool, PairHash> &grid, unordered_map<int, Arena> &arenaList);
};

/**
 * @brief Default constructor for Arena
*/
Arena::Arena()
{
    this->asset = {0, 0, 0, 0};  
    this->isLvlDone = false;
    this->didArenaSpawn = false;
    this->isVisible = true;
}

/**
 * @brief Copy constructor for Arena
 * @param asset The other Arena
*/
Arena::Arena(const SDL_Rect& asset)
{
    this->asset = asset;
    this->isLvlDone = false;
    this->isVisible = false;
    this->forcedVisibilty = false; 
}

/**
 * @brief Returns if arena spawned
 * @return True or False
*/
bool Arena::getArenaSpawn() const
{
    return this->didArenaSpawn;
}

/**
 * @brief Returns if level is done
 * @return True or False
*/
bool Arena::getLvlDone()
{
    return this->isLvlDone;
}

/**
 * @brief Gets the asset
 * @return Asset
*/
SDL_Rect Arena::getAsset() const
{
    return this->asset;
}

/**
 * @brief Returns if arena is visible
 * @return True or False
*/
bool Arena::getVisible() const
{
    return this->isVisible;
}

/**
 * @brief Returns if arena is forced visible
 * @return True or False
*/
bool Arena::isForcedVisible()
{
    return this->forcedVisibilty;
}

/**
 * @brief Returns if player is touching arena
 * @param player The player asset
 * @return True or False
*/
bool Arena::isPlayerTouching(const SDL_Rect& player)
{
    return SDL_HasIntersection(&player, &asset) == SDL_TRUE;
}

/**
 * @brief Sets forced visible
 * @param newForcedVisibility The new forced visibility
*/
void Arena::setForcedVisibility(bool newForcedVisibility)
{
    this->forcedVisibilty = newForcedVisibility;
}

/**
 * @brief Sets if arena spawned
*/
void Arena::setArenaSpawn()
{
    this->didArenaSpawn = true;
}

/**
 * @brief Sets visible
 * @param newVisible The new visible
*/
void Arena::setVisible(bool newVisible)
{
    this->isVisible = newVisible;
}

/**
 * @brief Generates arena positions based on grid
 * @param counter The number of arenas
 * @param grid The grid for spawns
 * @param arenaList The arenaList
*/
void Arena::generateArenaPositions(int counter, unordered_map<pair<int, int>, bool, PairHash>& grid, unordered_map<int, Arena>& arenaList)
{
    int xArena, yArena;
    for (int i=0; i < counter; i++)
    {
        int attempts = 0;
        do
        {
            auto it = begin(grid);
            advance(it, rand() % grid.size());
            pair<int, int> randomCell = it->first;

            xArena = randomCell.first * ARENA_WIDTH;
            yArena = randomCell.second * ARENA_HEIGHT;

            bool tooCloseToExistingArena = false;
            for (const auto& entry : arenaList)
            {
                int distanceX = abs(xArena - entry.second.getAsset().x);
                int distanceY = abs(yArena - entry.second.getAsset().y);

                if (distanceX < MIN_DISTANCE_BETWEEN_ARENAS || distanceY < MIN_DISTANCE_BETWEEN_ARENAS)
                {
                    tooCloseToExistingArena = true;
                    break;
                }
            }

            if (grid[randomCell] || arenaList.count(i) > 0 || tooCloseToExistingArena)
            {
                attempts++;
                if (attempts > 100)
                    break;

                continue;
            }

            break;

        } while (true);

        arenaList.insert({i, Arena({xArena, yArena, ARENA_WIDTH, ARENA_HEIGHT})});
        grid[{xArena / ARENA_WIDTH, yArena / ARENA_HEIGHT}] = true;
    }   
}

#endif