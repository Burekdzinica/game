#ifndef LADDER_HPP
#define LADDER_HPP

#include <SDL2/SDL.h>
#include <time.h>

using namespace std;

extern const int enemyWidth, enemyHeight;
extern const int arenaWidth, arenaHeight;

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

class Level
{
    private:
        int lvl;
        int arenaCounter;

    public:
        Level();
        void resetGame(Player &player, vector <Enemy> &enemyList, unordered_map <int, Arena>& arenaList, Ladder& ladder, int& isCloseTo, int health, int WIDTH, int HEIGHT);
        void setLevel();
        void setArenaCounter(int arenaCounter);
        int getLevel();
        int getArenaCounter();
};

Level::Level()
{
    this->lvl = 1;
}

void Level::resetGame(Player &player, vector <Enemy> &enemyList, unordered_map <int, Arena>& arenaList, Ladder& ladder, int& isCloseTo, int health, int WIDTH, int HEIGHT)
{
    player.reset(health, {max((rand() % WIDTH - player.getAsset().w), 0), max((rand() % HEIGHT - player.getAsset().h), 0), player.getAsset().w, player.getAsset().h});

    static int levelCounter = 1;
    levelCounter++;
    if ((levelCounter == 2) || (((levelCounter - 2) % 3 == 0)))
    {
        int xEnemy, yEnemy;
        do
        {
            xEnemy = max((rand() % WIDTH - enemyWidth), 0);
            yEnemy =  max((rand() % HEIGHT - enemyHeight), 0);

        } while (!(xEnemy < player.getAsset().x - 200 || xEnemy > player.getAsset().x + 200) && !(yEnemy < player.getAsset().y - 200 || yEnemy > player.getAsset().y + 200));

        Enemy newEnemy({xEnemy, yEnemy, enemyWidth, enemyHeight});

        enemyList.push_back(newEnemy);
    }

    for (auto &enemy : enemyList)
    {
        int newX, newY;
        do
        {
            newX = max((rand() % WIDTH - enemyWidth), 0);
            newY = max((rand() % HEIGHT - enemyWidth), 0);

        } while (!(newX < player.getAsset().x - 200 || newX > player.getAsset().x + 200) && !(newY < player.getAsset().y - 200 || newY > player.getAsset().y + 200));
        enemy.setX(newX);
        enemy.setY(newY);
        enemy.setBounds();
    }

    arenaCounter += rand() % 2; 

    arenaList.clear();

    for (int i=0; i<arenaCounter; i++)
    {
        arenaList.insert({i, Arena({max((rand() % WIDTH - 200), 0), max((rand() % HEIGHT - 200), 0), arenaWidth, arenaHeight})});
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