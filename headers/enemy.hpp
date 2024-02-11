#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SDL2/SDL.h>

class Enemy
{
    private:
        SDL_Rect asset;
    
    public:
        Enemy(SDL_Rect asset);
        // Enemy(int WIDTH, int HEIGHT, int WIDTH_enemy, int HEIGHT_enemy);
        SDL_Rect getAsset();
        void setX(int x);
        void setY(int y);
        bool isPlayerTouching(const SDL_Rect& player);
};

Enemy::Enemy(SDL_Rect asset)
{
    this->asset = asset;
}




// Enemy::Enemy(int WIDTH, int HEIGHT, int WIDTH_enemy, int HEIGHT_enemy)
// {
//     int randWidth = rand() % WIDTH;

//     if (randWidth < WIDTH_enemy)
//         randWidth += WIDTH_enemy;
//     else
//         randWidth -= WIDTH_enemy;

//     int randHeight = rand() % HEIGHT;
    
//     if (randHeight < HEIGHT_enemy)
//         randHeight += HEIGHT_enemy;
//     else
//         randHeight -= HEIGHT_enemy;

//     asset = {randWidth, randHeight, WIDTH_enemy, HEIGHT_enemy};

// }

SDL_Rect Enemy::getAsset()
{
    return this->asset;
}

void Enemy::setX(int newX)
{
    this->asset.x = newX;
}

void Enemy::setY(int newY)
{
    this->asset.y = newY;
}

bool Enemy::isPlayerTouching(const SDL_Rect& player)
{
    return SDL_HasIntersection(&player, &asset) == SDL_TRUE;
}

#endif