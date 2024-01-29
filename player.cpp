#include "player.hpp"

#include <SDL2/SDL.h>

Player::Player(int health, SDL_Rect asset, int x, int y)
{
    this->health = health;
    this->asset = asset;
    this->position_x = x;
    this->position_y = y;
}

int Player::getHealth()
{
    return this->health;
}

SDL_Rect* Player::getAsset()
{
    return &this->asset;
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
