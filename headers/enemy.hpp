#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SDL2/SDL.h>
#include <math.h>
#include <chrono>
#include <thread>

using namespace std;

enum class EnemyState
{
    Idle,
    Chasing,
};

class Enemy
{
    private:
        SDL_Rect asset;
        EnemyState state;
        chrono::steady_clock::time_point chaseStartTime;
        chrono::seconds chaseDuration;

    
    public:
        Enemy(SDL_Rect asset);
        SDL_Rect getAsset();
        void setX(int x);
        void setY(int y);
        EnemyState getState();
        void setState(EnemyState state);
        bool isPlayerTouching(const SDL_Rect& player);
        void updateEnemyAI(Player& player, float detectionDistance);
        bool isPlayerInView(Player& player, float detectionDistance);
        void moveChasing(SDL_Rect playerAsset);
        void moveIdle();
};

Enemy::Enemy(SDL_Rect asset)
{
    this->asset = asset;
    this->state = EnemyState::Idle;
}

SDL_Rect Enemy::getAsset()
{
    return this->asset;
}

void Enemy::setX(int newX)
{
    this->asset.x = newX;
}

void Enemy::setY(int newY)
{
    this->asset.y = newY;
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

void Enemy::updateEnemyAI(Player& player, float detectionDistance)
{
    switch (getState())
    {
        case EnemyState::Idle:
            moveIdle();
            if (isPlayerInView(player, detectionDistance))
            {
                setState(EnemyState::Chasing);
                chaseStartTime = chrono::steady_clock::now();
            }
            break;

        case EnemyState::Chasing:
            moveChasing(player.getAsset());

            auto currentTime = chrono::steady_clock::now();
            auto elapsedTime = chrono::duration_cast <chrono::milliseconds> (currentTime - chaseStartTime);

            if (elapsedTime >= chrono::milliseconds(3000))
                setState(EnemyState::Idle);
            
        
            else if (!(isPlayerInView(player, detectionDistance)))
                setState(EnemyState::Idle);
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
    
    int speed = 3;

    setX(asset.x + static_cast <int> (dx * speed));
    setY(asset.y + static_cast <int> (dy * speed));

}

void Enemy::moveIdle()
{
    srand(time(NULL));
    
    static bool isMovingUp;
    static bool isMovingRight;

    static int random = rand() % 2;
    if (random == 0)
    {
        isMovingRight = true;
        isMovingUp = false;

    }
    
    else if (random == 1)
    {
        isMovingUp = true;
        isMovingRight = false;
    }

    

    if (isMovingRight && !isMovingUp)
    {
        static int initialX = asset.x;
        if (isMovingRight)
        {
            setX(asset.x + 2);

            if (asset.x >= (initialX + 100))
                isMovingRight = false;
        }
        else if (!isMovingRight)
        {
            setX(asset.x - 2);

            if (asset.x <= (initialX - 100))
                isMovingRight = true;
        }
    }
    else
    {
        static int initialY = asset.y;
        if (isMovingUp)
        {
            setY(asset.y + 2);

            if (asset.y >= (initialY + 100))
                isMovingUp = false;
        }
        else if (!isMovingUp)
        {
            setY(asset.y - 2);

            if (asset.y <= (initialY - 100))
                isMovingUp = true;
        }
    }
}





#endif