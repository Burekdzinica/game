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
        void resetGame(Player& player, vector <Enemy>&  enemyList, unordered_map <int, Arena>& arenaList, Ladder& ladder, int isCloseTo, int health);    
        void setLevel();
        void setLevel(int x);
        void resetLevel();
        void setArenaCounter(int arenaCounter);
        int getLevel();
        int getArenaCounter();
        void setEnemyCounter(int x);
        void decreaseEnemyCounter();
        void generateEnemyPositions(unordered_map<pair<int, int>, bool, PairHash>& grid, Player& player, vector <Enemy>& enemyList);
};

Level::Level()
{
    this->lvl = 1;
    this->enemyCounter = 1;
}

void Level::resetGame(Player& player, vector <Enemy>& enemyList, unordered_map <int, Arena>& arenaList, Ladder& ladder, int isCloseTo, int health)
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
    generateEnemyPositions(grid, player, enemyList);

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


void Level::generateEnemyPositions(unordered_map<pair<int, int>, bool, PairHash>& grid, Player& player, vector <Enemy>& enemyList)
{
    for (int i = 0; i < enemyCounter; i++)
    {
        int xEnemy, yEnemy;
        do
        {
            auto it = begin(grid);
            advance(it, rand() % grid.size());
            pair<int, int> randomCell = it->first;

            xEnemy = randomCell.first * ENEMY_WIDTH;
            yEnemy = randomCell.second * ENEMY_HEIGHT;

            bool tooCloseToPlayer = (abs(xEnemy - player.getAsset().x) < PLAYER_WIDTH + MIN_DISTANCE_BETWEEN_PLAYER_AND_ENEMY) || (abs(yEnemy - player.getAsset().y) < PLAYER_HEIGHT + MIN_DISTANCE_BETWEEN_PLAYER_AND_ENEMY);
            if (!grid[randomCell] && !tooCloseToPlayer)
            {
                grid[randomCell] = true;
                break;
            }

        } while(true);
        
        Enemy newEnemy({xEnemy, yEnemy, ENEMY_WIDTH, ENEMY_HEIGHT});
        enemyList.push_back(newEnemy);
    }
}

#endif 