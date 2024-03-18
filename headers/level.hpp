#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <iostream>
#include <vector>
#include <unordered_map>

#include "player.hpp"
#include "enemy.hpp"
#include "arena.hpp"

using namespace std;

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
        void setLevel();
        void setLevel(int x);
        void resetLevel();
        void setArenaCounter(int arenaCounter);
        int getLevel();
        int getArenaCounter();
        void setEnemyCounter(int x);
        void decreaseEnemyCounter();
};

Level::Level()
{
    this->lvl = 1;
    this->enemyCounter = 1;
}

void Level::nextLevel(Player& player, vector <Enemy>& enemyList, unordered_map <int, Arena>& arenaList, Ladder& ladder, int isCloseTo, int health)
{
    player.reset(health, {max((rand() % GameSettings::WIDTH - player.getAsset().w), 0), max((rand() % GameSettings::HEIGHT - player.getAsset().h), 0), player.getAsset().w, player.getAsset().h});

    unordered_map <pair<int, int>, bool, PairHash> grid;
    for (int i = 0; i < GameSettings::WIDTH / ENEMY_WIDTH; i++)
        for (int j = 0; j < GameSettings::HEIGHT / ENEMY_HEIGHT; j++)
            grid.insert({(make_pair(i,j)), false});

    static int levelCounter = lvl;
    levelCounter++;
    if ((levelCounter == 2) || (((levelCounter - 2) % 3 == 0)))
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

void Level::setLevel()
{
    this->lvl++;
}

void Level::setLevel(int x)
{
    this->lvl = x;
}

void Level::resetLevel()
{
    this->lvl = 1;
}

void Level::setArenaCounter(int arenaCounter)
{
    this->arenaCounter = arenaCounter;
}

int Level::getLevel()
{
    return this->lvl;
}

int Level::getArenaCounter()
{
    return this->arenaCounter;
}

void Level::setEnemyCounter(int x)
{
    this->enemyCounter = x;
}

void Level::decreaseEnemyCounter()
{
    this->enemyCounter--;
}

#endif 