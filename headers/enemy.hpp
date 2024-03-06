#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SDL2/SDL.h>
#include <math.h>
#include <chrono>
#include <thread>

#include "player.hpp"
#include "gameSettings.hpp"
#include "entity.hpp"

using namespace std;

const int BOUND_RANGE = 900;

enum class EnemyState
{
    Idle,
    Chasing,
    Attacked,
};

class Enemy : public EntityAnimation
{
    private:
        EnemyState state;
        chrono::steady_clock::time_point chaseStartTime;
        chrono::steady_clock::time_point attackStartTime;
        int xMovement;
        int yMovement;
        int direction;
        int bounds[4];

    
    public:
        Enemy(SDL_Rect asset);
        EnemyState getState();
        void setState(EnemyState state);
        void setBounds();
        bool isPlayerTouching(const SDL_Rect& player);
        void updateEnemyAI(Player& player, float detectionDistance, int animationSpeed);
        bool isPlayerInView(Player& player, float detectionDistance);
        void moveChasing(SDL_Rect playerAsset);
        void moveIdle();
};

Enemy::Enemy(SDL_Rect asset)
{
    this->asset = asset;
    this->state = EnemyState::Attacked;
    this->attackStartTime = chrono::steady_clock::now();
    this->xMovement = rand() % 2; // 1
    this->yMovement = (xMovement == 1) ? 0 : 1;
    this->direction = 1;
    this->srcRect = {0 , 0, 73, 126};
    this->flip = SDL_FLIP_NONE;

    setBounds();
}

bool Enemy::isPlayerTouching(const SDL_Rect& player)
{
    return SDL_HasIntersection(&player, &asset) == SDL_TRUE;
}

EnemyState Enemy::getState()
{
    return this->state;
}

void Enemy::setState(EnemyState newState)
{
    this->state = newState;
}

void Enemy::setBounds()
{
    this->bounds[0] = this->asset.x - BOUND_RANGE; // x left
    this->bounds[1] = this->asset.x + BOUND_RANGE; // x right
    this->bounds[2] = this->asset.y - BOUND_RANGE; // y left
    this->bounds[3] = this->asset.y + BOUND_RANGE; // y right
}

void Enemy::updateEnemyAI(Player& player, float detectionDistance, int animationSpeed)
{
    auto currentTime = chrono::steady_clock::now();
    auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(currentTime - chaseStartTime);

    switch (getState())
    {
        case EnemyState::Idle:
            setSrcRect(0, 126, 73, 126, 6, animationSpeed);
            moveIdle();
            if (isPlayerInView(player, detectionDistance))
            {
                setState(EnemyState::Chasing);
                chaseStartTime = chrono::steady_clock::now();
            }
            break;

        case EnemyState::Chasing:
            setSrcRect(0, 126, 73, 126, 6, animationSpeed);
            moveChasing(player.getAsset());
            // if enemy chased for x time, goes to attacked
            if (elapsedTime >= chrono::milliseconds(5000) && isPlayerInView(player, detectionDistance))
            {
                setState(EnemyState::Attacked);
                attackStartTime = chrono::steady_clock::now();
            }
            
            else if (!(isPlayerInView(player, detectionDistance)))
            {
                setState(EnemyState::Idle);
                setBounds();
            }
            
            else if (isPlayerTouching(player.getAsset()))
            {
                setState(EnemyState::Attacked);
                
                //moves enemy a bit / depends on pos
                if (player.getAsset().x > asset.x)
                    setX(max((asset.x - 50), 0));

                else if (player.getAsset().x < asset.x)
                    setX(max((asset.x + 50), 0));
                
                if (player.getAsset().y > asset.y)
                    setY(max((asset.y - 50), 0));
                
                else if (player.getAsset().y < asset.y)
                    setY(max((asset.y + 50), 0));

                player.changeHealth(-1);

                attackStartTime = chrono::steady_clock::now();
            }
            break;

        case EnemyState::Attacked:
            setSrcRect(0, 0, 73, 126, 4, animationSpeed);
            this->flip = SDL_FLIP_NONE;
            auto currentTime = chrono::steady_clock::now();
            auto elapsedTime = chrono::duration_cast <chrono::milliseconds> (currentTime - attackStartTime);

            // waits for x time, then goes to idle
            if (elapsedTime >= chrono::milliseconds(2000))
            {
                setState(EnemyState::Idle);
                setBounds();
            }
            if (isPlayerTouching(player.getAsset()))
            {
                if (player.getAsset().x > asset.x)
                    setX(max((asset.x - 50), 0));

                else if (player.getAsset().x < asset.x)
                    setX(max((asset.x + 50), 0));
                
                if (player.getAsset().y > asset.y)
                    setY(max((asset.y - 50), 0));
                
                else if (player.getAsset().y < asset.y)
                    setY(max((asset.y + 50), 0));

                player.changeHealth(-1);
            }
            break;
    }
}

bool Enemy::isPlayerInView(Player& player, float detectionDistance)
{
    float dx = static_cast <float> (player.getAsset().x + player.getAsset().w / 2) - static_cast <float> (asset.x + asset.w / 2);
    float dy = static_cast <float> (player.getAsset().y + player.getAsset().h / 2) - static_cast <float> (asset.y + asset.h / 2);

    float distance = sqrt(dx * dx + dy * dy);

    return (distance <= detectionDistance);
}

void Enemy::moveChasing(SDL_Rect playerAsset)
{
    float dx = static_cast <float> (playerAsset.x + playerAsset.w / 2) - static_cast <float> (asset.x + asset.w / 2);
    float dy = static_cast <float> (playerAsset.y + playerAsset.h / 2) - static_cast <float> (asset.y + asset.h / 2);

    float distance = sqrt(dx * dx + dy * dy);

    if (distance != 0)
    {
        dx /= distance;
        dy /= distance;
    }
    
    float speed = 3.5;

    if (dx < 0)
        this->flip = SDL_FLIP_HORIZONTAL;
    else    
        this->flip = SDL_FLIP_NONE;

    setX(asset.x + static_cast <int> (dx * speed));
    setY(asset.y + static_cast <int> (dy * speed));

}

void Enemy::moveIdle()
{
    const float speed = 2.5;
    flip = SDL_FLIP_NONE;

    // bounces of corner
    if (asset.x + this->direction * speed * this->xMovement > GameSettings::WIDTH - asset.w|| asset.x + this->direction * speed * this->xMovement < 0) 
    {
        this->direction *= -1;
        flip = (this->direction == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    }

    setX(asset.x + this->direction * speed * this->xMovement);
    if (asset.x > this->bounds[1] || (asset.x < this->bounds[0]))
    {
        this->direction *= -1;
        flip = (this->direction == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    }

    if (asset.y + this->direction * speed * this->yMovement > GameSettings::HEIGHT - asset.h || asset.y + this->direction * speed * this->yMovement < 0) 
        this->direction *= -1;

    setY(asset.y + this->direction * speed * this->yMovement);
    if (asset.y > this->bounds[3] || (asset.y < this->bounds[2]))
        this->direction *= -1;
}

#endif