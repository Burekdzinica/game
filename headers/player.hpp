#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <cmath>
#include <SDL2/SDL.h>

#include "gameSettings.hpp"

extern const int animationSpeed;

enum class PlayerState
{
    Idle,
    Moving,
};

class Player
{
    private:
        int health;
        SDL_Rect asset; // stores position
        SDL_Rect srcRect;
        PlayerState state;
        bool isMoving;
        SDL_RendererFlip flip;
        bool nearLadder;
        bool nearArena;

    public:
        Player() = default;
        Player(int health, SDL_Rect asset);
        void setHealth(int newHealth);
        int getHealth();
        void changeHealth(int healthDiff);
        void movePlayer();
        void move(int x, int y);
        SDL_Rect getAsset() const;
        bool isNearby(const SDL_Rect player, SDL_Rect arena, int range) const;
        void setX(int newX);
        void setY(int newY);
        int getX();
        int getY();
        bool isNearLadder();
        bool isNearArena();
        void updatePlayerAnimation(int speed);
        void setSrcRect(int x, int y, int w, int h, int frame, int speed);
        PlayerState getState();
        void setNearArena(bool newNearArena);
        void setNearLadder(bool newNearLadder);
        void setState(PlayerState newState);
        SDL_RendererFlip getFlip();
        void setFlip(SDL_RendererFlip newflip);
        SDL_Rect getSrcRect();
        void reset(int health, SDL_Rect newAsset);
};

Player::Player(int health, SDL_Rect asset)
{
    this->health = health;
    this->asset = asset;
    Data::isPlayerAlive = health > 0;
    this->srcRect = {0, 0, 120, 116};
    this->isMoving = false;
    this->flip = SDL_FLIP_NONE;
    this->state = PlayerState::Idle;
    this->nearArena = false;
    this->nearLadder = false;
}

void Player::setHealth(int newHealth)
{
    this->health = newHealth;
}

int Player::getHealth()
{
    return this->health;
}

void Player::changeHealth(int healthDiff)
{
    this->health += healthDiff;

    Data::isPlayerAlive = !(this->health <= 0);
}

void Player::movePlayer()
{
    const Uint8 *keyState = SDL_GetKeyboardState(nullptr);
    float x = 0;
    float y = 0;
    float speed = 8;

    if (keyState[SDL_SCANCODE_A])
    {
        this->state = PlayerState::Moving;
        flip = SDL_FLIP_HORIZONTAL;
        x -= speed;
    }
    if (keyState[SDL_SCANCODE_D])
    {
        this->state = PlayerState::Moving;
        x += speed;
    }


    if (keyState[SDL_SCANCODE_W])
    {
        this->state = PlayerState::Moving;
        y -= speed;
    }


    if (keyState[SDL_SCANCODE_S])
    {
        this->state = PlayerState::Moving;
        y += speed;
    }

    if (x != 0 && y != 0)
    {
        float diagonalSpeed = speed / sqrt(2);
        x = (x > 0) ? diagonalSpeed : -diagonalSpeed;
        y = (y > 0) ? diagonalSpeed : -diagonalSpeed;

    }

    move(x, y);
}

// doesn't go offscreen
void Player::move(int x, int y)
{
    if ((this->asset.x + x) > (GameSettings::WIDTH - asset.w))
        this->asset.x = (GameSettings::WIDTH - asset.w);

    else if ((this->asset.x + x) <= 0)
        this->asset.x = 0;
    
    if ((this->asset.y + y) > (GameSettings::HEIGHT - asset.h))
        this->asset.y = (GameSettings::HEIGHT - asset.h);  

    else if ((this->asset.y + y) <= 0)
        this->asset.y = 0;
    
    if ((this->asset.x + x) >= 0 && (this->asset.x + x) <= (GameSettings::WIDTH - asset.w) && (this->asset.y + y) >= 0 && (this->asset.y + y) <= (GameSettings::HEIGHT - asset.h))
    {
        this->asset.x += x;
        this->asset.y += y;
    }
    updatePlayerAnimation(animationSpeed);
    updatePlayerAnimation(animationSpeed);
}

SDL_Rect Player::getAsset() const
{
    return this->asset;
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

bool Player::isNearArena()
{
    return this->nearArena;
}

bool Player::isNearLadder()
{
    return this->nearLadder;
}

void Player::updatePlayerAnimation(int speed)
{
    switch(getState())
    {
        case PlayerState::Idle:
            setSrcRect(0, 0, 120, 112, 4, speed);
            break;
        
        case PlayerState::Moving:
            setSrcRect(0, 112, 120, 112,  8, speed);
            break;
    }
}

void Player::setSrcRect(int x, int y, int w, int h, int frames, int speed)
{
    this->srcRect.y = y;
    this->srcRect.w = w;
    this->srcRect.h = h;
    this->srcRect.x = srcRect.w * (static_cast<int> ((SDL_GetTicks() / speed) % frames));
}

PlayerState Player::getState()
{
    return this->state;
}

void Player::setState(PlayerState newState)
{
    this->state = newState;
}

void Player::setNearArena(bool newNearArena)
{
    this->nearArena = newNearArena;
}

void Player::setNearLadder(bool newNearLadder)
{
    this->nearLadder = newNearLadder;
}

SDL_RendererFlip Player::getFlip()
{
    return this->flip;
}

void Player::setFlip(SDL_RendererFlip newFlip)
{
    this->flip = newFlip;
}

SDL_Rect Player::getSrcRect()
{
    return this->srcRect;
}

void Player::reset(int health, SDL_Rect newAsset)
{
    this->health = health;
    Data::isPlayerAlive = health > 0;
    this->srcRect = {0, 0, 120, 116};
    this->isMoving = false;
    this->asset = newAsset;
}

#endif