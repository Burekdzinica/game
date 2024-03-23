#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <chrono>
#include <unordered_map>
#include <vector>

#include "player.hpp"
#include "gameSettings.hpp"
#include "entity.hpp"
#include "ladder.hpp"


using namespace std;

typedef unordered_map<pair<int, int>, bool, PairHash> Grid_t;

extern const int PLAYER_WIDTH, PLAYER_HEIGHT ;
extern const int PLAYER_WIDTH, PLAYER_HEIGHT;
extern const int MIN_DISTANCE_BETWEEN_PLAYER_AND_ENEMY;

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
        SDL_Texture* enemyTexture;

    
    public:
        Enemy();
        Enemy(const SDL_Rect& asset);
        ~Enemy();
        EnemyState getState();
        void setState(const EnemyState& state);
        void setBounds();
        bool isPlayerTouching(const SDL_Rect& player);
        void updateEnemyAI(Player& player, float detectionDistance, int animationSpeed);
        bool isPlayerInView(Player& player, float detectionDistance);
        void moveChasing(const SDL_Rect& playerAsset);
        void moveIdle();
        void setAttackTimer();
        static void generateEnemyPositions(Grid_t& grid, Player& player, vector <Enemy>& enemyList, int enemyCounter);
        void render(Enemy currentEnemy);
};

/**
 * @brief Default constructor for Enemy
*/
Enemy::Enemy()
{
    this->asset = asset;
    this->state = EnemyState::Attacked;
    this->attackStartTime = chrono::steady_clock::now();
    this->xMovement = rand() % 2; // 1
    this->yMovement = (xMovement == 1) ? 0 : 1;
    this->direction = 1;
    this->srcRect = {0 , 0, 73, 126};
    this->flip = SDL_FLIP_NONE;
    this->enemyTexture = Window::loadTexture("assets/enemy_reloaded.png");

    setBounds();
}

/**
 * @brief Contructor for Enemy
 * @param asset The enemy asset
*/
Enemy::Enemy(const SDL_Rect& asset)
{
    this->asset = asset;
    
    this->state = EnemyState::Attacked;
    this->attackStartTime = chrono::steady_clock::now();

    this->xMovement = rand() % 2; // 1
    this->yMovement = (xMovement == 1) ? 0 : 1;
    this->direction = 1;

    this->srcRect = {0 , 0, 73, 126};
    this->flip = SDL_FLIP_NONE;

    this->enemyTexture = Window::loadTexture("assets/enemy_reloaded.png");

    setBounds();
}

Enemy::~Enemy()
{
    SDL_DestroyTexture(enemyTexture);
}

/**
 * @brief Returns if enemy is touching player
 * @param player Player asset
 * @return True or False
*/
bool Enemy::isPlayerTouching(const SDL_Rect& player)
{
    return SDL_HasIntersection(&player, &asset) == SDL_TRUE;
}

/**
 * @brief Gets state
 * @return State
*/
EnemyState Enemy::getState()
{
    return this->state;
}

/**
 * @brief Sets state
 * @param newState New state
*/
void Enemy::setState(const EnemyState& newState)
{
    this->state = newState;
}

/**
 * @brief Sets bounding box
*/
void Enemy::setBounds()
{
    this->bounds[0] = this->asset.x - BOUND_RANGE; // x left
    this->bounds[1] = this->asset.x + BOUND_RANGE; // x right
    this->bounds[2] = this->asset.y - BOUND_RANGE; // y left
    this->bounds[3] = this->asset.y + BOUND_RANGE; // y right
}

/**
 * @brief Updates AI
 * @param player Player
 * @param detectionDistance The distance to detect player
 * @param animationSpeed The animation speed
*/
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
            break;
    }
}

/**
 * @brief Returns if player is in view
 * @param player The player 
 * @param detectionDistance The distance to detect player
 * @return True or False
*/
bool Enemy::isPlayerInView(Player& player, float detectionDistance)
{
    float dx = (float) (player.getAsset().x + player.getAsset().w / 2) - (float) (asset.x + asset.w / 2);
    float dy = (float) (player.getAsset().y + player.getAsset().h / 2) - (float) (asset.y + asset.h / 2);

    float distance = sqrt(dx * dx + dy * dy);

    return (distance <= detectionDistance);
}

/**
 * @brief Chasing player movement
 * @param playerAsset The player asset
*/
void Enemy::moveChasing(const SDL_Rect& playerAsset)
{
    float dx = (float) (playerAsset.x + playerAsset.w / 2) - (float) (asset.x + asset.w / 2);
    float dy = (float) (playerAsset.y + playerAsset.h / 2) - (float) (asset.y + asset.h / 2);

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

    setX(asset.x + (int) (dx * speed));
    setY(asset.y + (int) (dy * speed));
}

/**
 * @brief Idle movement
*/
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

/**
 * @brief Sets attack timer
*/
void Enemy::setAttackTimer()
{
    attackStartTime = chrono::steady_clock::now();
}

/**
 * @brief Generates positions
 * @param grid The grid to spawn on
 * @param player The player
 * @param enemyList The enemy list
 * @param enemyCounter How many enemies to spawn
*/
void Enemy::generateEnemyPositions(Grid_t& grid, Player& player, vector <Enemy>& enemyList, int enemyCounter)
{
    for (int i = 0; i < enemyCounter; i++)
    {
        int xEnemy, yEnemy;
        do
        {
            auto it = begin(grid);
            advance(it, rand() % grid.size());
            pair<int, int> randomCell = it->first;

            xEnemy = randomCell.first * ENEMY_WIDTH;
            yEnemy = randomCell.second * ENEMY_HEIGHT;

            bool tooCloseToPlayer = (abs(xEnemy - player.getAsset().x) < PLAYER_WIDTH + MIN_DISTANCE_BETWEEN_PLAYER_AND_ENEMY) || (abs(yEnemy - player.getAsset().y) < PLAYER_HEIGHT + MIN_DISTANCE_BETWEEN_PLAYER_AND_ENEMY);
            if (!grid[randomCell] && !tooCloseToPlayer)
            {
                grid[randomCell] = true;
                break;
            }

        } while(true);
        
        Enemy newEnemy({xEnemy, yEnemy, ENEMY_WIDTH, ENEMY_HEIGHT});
        enemyList.push_back(newEnemy);
    }
}

/**
 * @brief Renders the enemy
 * @param currentEnemy The current enemy
*/
void Enemy::render(Enemy currentEnemy)
{
    Window::drawAnimation(currentEnemy.getSrcRect(), currentEnemy.getAsset(), enemyTexture, currentEnemy.getFlip());
}


#endif