#ifndef GAME_CPP
#define GAME_CPP

#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
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
#include "ladder.hpp"
#include "startScreen.hpp"
#include "gameSettings.hpp"

using namespace std;

const int ARENA_WIDTH = 83, ARENA_HEIGHT = 83;
const int PLAYER_WIDTH = 117, PLAYER_HEIGHT = 140;
const int ENEMY_WIDTH = 98, ENEMY_HEIGHT = 166;
const int LADDER_WIDTH = 98, LADDER_HEIGHT = 98;

const int ANIMATION_SPEED = 200;

const int SIGHT_RANGE = 2000;
const int ENEMY_RANGE = 500;
const int INTERACTION_RANGE_ARENA = SIGHT_RANGE + ARENA_WIDTH - 150;
const int INTERACTION_RANGE_LADDER = SIGHT_RANGE + LADDER_WIDTH - 50;

const int MIN_DISTANCE_BETWEEN_ARENAS = 10;
const int MIN_DISTANCE_BETWEEN_PLAYER_AND_ENEMY = 50;

class Game
{
    private:
        ofstream highscoresData;
        ofstream saveFile;
        ofstream replayFile;
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
        void updateHighscores();
        void continueGame();
        void closeSaveFile();
        void closeReplayFile();
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

    highscoresData.open("highscores.txt", ios::app);
    
    playerTexture = LOAD_TEXTURE(window.getRenderer(), "assets/player_remastared.png");
    enemyTexture = LOAD_TEXTURE(window.getRenderer(), "assets/enemy_reloaded.png");
    arenaTexture = LOAD_TEXTURE(window.getRenderer(), "assets/arena.png");
    ladderTexture = LOAD_TEXTURE(window.getRenderer(), "assets/ladder.png");
    hearts_1Texture = LOAD_TEXTURE(window.getRenderer(), "assets/3_hearts_reloaded.png");
    hearts_2Texture = LOAD_TEXTURE(window.getRenderer(), "assets/2_hearts_reloaded.png");
    hearts_3Texture = LOAD_TEXTURE(window.getRenderer(), "assets/1_hearts_reloaded.png");

    // makes grid for spawns
    for (int i = 0; i < GameSettings::WIDTH / ENEMY_WIDTH; i++)
        for (int j = 0; j < GameSettings::HEIGHT / ENEMY_HEIGHT; j++)
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

            xArena = randomCell.first * ARENA_WIDTH;
            yArena = randomCell.second * ARENA_HEIGHT;

            bool tooCloseToExistingArena = false;
            for (const auto& entry : arenaList)
            {
                int distanceX = abs(xArena - entry.second.getAsset().x);
                int distanceY = abs(yArena - entry.second.getAsset().y);

                if (distanceX < MIN_DISTANCE_BETWEEN_ARENAS || distanceY < MIN_DISTANCE_BETWEEN_ARENAS)
                {
                    tooCloseToExistingArena = true;
                    break;
                }
            }

            if (grid[randomCell] || arenaList.count(i) > 0 || tooCloseToExistingArena)
                continue;

            break;

        } while (true);

        arenaList.insert({i, Arena({xArena, yArena, ARENA_WIDTH, ARENA_HEIGHT})});
        grid[{xArena / ARENA_WIDTH, yArena / ARENA_HEIGHT}] = true;
    }   

    this->player = Player(3, {max((rand() % GameSettings::WIDTH - PLAYER_WIDTH), 0), max((rand() % GameSettings::HEIGHT - PLAYER_HEIGHT), 0), PLAYER_WIDTH, PLAYER_HEIGHT});
    
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

    this->ladder = Ladder({max((rand() % GameSettings::WIDTH - LADDER_WIDTH), 0), max((rand() % GameSettings::HEIGHT - LADDER_HEIGHT), 0), LADDER_WIDTH, LADDER_HEIGHT});
}

void Game::update()
{
    if (!saveFile.is_open())
        saveFile.open("saveFile.txt");

    if (!replayFile.is_open())
        replayFile.open("replayFile.txt");

    // only writes in file if there is a change
    static string tmpName = "31dwqd"; // random becuse i dont type name 
    if (tmpName != Data::playerName)
    {
        saveFile << "Name: " << Data::playerName << "\n"; 

        tmpName = Data::playerName;
    }

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
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            Data::inPauseScreen = true;
            saveFile.close();
        }
    }
    player.movePlayer();

    static int tmpPlayerX = 0, tmpPlayerY = 0;
    if (tmpPlayerX != player.getAsset().x || tmpPlayerY != player.getAsset().y)
    {
        saveFile << "Player: " << player.getAsset().x << "\t" <<  player.getAsset().y << "\n";
        replayFile << "Player: " << player.getAsset().x << "\t" << player.getAsset().y << "\n";

        tmpPlayerX = player.getAsset().x;
        tmpPlayerY = player.getAsset().y;
    }

    // arena visibility
    for (auto& entry : arenaList)
    {
        Arena& currentArena = entry.second;

        if (player.isNearby(player.getAsset(), currentArena.getAsset(), SIGHT_RANGE))
        {
            currentArena.setVisible(true);

            isCloseTo = entry.first;
        }
        else
            currentArena.setVisible(false);

        static vector<int> tmpArenaX(2, 0), tmpArenaY(2, 0);
        if (tmpArenaX[entry.first] != currentArena.getAsset().x || tmpArenaY[entry.first] != currentArena.getAsset().y)
        {
            saveFile << "Arena: " << currentArena.getAsset().x << "\t" << currentArena.getAsset().y << "\n";
            replayFile << "Arena: " << currentArena.getAsset().x << "\t" << currentArena.getAsset().y << "\n";

            tmpArenaX[entry.first] = currentArena.getAsset().x;
            tmpArenaY[entry.first] = currentArena.getAsset().y;
        }
    }

    // renders arena
    for (auto& entry : arenaList)
    {
        Arena& currentArena = entry.second;

        if (currentArena.getVisible() || currentArena.isForcedVisible())
            window.draw(window.getRenderer(), currentArena.getAsset(), arenaTexture);
        
        if (player.isNearby(player.getAsset(), currentArena.getAsset(), INTERACTION_RANGE_ARENA))
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
        player.updatePlayerAnimation(ANIMATION_SPEED);

    for (auto& currentEnemy : enemyList)
    {
        currentEnemy.updateEnemyAI(player, ENEMY_RANGE, ANIMATION_SPEED);

        // vector<int> tmpEnemyX, tmpEnemyY;
        // if (tmpEnemyX[currentEnemy] != currentEnemy.getAsset().x || tmpEnemyY != currentEnemy.getAsset().y)
        // {
        //     saveFile << "Enemy: " <<  currentEnemy.getAsset().x << "\t" << currentEnemy.getAsset().y <<  "\n";
        //     replayFile << "Enemy: " <<  currentEnemy.getAsset().x << "\t" << currentEnemy.getAsset().y <<  "\n";

        //     tmpEnemyX = currentEnemy.getAsset().x;
        //     tmpEnemyY = currentEnemy.getAsset().y;
        // }
    }
    

    //spawns door for next lvl 
    if (arenaList.empty())
    {
        window.draw(window.getRenderer(), ladder.getAsset(), ladderTexture);

        if (player.isNearby(player.getAsset(), ladder.getAsset(), INTERACTION_RANGE_LADDER))
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
    static int tmpLadderX = 0, tmpLadderY = 0;
    if (tmpLadderX != ladder.getAsset().x || tmpLadderY != ladder.getAsset().y)
    {
        saveFile << "Ladder: " << ladder.getAsset().x << "\t" << ladder.getAsset().y << "\n";
        replayFile << "Ladder: " << ladder.getAsset().x << "\t" << ladder.getAsset().y << "\n";

        tmpLadderX = ladder.getAsset().x;
        tmpLadderY = ladder.getAsset().y;
    }

    static int tmpLevel = 0;
    if (tmpLevel != level.getLevel())
    {
        saveFile << "Level: " << level.getLevel() << "\n";
        replayFile << "Level: " << level.getLevel() << "\n";

        tmpLevel = level.getLevel();
    }

    static int tmpPoints = 0;
    if (tmpPoints != points)
    {
        saveFile << "Points: " << points << "\n";
        replayFile << "Points: " << points << "\n";

        tmpPoints = points;
    }

    // saveFile.close();
    // replayFile.close();
}

void Game::render()
{
    window.drawAnimation(window.getRenderer(), player.getSrcRect(), player.getAsset(), playerTexture, player.getFlip());
    for (auto& currentEnemy : enemyList)
        window.drawAnimation(window.getRenderer(),currentEnemy.getSrcRect(), currentEnemy.getAsset(), enemyTexture, currentEnemy.getFlip());
        
    if (arenaList.size() < level.getArenaCounter())
        window.createText(window.getRenderer(), ("Remaining arenas: " + to_string(arenaList.size())).c_str(), GameSettings::WIDTH, 50);

    if (player.isNearArena())
        window.createText(window.getRenderer(), "Save bull [E]", (GameSettings::WIDTH / 2) + 100, GameSettings::HEIGHT - 50);

    if (player.isNearLadder())
        window.createText(window.getRenderer(), ("Next level [F]"), (GameSettings::WIDTH / 2) + 100, GameSettings::HEIGHT - 50);

    window.createText(window.getRenderer(), ("Level: " + to_string(level.getLevel())).c_str(), GameSettings::WIDTH / 2, 0);
    window.createText(window.getRenderer(), ("Points: " + to_string(points)).c_str(), GameSettings::WIDTH, 0);

    window.drawPlayerHealth(player.getHealth(), hearts_1Texture, hearts_2Texture, hearts_3Texture);

    // game over screen
    if (!(Data::isPlayerAlive))
    {
        replayFile.close();

        SDL_RenderClear(window.getRenderer());     
        SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 0);

        window.createText(window.getRenderer(), "Game over", GameSettings::WIDTH / 2, GameSettings::HEIGHT / 2);
        window.createText(window.getRenderer(), "Press [R] to restart", GameSettings::WIDTH / 2, GameSettings::HEIGHT / 2 + 200);

        window.present();

        updateHighscores();
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
    for (int i = 0; i < GameSettings::WIDTH / ENEMY_WIDTH; i++)
        for (int j = 0; j < GameSettings::HEIGHT / ENEMY_HEIGHT; j++)
            grid.insert({(make_pair(i,j)), false});

    player.setX(max((rand() % (GameSettings::WIDTH - PLAYER_WIDTH)), 0));
    player.setY(max((rand() % (GameSettings::HEIGHT - PLAYER_HEIGHT)), 0));
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

            xArena = randomCell.first * ARENA_WIDTH;
            yArena = randomCell.second * ARENA_HEIGHT;

            bool tooCloseToExistingArena = false;
            for (const auto& entry : arenaList)
            {
                int distanceX = abs(xArena - entry.second.getAsset().x);
                int distanceY = abs(yArena - entry.second.getAsset().y);

                if (distanceX < MIN_DISTANCE_BETWEEN_ARENAS || distanceY < MIN_DISTANCE_BETWEEN_ARENAS)
                {
                    tooCloseToExistingArena = true;
                    break;
                }
            }

            if (grid[randomCell] || arenaList.count(i) > 0 || tooCloseToExistingArena)
                continue;

            break;

        } while (true);

        arenaList.insert({i, Arena({xArena, yArena, ARENA_WIDTH, ARENA_HEIGHT})});
        grid[{xArena / ARENA_WIDTH, yArena / ARENA_HEIGHT}] = true;
    }   

    
    enemyList.clear();
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

    ladder.setX(max((rand() % GameSettings::WIDTH - LADDER_WIDTH), 0));
    ladder.setY(max((rand() % GameSettings::HEIGHT - LADDER_HEIGHT), 0));

    points = 0;
}

void Game::updateHighscores()
{
    vector<pair<string,int>> highscores;
    
    ifstream highscoresInput("highscores.txt");
    string line;

    while(getline(highscoresInput, line))
    {
        if (!line.empty())
        {
            size_t tabPos = line.find_first_of(" \t");
            if (tabPos != string::npos)
            {
                string playerName = line.substr(0, tabPos);
                int score = stoi(line.substr(tabPos + 1));
                highscores.push_back({playerName, score});
            }
        }
    }
    highscoresInput.close();

    auto playerIterator = find_if(highscores.begin(), highscores.end(), [this](const auto &entry) {
        return entry.first == Data::playerName;
    });

    if (playerIterator != highscores.end())
        playerIterator->second = max(playerIterator->second, points);
    else
        highscores.push_back({Data::playerName, points});

    sort(highscores.begin(), highscores.end(), [](const auto &a, const auto &b) -> bool 
    {
        return a.second > b.second;
    });

    highscoresData.open("highscores.txt", ios::trunc);
    highscores.resize(min(5, (int)highscores.size()));

    for (const auto &entry : highscores)
        highscoresData << left << setw(20) << entry.first << setw(5)<< entry.second << "\n";

    highscoresData.close();
}

void Game::continueGame()
{
    ifstream readSaveFile;
    readSaveFile.open("saveFile.txt");

    string line;

    int enemyCounter = 1;
    while (getline(readSaveFile, line))
    {
        istringstream iss(line);
        string key, value;
        int value1, value2;


        while (iss >> key)
        {
            if (key == "Name:")
            {
                iss >> value;
                Data::playerName = value;
            }

            else if (key == "Player:")
            {
                iss >> value1 >> value2;
                player.setX(value1);
                player.setY(value2);
            }
            else if (key == "Arena:")
            {
                iss >> value1 >> value2;

                static int arenaCounter = 1;

                if (!arenaList.empty())
                {
                    Arena& currentArena = arenaList[arenaCounter % arenaList.size()];
                    currentArena.setX(value1);
                    currentArena.setY(value2);

                }
                
                arenaCounter++;
            }
            else if (key == "Enemy:")
            {
                iss >> value1 >> value2;

                if (enemyCounter == 1)
                {
                    enemyList.clear();
                    level.setEnemyCounter(1);
                }

                Enemy newEnemy({value1, value2, ENEMY_WIDTH, ENEMY_HEIGHT});
                newEnemy.setBounds();
                enemyList.push_back(newEnemy);

                level.setEnemyCounter(enemyCounter);

                enemyCounter++;
            }
            else if (key == "Points:")
            {
                iss >> value1;
                points = value1;
            }
            else if (key == "Level:")
            {
                iss >> value1;
                level.setLevel(value1);
            }
        }
    }
}

void Game::closeSaveFile()
{
    saveFile.close();
}

void Game::closeReplayFile()
{
    replayFile.close();
}

#endif







// writes in saveFile replay stuff ??
// sometimes crash?
// enemy replay saving