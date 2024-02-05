#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>

class Player
{
    private:
        int health;
        SDL_Rect asset; // stores position
        bool isPlayerAlive;
        int points;

    public:
        Player(int health, SDL_Rect asset);
        int getHealth();
        void changeHealth(int healthDiff);
        void movePlayer(SDL_Keycode key);
        SDL_Rect getAsset();
        bool getIsPlayerAlive();
        void setIsPlayerAlive(bool newAlive);
        bool isNearby(SDL_Rect player, SDL_Rect arena, int range);
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

void Player::movePlayer(SDL_Keycode key)
{
        switch (key)
        {
            case SDLK_a:
                setX(-10);
                break;
            case SDLK_d:
                setX(10);
                break;
            case SDLK_w:
                setY(-10);
                break;
            case SDLK_s:
                setY(10);
                break;
        }
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

bool Player::isNearby(SDL_Rect sourceRect, SDL_Rect destRect, int range)
{
    return (abs(sourceRect.x - destRect.x) < range && abs(sourceRect.y - destRect.y) < range);  
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

