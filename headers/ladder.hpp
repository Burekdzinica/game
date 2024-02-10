#ifndef LADDER_HPP
#define LADDER_HPP

#include <SDL2/SDL.h>

using namespace std;

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

class Level
{
    private:
        int lvl;

    public:
        Level();
        void resetGame(Player &player, Enemy& enemy, unordered_map <int, Arena>& arenaList, Ladder& ladder, int& isCloseTo, int health, int WIDTH, int HEIGHT);
        void setLevel();
        int getLevel();
};

Level::Level()
{
    this->lvl = 1;
}

void Level::resetGame(Player &player, Enemy& enemy, unordered_map <int, Arena>& arenaList, Ladder& ladder, int& isCloseTo, int health, int WIDTH, int HEIGHT)
{
    player = Player(health, {rand() % WIDTH, rand() % HEIGHT, 180, 216});

    enemy.setX(rand() % WIDTH - enemy.getAsset().w);
    enemy.setY(rand() % HEIGHT - enemy.getAsset().h);

    arenaList.clear();

    int randomArenaCounter = rand() % 2;
    int arenaCounter = 3 + randomArenaCounter;

    for (int i=0; i<arenaCounter; i++)
    {
        arenaList.insert({i, Arena({rand() % WIDTH - 200, rand() % HEIGHT - 200, 200, 200})});
    }

    ladder.setX(rand() % WIDTH - ladder.getAsset().w);
    ladder.setY(rand() % HEIGHT - ladder.getAsset().h);

    isCloseTo = -1;
}

void Level::setLevel()
{
    this->lvl++;
}

int Level::getLevel()
{
    return this->lvl;
}


#endif 