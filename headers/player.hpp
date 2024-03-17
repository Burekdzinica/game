#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <cmath>
#include <SDL2/SDL.h>

#include "gameSettings.hpp"
#include "entity.hpp"

extern const int ANIMATION_SPEED;

enum class PlayerState
{
    Idle,
    Moving,
};

class Player : public EntityAnimation
{
    private:
        int health;
        PlayerState state;
        bool isMoving;
        bool nearLadder;
        bool nearArena;
        int attack;

    public:
        Player() = default;
        Player(int health, const SDL_Rect& asset);
        void setHealth(int newHealth);
        int getHealth();
        void changeHealth(int healthDiff);
        void movePlayer();
        void move(int x, int y);
        bool isNearby(const SDL_Rect& player, const SDL_Rect& arena, int range);
        bool isNearLadder();
        bool isNearArena();
        void updatePlayerAnimation(int speed);
        PlayerState getState();
        void setNearArena(bool newNearArena);
        void setNearLadder(bool newNearLadder);
        void setState(const PlayerState& newState);
        void reset(int health, const SDL_Rect& newAsset);
        void setAttack(int newAttack);
        void increaseAttack();
        void decreaseAttack();
        int getAttack();
        bool isPlayerTouching (const SDL_Rect& other);
};

Player::Player(int health, const SDL_Rect& asset)
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
    this->attack = 0;
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
    updatePlayerAnimation(ANIMATION_SPEED);
}

bool Player::isNearby(const SDL_Rect& sourceRect, const SDL_Rect& destRect, int range)
{
    return (abs(sourceRect.x - destRect.x) < range && abs(sourceRect.y - destRect.y) < range);  
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

PlayerState Player::getState()
{
    return this->state;
}

void Player::setState(const PlayerState& newState)
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

void Player::reset(int health, const SDL_Rect& newAsset)
{
    this->health = health;
    Data::isPlayerAlive = health > 0;
    this->srcRect = {0, 0, 120, 116};
    this->isMoving = false;
    this->asset = newAsset;
}

void Player::setAttack(int newAttack)
{
    this->attack = newAttack;
}

void Player::increaseAttack()
{
    this->attack++;
}

void Player::decreaseAttack()
{
    this->attack--;
}


int Player::getAttack()
{
    return this->attack;
}

bool Player::isPlayerTouching(const SDL_Rect& other)
{
    return SDL_HasIntersection(&other, &asset) == SDL_TRUE;
}

#endif