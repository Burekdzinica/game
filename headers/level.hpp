#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "player.hpp"
#include "enemy.hpp"
#include "arena.hpp"

using namespace std;

typedef unordered_map<pair<int, int>, bool, PairHash> Grid_t;

class Level
{
    private:
        int lvl;
        int arenaCounter;
        int enemyCounter;

    public:
        // Level() = default;
        Level();
        void nextLevel(Player& player, vector <Enemy>&  enemyList, unordered_map <int, Arena>& arenaList, Ladder& ladder, int isCloseTo, int health);    
        void increaseLevel();
        void setLevel(int x);
        void resetLevel();
        void setArenaCounter(int arenaCounter);
        int getLevel();
        int getArenaCounter();
        void setEnemyCounter(int x);
        void decreaseEnemyCounter();
};

/**
 * @brief Default constructor for Level
*/
Level::Level()
{
    this->lvl = 1;
    this->enemyCounter = 1;
}

/**
 * @brief Next level
 * @param player The player object
 * @param enemyList The enemy list
 * @param arenaList The arena list
 * @param ladder The ladder object
 * @param isCloseTo The index for arena clicks
 * @param health The player health
*/
void Level::nextLevel(Player& player, vector <Enemy>& enemyList, unordered_map <int, Arena>& arenaList, Ladder& ladder, int isCloseTo, int health)
{
    player.reset(health, {max((rand() % GameSettings::WIDTH - player.getAsset().w), 0), 
                          max((rand() % GameSettings::HEIGHT - player.getAsset().h), 0), player.getAsset().w, player.getAsset().h});

    Grid_t grid;
    for (int i = 0; i < GameSettings::WIDTH / ENEMY_WIDTH; i++)
        for (int j = 0; j < GameSettings::HEIGHT / ENEMY_HEIGHT; j++)
            grid.insert({(make_pair(i,j)), false});

    if ((lvl == 2) || (((lvl - 2) % 3 == 0)))
        enemyCounter++;

    enemyList.clear();
    Enemy::generateEnemyPositions(grid, player, enemyList, this->enemyCounter);

    arenaCounter += rand() % 2; 

    arenaList.clear();
    Arena::generateArenaPositions(arenaCounter, grid, arenaList);

    ladder.setX(max((rand() % GameSettings::WIDTH - ladder.getAsset().w), 0));
    ladder.setY(max((rand() % GameSettings::HEIGHT - ladder.getAsset().h), 0));

    isCloseTo = -1;

    player.setNearLadder(false);
}

/**
 * @brief Sets the next level
*/
void Level::increaseLevel()
{
    this->lvl++;
}

/**
 * @brief
 * @param x The x level
*/
void Level::setLevel(int x)
{
    this->lvl = x;
}

/**
 * @brief Sets lvl to 1
*/
void Level::resetLevel()
{
    this->lvl = 1;
}

/**
 * @brief Sets arena counter
 * @param arenaCounter The new arena counter
*/
void Level::setArenaCounter(int arenaCounter)
{
    this->arenaCounter = arenaCounter;
}

/**
 * @brief Gets the level
 * @return Level count
*/
int Level::getLevel()
{
    return this->lvl;
}

/**
 * @brief Gets arena counter
 * @return Arena counter
*/
int Level::getArenaCounter()
{
    return this->arenaCounter;
}

/**
 * @brief Sets enemy counter
 * @param x The new enemy counter
*/
void Level::setEnemyCounter(int x)
{
    this->enemyCounter = x;
}

/**
 * @brief Decreasese enemy counter by 1
*/
void Level::decreaseEnemyCounter()
{
    this->enemyCounter--;
}

#endif 