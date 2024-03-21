#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <cmath>

#include "gameSettings.hpp"
#include "entity.hpp"
#include "window.hpp"

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
        SDL_Texture* playerNoSpearTexture;
        SDL_Texture* playerSpearTexture;

    public:
        Player() = default;
        Player(int health, const SDL_Rect& asset);
        ~Player();
        
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
        void render();
};

/**
 * @brief Constructor for Player
 * @param health The player health
 * @param asset The rectangle asset
*/
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

    this->playerSpearTexture = Window::loadTexture("assets/player_spear.png");
    this->playerNoSpearTexture = Window::loadTexture("assets/player_no_spear.png");
}

/**
 * @brief Destructor for player
*/
Player::~Player()
{
    SDL_DestroyTexture(playerNoSpearTexture);
    SDL_DestroyTexture(playerSpearTexture);
}

/**
 * @brief Sets player health
 * @param newHealth The new player Health
*/
void Player::setHealth(int newHealth)
{
    this->health = newHealth;
}

/**
 * @brief Gets the player health
 * @return Player health
*/
int Player::getHealth()
{
    return this->health;
}

/**
 * @brief Changes player health
 * Changes isPlayerAlive 
 * @param healthDiff The new health difference
*/
void Player::changeHealth(int healthDiff)
{
    this->health += healthDiff;

    Data::isPlayerAlive = !(this->health <= 0);
}

/**
 * @brief Player movement
*/
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

/**
 * @brief Moves player
 * @param x The new x position
 * @param y The new y position
*/
void Player::move(int x, int y)
{
    // so it doesn't go offscreen
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

/**
 * @brief Returns if player is nearby object
 * @param sourceRect The A rectangle
 * @param destRect The B rectangle
 * @param range The nearby radius
 * @return True or False
*/
bool Player::isNearby(const SDL_Rect& sourceRect, const SDL_Rect& destRect, int range)
{
    return (abs(sourceRect.x - destRect.x) < range && abs(sourceRect.y - destRect.y) < range);  
}

/**
 * @brief Returns if player is near arena
 * @return True or False
*/
bool Player::isNearArena()
{
    return this->nearArena;
}

/**
 * @brief Returns if player is near ladder
 * @return True or False
*/
bool Player::isNearLadder()
{
    return this->nearLadder;
}

/**
 * @brief Updates the player animation
 * @param speed The animation speed
*/
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

/**
 * @brief Gets player state
 * @return Player state
*/
PlayerState Player::getState()
{
    return this->state;
}

/**
 * @brief Sets player state
 * @param newState the new player state
*/
void Player::setState(const PlayerState& newState)
{
    this->state = newState;
}

/**
 * @brief Sets if player is near arena
 * @param newNearArena The new near arena
*/
void Player::setNearArena(bool newNearArena)
{
    this->nearArena = newNearArena;
}

/**
 * @brief Sets if player is near ladder
 * @param newNearLadder The new near ladder
*/
void Player::setNearLadder(bool newNearLadder)
{
    this->nearLadder = newNearLadder;
}

/**
 * @brief Resets the player
 * @param health The new health
 * @param newAsset The new asset
*/
void Player::reset(int health, const SDL_Rect& newAsset)
{
    this->health = health;
    Data::isPlayerAlive = health > 0;
    this->srcRect = {0, 0, 120, 116};
    this->isMoving = false;
    this->asset = newAsset;
    this->attack = 0;
}

/**
 * @brief Sets player attack
 * @param newAttack The new player attack
*/
void Player::setAttack(int newAttack)
{
    this->attack = newAttack;
}

/**
 * @brief Increases player attack
*/
void Player::increaseAttack()
{
    this->attack++;
}

/**
 * @brief Decreases player attack
*/
void Player::decreaseAttack()
{
    this->attack--;
}

/**
 * @brief Gets player attack
*/
int Player::getAttack()
{
    return this->attack;
}

/**
 * @brief Returns if player is touching
 * @param other The object player is touching
 * @return True or False
*/
bool Player::isPlayerTouching(const SDL_Rect& other)
{
    return SDL_HasIntersection(&other, &asset) == SDL_TRUE;
}

/**
 * @brief Renders player
*/
void Player::render()
{
    if (this->attack <= 0) 
        Window::drawAnimation(this->srcRect, this->asset, playerNoSpearTexture, this->flip);
    else
        Window::drawAnimation(this->srcRect, this->asset, playerSpearTexture, this->flip);

}

#endif