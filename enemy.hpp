#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SDL2/SDL.h>

class Enemy
{
    private:
        SDL_Rect asset;
    
    public:
        Enemy(SDL_Rect asset);
        SDL_Rect getAsset();
        void setX(int x);
        void setY(int y);
        bool isPlayerTouching(const SDL_Rect& player);
};

Enemy::Enemy(SDL_Rect asset)
{
    this->asset = asset;
}

SDL_Rect Enemy::getAsset()
{
    return this->asset;
}

void Enemy::setX(int newX)
{
    this->asset.x += newX;
}

void Enemy::setY(int newY)
{
    this->asset.y += newY;
}

bool Enemy::isPlayerTouching(const SDL_Rect& player)
{
    return SDL_HasIntersection(&player, &asset) == SDL_TRUE;
}



#endif