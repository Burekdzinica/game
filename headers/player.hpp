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
        void move(int x, int y);
        SDL_Rect getAsset() const;
        bool getIsPlayerAlive();
        void setIsPlayerAlive(bool newAlive);
        bool isNearby(const SDL_Rect player, SDL_Rect arena, int range) const;
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
                setX(-15);
                break;
            case SDLK_d:
                setX(15);
                break;
            case SDLK_w:
                setY(-15);
                break;
            case SDLK_s:
                setY(15);
                break;
        }
}

// void Player::movePlayer(SDL_Keycode key)
// {
//     float x = 0;
//     float y = 0;
//     float speed = 10;

//     switch (key)
//     {
//         case SDLK_a:
//             x -= speed;
//             break;
//         case SDLK_d:
//             x += speed;
//             break;
//         case SDLK_w:
//             y -= speed;
//             break;
//         case SDLK_s:
//             y += speed;
//             break;
//     }

//     move(x,y);
// }

// void Player::move(int x, int y)
// {
//     this->asset.x += x;
//     this->asset.y += y;
// }

SDL_Rect Player::getAsset() const
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

bool Player::isNearby(const SDL_Rect sourceRect, SDL_Rect destRect, int range) const
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

