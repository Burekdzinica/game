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

using namespace std;

const int WIDTH = 1720, HEIGHT = 820;

const int arenaWidth = 128, arenaHeight = 128;
const int playerWidth = 180, playerHeight = 216;
const int enemyWidth = 149, enemyHeight = 256;
const int ladderWidth = 150, ladderHeight = 150;

const int animationSpeed = 200;

const int sightRange = 350;
const int enemyRange = 500;

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
        // Game() = default;
        void setup();
        void update();
        void render();
        bool isOpen();
        void clean();
};

Game::Game()
{
    this->points = 0;
    this-> isCloseTo = -1;
    this->open = true;
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
    for (int i = 0; i< WIDTH / enemyWidth; i++)
        for (int j = 0; j < HEIGHT / enemyHeight; j++)
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

    this->player = Player(3, {max((rand() % WIDTH - playerWidth), 0), max((rand() % HEIGHT - playerHeight), 0), playerWidth, playerHeight});
    
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

    this->ladder = Ladder({max((rand() % WIDTH - ladderWidth), 0), max((rand() % HEIGHT - ladderHeight), 0), ladderWidth, ladderHeight});

    Text text("fonts/test.ttf", 50);
    // this->text = Text("fonts/test.ttf", 50);

    // int isCloseTo = -1;
}

void Game::update()
{
    if (!player.getIsPlayerAlive())
        this->open = false;

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
        
        if (player.isNearby(player.getAsset(), currentArena.getAsset(), sightRange - 150))
            player.setNearArena(true);
    }

    // deletes arena
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_e && isCloseTo >= 0 && player.isNearArena())
        {
            arenaList.erase(isCloseTo);
            
            if (!arenaList.empty() && (rand() % 2) == 1)
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

        if (player.isNearby(player.getAsset(), ladder.getAsset(), sightRange - 150))
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
        text.createText(window.getRenderer(), ("Remaining arenas: " + to_string(arenaList.size())).c_str(), WIDTH, 50);

    if (player.isNearArena())
        text.createText(window.getRenderer(), "Save bull [E]", (WIDTH / 2) + 100, HEIGHT - 50);

    if (player.isNearby(player.getAsset(), ladder.getAsset(), sightRange - 200) && player.isNearLadder())
        text.createText(window.getRenderer(), ("Next level [F]"), (WIDTH / 2) + 100, HEIGHT - 50);

    text.createText(window.getRenderer(), ("Level: " + to_string(level.getLevel())).c_str(), WIDTH / 2, 0);
    text.createText(window.getRenderer(), ("Points: " + to_string(points)).c_str(), WIDTH, 0);

    window.drawPlayerHealth(player.getHealth(), hearts_1Texture, hearts_2Texture, hearts_3Texture);

    if (!(player.getIsPlayerAlive()))
    {
        SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 0);
        SDL_RenderClear(window.getRenderer());

        text.createText(window.getRenderer(), "Game over", WIDTH / 2, HEIGHT / 2);

        window.present();

        SDL_Delay(1000);
    }

    window.present();  
    window.clear();
}

bool Game::isOpen()
{
    return this->open;
}

void Game::clean()
{
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(arenaTexture);
    SDL_DestroyTexture(ladderTexture);
    SDL_DestroyTexture(hearts_1Texture);
    SDL_DestroyTexture(hearts_2Texture);
    SDL_DestroyTexture(hearts_3Texture);
}

#endif