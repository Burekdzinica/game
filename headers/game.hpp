#ifndef GAME_CPP
#define GAME_CPP

#include <iostream>
#include <unordered_map>
#include <time.h>
#include <chrono>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "player.hpp"
#include "window.hpp"
#include "arena.hpp"
#include "enemy.hpp"
#include "text.hpp"
#include "ladder.hpp"
#include "startScreen.hpp"
#include "gameSettings.hpp"

using namespace std;

const int arenaWidth = 83, arenaHeight = 83;
const int playerWidth = 117, playerHeight = 140;
const int enemyWidth = 98, enemyHeight = 166;
const int ladderWidth = 98, ladderHeight = 98;

const int animationSpeed = 200;

const int sightRange = 200;
const int enemyRange = 350;
const int interactionRangeArena = sightRange + arenaWidth - 150;
const int interactionRangeLadder = sightRange + ladderWidth - 50;

const int minDistanceBetweenArenas = 10;
const int minDistanceBetweenPlayerAndEnemy = 50;

class Game
{
    private:
        SDL_Texture *playerTexture;
        SDL_Texture *enemyTexture;
        SDL_Texture *arenaTexture;
        SDL_Texture *ladderTexture;
        SDL_Texture *hearts_1Texture;
        SDL_Texture *hearts_2Texture;
        SDL_Texture *hearts_3Texture;
        
        Window window;
        Player player;
        Level level;
        Ladder ladder;
        vector <Enemy> enemyList;
        unordered_map <pair<int, int>, bool, PairHash> grid;
        unordered_map <int, Arena> arenaList;
        Text text;
        int points;
        int isCloseTo;
        bool open;

    public:
        Game();
        ~Game();
        void setup();
        void update();
        void render();
        bool isOpen();
        void restart();
};

Game::Game()
{
    this->points = 0;
    this-> isCloseTo = -1;
    this->open = true;
}

Game::~Game()
{
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(arenaTexture);
    SDL_DestroyTexture(ladderTexture);
    SDL_DestroyTexture(hearts_1Texture);
    SDL_DestroyTexture(hearts_2Texture);
    SDL_DestroyTexture(hearts_3Texture);
}

void Game::setup()
{
    srand(time(NULL));
    #define LOAD_TEXTURE(renderer, imgPath) SDL_CreateTextureFromSurface(renderer, IMG_Load(imgPath))

    window.init();

    StartScreen startScreen(window.getRenderer());
    startScreen.run(window.getRenderer());
    
    playerTexture = LOAD_TEXTURE(window.getRenderer(), "assets/player_remastared.png");
    enemyTexture = LOAD_TEXTURE(window.getRenderer(), "assets/enemy_reloaded.png");
    arenaTexture = LOAD_TEXTURE(window.getRenderer(), "assets/arena.png");
    ladderTexture = LOAD_TEXTURE(window.getRenderer(), "assets/ladder.png");
    hearts_1Texture = LOAD_TEXTURE(window.getRenderer(), "assets/3_hearts_reloaded.png");
    hearts_2Texture = LOAD_TEXTURE(window.getRenderer(), "assets/2_hearts_reloaded.png");
    hearts_3Texture = LOAD_TEXTURE(window.getRenderer(), "assets/1_hearts_reloaded.png");


    // makes grid for spawns
    for (int i = 0; i < GameSettings::WIDTH / enemyWidth; i++)
        for (int j = 0; j < GameSettings::HEIGHT / enemyHeight; j++)
            grid.insert({(make_pair(i,j)), false});

    level.setArenaCounter(2 + rand() % 2);

    int xArena, yArena;
    for (int i=0; i < level.getArenaCounter(); i++)
    {
        do
        {
            auto it = begin(grid);
            advance(it, rand() % grid.size());
            pair<int, int> randomCell = it->first;

            xArena = randomCell.first * arenaWidth;
            yArena = randomCell.second * arenaHeight;

            bool tooCloseToExistingArena = false;
            for (const auto& entry : arenaList)
            {
                int distanceX = abs(xArena - entry.second.getAsset().x);
                int distanceY = abs(yArena - entry.second.getAsset().y);

                if (distanceX < minDistanceBetweenArenas || distanceY < minDistanceBetweenArenas)
                {
                    tooCloseToExistingArena = true;
                    break;
                }
            }

            if (grid[randomCell] || arenaList.count(i) > 0 || tooCloseToExistingArena)
                continue;

            break;

        } while (true);

        arenaList.insert({i, Arena({xArena, yArena, arenaWidth, arenaHeight})});
        grid[{xArena / arenaWidth, yArena / arenaHeight}] = true;
    }   

    this->player = Player(3, {max((rand() % GameSettings::WIDTH - playerWidth), 0), max((rand() % GameSettings::HEIGHT - playerHeight), 0), playerWidth, playerHeight});
    
    int xEnemy, yEnemy;
    do
    {
        auto it = begin(grid);
        advance(it, rand() % grid.size());
        pair<int, int> randomCell = it->first;

        xEnemy = randomCell.first * enemyWidth;
        yEnemy = randomCell.second * enemyHeight;

        bool tooCloseToPlayer = (abs(xEnemy - player.getX()) < playerWidth + minDistanceBetweenPlayerAndEnemy) || (abs(yEnemy - player.getY()) < playerHeight + minDistanceBetweenPlayerAndEnemy);
        if (!grid[randomCell] && !tooCloseToPlayer)
        {
            grid[randomCell] = true;
            break;
        }

    } while(true);
    
    Enemy newEnemy({xEnemy, yEnemy, enemyWidth, enemyHeight});
    enemyList.push_back(newEnemy);

    this->ladder = Ladder({max((rand() % GameSettings::WIDTH - ladderWidth), 0), max((rand() % GameSettings::HEIGHT - ladderHeight), 0), ladderWidth, ladderHeight});

    Text text("fonts/test.ttf", 50);
}

void Game::update()
{
    player.setNearArena(false);
    player.setNearLadder(false);

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            this->open = false;
            return;
        }
        if (event.type == SDL_KEYUP)
        {
            player.setFlip(SDL_FLIP_NONE);
            player.setState(PlayerState::Idle);
        }   
    }
    player.movePlayer();

    // arena visibility
    for (auto& entry : arenaList)
    {
        Arena& currentArena = entry.second;

        if (player.isNearby(player.getAsset(), currentArena.getAsset(), sightRange))
        {
            currentArena.setVisible(true);

            isCloseTo = entry.first;
        }
        else
            currentArena.setVisible(false);
    }

    // renders arena
    for (auto& entry : arenaList)
    {
        Arena& currentArena = entry.second;

        if (currentArena.getVisible() || currentArena.isForcedVisible())
            window.draw(window.getRenderer(), currentArena.getAsset(), arenaTexture);
        
        if (player.isNearby(player.getAsset(), currentArena.getAsset(), interactionRangeArena))
            player.setNearArena(true);
    }

    // deletes arena
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_e && isCloseTo >= 0 && player.isNearArena())
        {
            arenaList.erase(isCloseTo);
            
            // 20% chance to see other arena on arena pickup
            if (!arenaList.empty() && (rand() % 5 + 1) == 1)
            {
                auto firstArena = arenaList.begin();

                firstArena->second.setVisible(true);              
                firstArena->second.setForcedVisibility(true);              
            }

            points += 100;
            
            isCloseTo = -1;
        } 
    }
    if (player.getState() == PlayerState::Idle)
        player.updatePlayerAnimation(animationSpeed);
    for (auto& currentEnemy : enemyList)
        currentEnemy.updateEnemyAI(player, enemyRange, animationSpeed);

    //spawns door for next lvl 
    if (arenaList.empty())
    {
        window.draw(window.getRenderer(), ladder.getAsset(), ladderTexture);

        if (player.isNearby(player.getAsset(), ladder.getAsset(), interactionRangeLadder))
        {
            player.setNearLadder(true);
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_f)
                {
                    level.resetGame(player, enemyList, arenaList, ladder, isCloseTo, player.getHealth());
                    level.setLevel();
                }
            }
        }
    }

}

void Game::render()
{
    window.drawAnimation(window.getRenderer(), player.getSrcRect(), player.getAsset(), playerTexture, player.getFlip());
    for (auto& currentEnemy : enemyList)
        window.drawAnimation(window.getRenderer(),currentEnemy.getSrcRect(), currentEnemy.getAsset(), enemyTexture, currentEnemy.getFlip());
        
    if (arenaList.size() < level.getArenaCounter())
        text.createText(window.getRenderer(), ("Remaining arenas: " + to_string(arenaList.size())).c_str(), GameSettings::WIDTH, 50);

    if (player.isNearArena())
        text.createText(window.getRenderer(), "Save bull [E]", (GameSettings::WIDTH / 2) + 100, GameSettings::HEIGHT - 50);

    if (player.isNearLadder())
        text.createText(window.getRenderer(), ("Next level [F]"), (GameSettings::WIDTH / 2) + 100, GameSettings::HEIGHT - 50);

    text.createText(window.getRenderer(), ("Level: " + to_string(level.getLevel())).c_str(), GameSettings::WIDTH / 2, 0);
    text.createText(window.getRenderer(), ("Points: " + to_string(points)).c_str(), GameSettings::WIDTH, 0);

    window.drawPlayerHealth(player.getHealth(), hearts_1Texture, hearts_2Texture, hearts_3Texture);

    // game over screen
    if (!(Data::isPlayerAlive))
    {
        SDL_RenderClear(window.getRenderer());     
        SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 0);

        text.createText(window.getRenderer(), "Game over", GameSettings::WIDTH / 2, GameSettings::HEIGHT / 2);
        text.createText(window.getRenderer(), "Press [R] to restart", GameSettings::WIDTH / 2, GameSettings::HEIGHT / 2 + 200);

        window.present();
    }

    window.present();  
    window.clear();
}

bool Game::isOpen()
{
    return this->open;
}

void Game::restart()
{
    srand(time(NULL));

    Data::isPlayerAlive = true;

    grid.clear();
    // makes grid for spawns
    for (int i = 0; i < GameSettings::WIDTH / enemyWidth; i++)
        for (int j = 0; j < GameSettings::HEIGHT / enemyHeight; j++)
            grid.insert({(make_pair(i,j)), false});

    player.setX(max((rand() % GameSettings::WIDTH - playerWidth), 0));
    player.setY(max((rand() % GameSettings::HEIGHT - playerHeight), 0));
    player.setHealth(3);

    arenaList.clear();

    level.setArenaCounter(2 + rand() % 2);
    level.resetLevel();

    int xArena, yArena;
    for (int i=0; i < level.getArenaCounter(); i++)
    {
        do
        {
            auto it = begin(grid);
            advance(it, rand() % grid.size());
            pair<int, int> randomCell = it->first;

            xArena = randomCell.first * arenaWidth;
            yArena = randomCell.second * arenaHeight;

            bool tooCloseToExistingArena = false;
            for (const auto& entry : arenaList)
            {
                int distanceX = abs(xArena - entry.second.getAsset().x);
                int distanceY = abs(yArena - entry.second.getAsset().y);

                if (distanceX < minDistanceBetweenArenas || distanceY < minDistanceBetweenArenas)
                {
                    tooCloseToExistingArena = true;
                    break;
                }
            }

            if (grid[randomCell] || arenaList.count(i) > 0 || tooCloseToExistingArena)
                continue;

            break;

        } while (true);

        arenaList.insert({i, Arena({xArena, yArena, arenaWidth, arenaHeight})});
        grid[{xArena / arenaWidth, yArena / arenaHeight}] = true;
    }   

    
    enemyList.clear();
    int xEnemy, yEnemy;
    do
    {
        auto it = begin(grid);
        advance(it, rand() % grid.size());
        pair<int, int> randomCell = it->first;

        xEnemy = randomCell.first * enemyWidth;
        yEnemy = randomCell.second * enemyHeight;

        bool tooCloseToPlayer = (abs(xEnemy - player.getX()) < playerWidth + minDistanceBetweenPlayerAndEnemy) || (abs(yEnemy - player.getY()) < playerHeight + minDistanceBetweenPlayerAndEnemy);
        if (!grid[randomCell] && !tooCloseToPlayer)
        {
            grid[randomCell] = true;
            break;
        }

    } while(true);
    
    Enemy newEnemy({xEnemy, yEnemy, enemyWidth, enemyHeight});
    enemyList.push_back(newEnemy);

    ladder.setX(max((rand() % GameSettings::WIDTH - ladderWidth), 0));
    ladder.setY(max((rand() % GameSettings::HEIGHT - ladderHeight), 0));


    points = 0;
}



#endif