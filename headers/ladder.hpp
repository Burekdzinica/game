#ifndef LADDER_HPP
#define LADDER_HPP

#include <SDL2/SDL.h>
#include <time.h>

#include "player.hpp"

using namespace std;

extern const int enemyWidth, enemyHeight;
extern const int arenaWidth, arenaHeight;
extern const int playerWidth, playerHeight;
extern const int WIDTH, HEIGHT;

extern const int minDistanceBetweenArenas;

class Ladder 
{
    private:
        SDL_Rect asset;

    public:
        Ladder(SDL_Rect asset);
        SDL_Rect getAsset();
        void setX(int x);
        void setY(int y);

};

Ladder::Ladder(SDL_Rect asset)
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

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

class Level
{
    private:
        int lvl;
        int arenaCounter;

    public:
        Level();
        void resetGame(Player &player, vector <Enemy> &enemyList, unordered_map <int, Arena>& arenaList, Ladder& ladder, int& isCloseTo, int health);
        void setLevel();
        void setArenaCounter(int arenaCounter);
        int getLevel();
        int getArenaCounter();
};

Level::Level()
{
    this->lvl = 1;
}

void Level::resetGame(Player &player, vector <Enemy> &enemyList, unordered_map <int, Arena>& arenaList, Ladder& ladder, int& isCloseTo, int health)
{
    player.reset(health, {max((rand() % WIDTH - player.getAsset().w), 0), max((rand() % HEIGHT - player.getAsset().h), 0), player.getAsset().w, player.getAsset().h});

    unordered_map <pair<int, int>, bool, PairHash> grid;
    for (int i = 0; i< WIDTH / enemyWidth; i++)
        for (int j = 0; j < HEIGHT / enemyHeight; j++)
            grid.insert({(make_pair(i,j)), false});

    static int levelCounter = 1;
    levelCounter++;
    static int enemyCounter = 1;
    if ((levelCounter == 2) || (((levelCounter - 2) % 3 == 0)))
        enemyCounter++;

    enemyList.clear();
    for (int i = 0; i < enemyCounter; i++)
    {
        int xEnemy, yEnemy; 
        do
        {
            auto it = begin(grid);
            advance(it, rand() % grid.size());
            pair<int, int> randomCell = it->first;

            xEnemy = randomCell.first * enemyWidth;
            yEnemy = randomCell.second * enemyHeight;

            bool tooCloseToPlayer = (abs(xEnemy - player.getX()) < playerWidth + 50) && (abs(yEnemy - player.getY()) < playerHeight + 50);
            if (!grid[randomCell] && !tooCloseToPlayer)
            {
                grid[randomCell] = true;
                break;
            }
           
        } while (true);
        
        Enemy newEnemy({xEnemy, yEnemy, enemyWidth, enemyHeight});
        newEnemy.setBounds();
        enemyList.push_back(newEnemy);
    }

    arenaCounter += rand() % 2; 

    arenaList.clear();
    int xArena, yArena;
    for (int i=0; i<arenaCounter; i++)
    {
        int attempts = 0;
        do
        {
            auto it = begin(grid);
            advance(it, rand() % grid.size());
            pair<int, int> randomCell = it->first;

            xArena = randomCell.first * arenaWidth;
            yArena = randomCell.second * arenaHeight;

            bool tooCloseToExistingArena = false;
            for (const auto& entry : arenaList)
            {
                int distanceX = abs(xArena - entry.second.getAsset().x);
                int distanceY = abs(yArena - entry.second.getAsset().y);

                if (distanceX < minDistanceBetweenArenas || distanceY < minDistanceBetweenArenas)
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

        arenaList.insert({i, Arena({xArena, yArena, arenaWidth, arenaHeight})});
        grid[{xArena / arenaWidth, yArena / arenaHeight}] = true;
    }

    ladder.setX(max((rand() % WIDTH - ladder.getAsset().w), 0));
    ladder.setY(max((rand() % HEIGHT - ladder.getAsset().h), 0));

    isCloseTo = -1;
}

void Level::setLevel()
{
    this->lvl++;
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

#endif 