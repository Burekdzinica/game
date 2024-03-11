#ifndef LADDER_HPP
#define LADDER_HPP

#include <SDL2/SDL.h>
#include <time.h>

#include "player.hpp"
#include "gameSettings.hpp"

using namespace std;

extern const int ENEMY_WIDTH, ENEMY_HEIGHT;
extern const int ARENA_WIDTH, ARENA_HEIGHT;
extern const int PLAYER_WIDTH, PLAYER_HEIGHT;

extern const int MIN_DISTANCE_BETWEEN_ARENAS;

class Ladder 
{
    private:
        SDL_Rect asset;

    public:
        Ladder() = default;
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

// pairs the hash // doesnt work without this
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
        int enemyCounter;

    public:
        // Level() = default;
        Level();
        void resetGame(Player &player, vector <Enemy> &enemyList, unordered_map <int, Arena>& arenaList, Ladder& ladder, int& isCloseTo, int health);
        void setLevel();
        void setLevel(int x);
        void resetLevel();
        void setArenaCounter(int arenaCounter);
        int getLevel();
        int getArenaCounter();
        void setEnemyCounter(int x);
};

Level::Level()
{
    this->lvl = 1;
    this->enemyCounter = 1;
}

void Level::resetGame(Player &player, vector <Enemy> &enemyList, unordered_map <int, Arena>& arenaList, Ladder& ladder, int& isCloseTo, int health)
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

            bool tooCloseToPlayer = (abs(xEnemy - player.getAsset().x) < PLAYER_WIDTH + 50) && (abs(yEnemy - player.getAsset().y) < PLAYER_HEIGHT + 50);
            if (!grid[randomCell] && !tooCloseToPlayer)
            {
                grid[randomCell] = true;
                break;
            }
           
        } while (true);
        
        Enemy newEnemy({xEnemy, yEnemy, ENEMY_WIDTH, ENEMY_HEIGHT});
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

    ladder.setX(max((rand() % GameSettings::WIDTH - ladder.getAsset().w), 0));
    ladder.setY(max((rand() % GameSettings::HEIGHT - ladder.getAsset().h), 0));

    isCloseTo = -1;
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

#endif 