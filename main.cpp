#include <iostream>
#include <unordered_map>
#include <time.h>
#include <chrono>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "headers/player.hpp"
#include "headers/window.hpp"
#include "headers/arena.hpp"
#include "headers/enemy.hpp"
#include "headers/text.hpp"
#include "headers/ladder.hpp"

using namespace std;

#define LOAD_TEXTURE(renderer, imgPath) SDL_CreateTextureFromSurface(renderer, IMG_Load(imgPath))

const int WIDTH = 1720, HEIGHT = 820;

const int arenaWidth = 128, arenaHeight = 128;
const int playerWidth = 180, playerHeight = 216;
const int enemyWidth = 149, enemyHeight = 256;
const int ladderWidth = 150, ladderHeight = 150;

const int animationSpeed = 200;

const int sightRange = 400;
const int enemyRange = 300;

int points = 0;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    
    Window window("Reševanje bikca Ferdinanda", WIDTH, HEIGHT);
    window.init();

    SDL_Texture *playerTexture = LOAD_TEXTURE(window.getRenderer(), "assets/player_remastared.png");
    SDL_Texture *enemyTexture = LOAD_TEXTURE(window.getRenderer(), "assets/enemy_reloaded.png");
    SDL_Texture *arenaTexture = LOAD_TEXTURE(window.getRenderer(), "assets/arena.png");
    SDL_Texture *ladderTexture = LOAD_TEXTURE(window.getRenderer(), "assets/ladder.png");
    SDL_Texture *hearts_1Texture = LOAD_TEXTURE(window.getRenderer(), "assets/3_hearts_reloaded.png");
    SDL_Texture *hearts_2Texture = LOAD_TEXTURE(window.getRenderer(), "assets/2_hearts_reloaded.png");
    SDL_Texture *hearts_3Texture = LOAD_TEXTURE(window.getRenderer(), "assets/1_hearts_reloaded.png");

    unordered_map <int, Arena> arenaList;

    Level level;
    level.setArenaCounter(2 + rand() % 2);

    for (int i=0; i < level.getArenaCounter(); i++)
    {
        int xArena = max((rand() % WIDTH - 200), 0);
        int yArena = max((rand() % HEIGHT - 200), 0);

        for (const auto& entry : arenaList)
        {
            if (entry.second.getAsset().x == xArena)
                xArena = max((rand() % WIDTH - 200), 0);
            
            if (entry.second.getAsset().y == yArena)
                yArena = max((rand() % HEIGHT - 200), 0);
        }
        arenaList.insert({i, Arena({xArena, yArena, arenaWidth, arenaHeight})});
    }

    Player player(3, {max((rand() % WIDTH - 180), 0), max((rand() % HEIGHT - 216), 0), playerWidth, playerHeight});
    
    int xEnemy, yEnemy;
    do
    {
        xEnemy = max((rand() % WIDTH - enemyWidth), 0);
        yEnemy =  max((rand() % HEIGHT - enemyHeight), 0);
        
    } while (!(xEnemy < player.getAsset().x - 200 || xEnemy > player.getAsset().x + 200) && !(yEnemy < player.getAsset().y - 200 || yEnemy > player.getAsset().y + 200));

    Enemy newEnemy({xEnemy, yEnemy, enemyWidth, enemyHeight});

    vector <Enemy> enemyList;
    enemyList.push_back(newEnemy);

    Ladder ladder({max((rand() % WIDTH - 150), 0), max((rand() % HEIGHT - 150), 0), ladderWidth, ladderHeight});

    Text text("fonts/test.ttf", 50);

    const int fps = 120;
    int frameDelay = 1000 / fps;
    Uint32 frameStart;
    int frameTime;

    int isCloseTo = -1;

    while (player.getIsPlayerAlive())
    {
       frameStart = SDL_GetTicks();

        player.setNearArena(false);
        player.setNearLadder(false);

        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
                break;
            if (event.type == SDL_KEYDOWN) 
                player.movePlayer(event.key.keysym.sym); 
            else if (event.type == SDL_KEYUP)
                player.setIsMoving(false);
        }

        window.clear();

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
            
            if (player.isNearby(player.getAsset(), currentArena.getAsset(), sightRange - 200))
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

        player.updatePlayerAnimation(animationSpeed);
        for (auto& currentEnemy : enemyList)
            currentEnemy.updateEnemyAI(player, enemyRange, animationSpeed);

        //spawns door for next lvl 
        if (arenaList.empty())
        {
            window.draw(window.getRenderer(), ladder.getAsset(), ladderTexture);

            if (player.isNearby(player.getAsset(), ladder.getAsset(), sightRange - 200))
            {
                player.setNearLadder(true);
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_f)
                    {
                        level.resetGame(player, enemyList, arenaList, ladder, isCloseTo, player.getHealth(), WIDTH, HEIGHT);
                        level.setLevel();
                    }
                }
            }
        }

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

        //Game over
        if (!(player.getIsPlayerAlive()))
        {
            SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 0);
            SDL_RenderClear(window.getRenderer());

            text.createText(window.getRenderer(), "Game over", WIDTH / 2, HEIGHT / 2);

            window.present();

            SDL_Delay(1000);
        }

        window.present();  

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    text.~Text();
    window.~Window();

    return EXIT_SUCCESS;
}
