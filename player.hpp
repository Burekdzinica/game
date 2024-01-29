#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>

class Player
{
    private:
        int health;
        SDL_Rect asset; // stores position
        bool isPlayerAlive;

    public:
        Player(int health, SDL_Rect asset);
        int getHealth();
        void changeHealth(int healthDiff);
        SDL_Rect getAsset();
        bool getIsPlayerAlive();
        void setIsPlayerAlive(bool newAlive);
        void setX(int newX);
        void setY(int newY);
        int getX();
        int getY();

};

Player::Player(int health, SDL_Rect asset)
{
    this->health = health;
    this->asset = asset;
    this->isPlayerAlive = health > 0;
}

int Player::getHealth()
{
    return this->health;
}

void Player::changeHealth(int healthDiff)
{
    this->health += healthDiff;
    this->isPlayerAlive = !(this->health <= 0);
}

SDL_Rect Player::getAsset()
{
    return this->asset;
}

bool Player::getIsPlayerAlive() 
{
    return this->isPlayerAlive;
}

void Player::setIsPlayerAlive(bool newAlive)
{
    this->isPlayerAlive = newAlive;
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

