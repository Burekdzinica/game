#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>

class Player
{
    private:
        int health;
        SDL_Rect asset; // stores position

    public:
        Player(int health, SDL_Rect asset);
        int getHealth();
        SDL_Rect getAsset();
        void setX(int newX);
        void setY(int newY);
        int getX();
        int getY();

};

Player::Player(int health, SDL_Rect asset)
{
    this->health = health;
    this->asset = asset;
}

int Player::getHealth()
{
    return this->health;
}

SDL_Rect Player::getAsset()
{
    return this->asset;
}

void Player::setX(int newX)
{
    this->asset.x += newX;
}

void Player::setY(int newY)
{
    this->asset.y += newY;
}

int Player::getX()
{
    return this->asset.x;
}

int Player::getY()
{
    return this->asset.y;
}

#endif

