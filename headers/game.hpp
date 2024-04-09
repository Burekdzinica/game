#ifndef GAME_CPP
#define GAME_CPP

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "player.hpp"
#include "window.hpp"
#include "arena.hpp"
#include "ladder.hpp"
#include "gameSettings.hpp"
#include "map.hpp"
#include "enemy.hpp"
#include "level.hpp"
#include "spear.hpp"
#include "audioManager.hpp"

using namespace std;

const int ARENA_WIDTH = 62, ARENA_HEIGHT = 62;
const int PLAYER_WIDTH = 88, PLAYER_HEIGHT = 105;
const int ENEMY_WIDTH = 74, ENEMY_HEIGHT = 125;
const int LADDER_WIDTH = 74, LADDER_HEIGHT = 74;

const int ANIMATION_SPEED = 200;

//const int SIGHT_RANGE = 200;
const int SIGHT_RANGE = 2000;
const int ENEMY_RANGE = 450;
const int INTERACTION_RANGE_ARENA = SIGHT_RANGE + ARENA_WIDTH - 150;
const int INTERACTION_RANGE_LADDER = SIGHT_RANGE + LADDER_WIDTH - 50;

const int MIN_DISTANCE_BETWEEN_ARENAS = 10;
const int MIN_DISTANCE_BETWEEN_PLAYER_AND_ENEMY = 50;

struct PlayerPosition
{
    int x, y;
};

enum class GameState
{
    Playing,
    ResetGame,
    ContinueGame,
    ContinueScreen,
    Replay,
    StartScreen,
    PauseScreen,
    GameOver,
};

class Game
{
    private:
        ofstream highscoresData;
        ofstream saveFile;
        ofstream replayFile;

        chrono::steady_clock::time_point startTime;

        SDL_Texture *hearts_1Texture;
        SDL_Texture *hearts_2Texture;
        SDL_Texture *hearts_3Texture;
        
        static GameState gameState;

        Window window;
        AudioManager audioManager;
        Arena arena;
        Spear* spear;
        Map map;
        Player* player;
        Enemy enemy;
        Level level;
        Ladder* ladder = nullptr;

        vector <Enemy> enemyList;
        vector<PlayerPosition> replayList;
        Grid_t grid;
        unordered_map <int, Arena> arenaList;

        int points;
        int previousPoints;
        int isCloseTo;

        bool open;
        bool spearDeleted;
        bool isLadderSpawned;
        bool gameEnded;


        SDL_Texture* spearTexture;
        SDL_Texture* ladderTexture;

    public:
        Game();
        ~Game();

        void loadTextures();

        void setup();
        
        void eventHandler();
        void update();
        void render();

        void destroyArena();
        void climbLadder();

        void deleteSpearIfSave();
        void enemyCollision();
        void arenaVisibility();
        void spearPickup();
        void spawnNewSpear();
        void setNearby();
        void spawnLadder();

        void renderUI();

        void gameOver();
        void renderGameOver();

        bool isOpen();
        void restart();
        void updateHighscores();
        void continueGame();

        void save();
        void replay();

        // for delay 
        long long getCurrentTime();
        void delay(int milliseconds);
        void startTimer();
        long long getElapsedTime();

        void saveReplayToList();
        void saveReplay();

        int getLowestScore();
        
        static void setGameState(GameState newGameState);
        static GameState getGameState();

        void renderSpear();
        void renderLadder();

};

// Default gameState
GameState Game::gameState = GameState::ContinueScreen;

/**
 * @brief Default contructor for Game, calling Map, Ladder, Player constructor
*/
Game::Game() : map(), player(new Player(3, {max((rand() % GameSettings::WIDTH - PLAYER_WIDTH), 0), 
                                            max((rand() % GameSettings::HEIGHT - PLAYER_HEIGHT), 0), PLAYER_WIDTH, PLAYER_HEIGHT}))
{
    this->points = 0;
    this-> isCloseTo = -1;
    this->previousPoints = 0;

    this->open = true;
    //this->spearDeleted = false;
    this->isLadderSpawned = false;
    this->gameEnded = false;
}

/**
 * @brief Destructor for Game
*/
Game::~Game()
{
    SDL_DestroyTexture(hearts_1Texture);
    SDL_DestroyTexture(hearts_2Texture);
    SDL_DestroyTexture(hearts_3Texture);

    delete player;
    delete ladder;
    delete spear;
}

/**
 * @brief Loads all the textures
*/
void Game::loadTextures()
{
    hearts_1Texture = Window::loadTexture("assets/3_hearts_reloaded.png");
    hearts_2Texture = Window::loadTexture("assets/2_hearts_reloaded.png");
    hearts_3Texture = Window::loadTexture("assets/1_hearts_reloaded.png");
    spearTexture = Window::loadTexture("assets/spear.png");
    ladderTexture = Window::loadTexture("assets/ladder.png");
}

/**
 * @brief Setup
*/
void Game::setup()
{
    srand(time(NULL));

    highscoresData.open("files/highscores.txt", ios::app);

    loadTextures();

    // makes grid for spawns
    for (int i = 0; i < GameSettings::WIDTH / ENEMY_WIDTH; i++)
        for (int j = 0; j < GameSettings::HEIGHT / ENEMY_HEIGHT; j++)
            grid.insert({(make_pair(i,j)), false});

    spear = new Spear;
    spear->spawnSpear(grid);

    level.setArenaCounter(2 + rand() % 2);

    Arena::generateArenaPositions(level.getArenaCounter(), grid, arenaList);
    Enemy::generateEnemyPositions(grid, *player, enemyList, 1); 

    audioManager.playMusic(3);
}

/**
 * @brief Event handler
*/
void Game::eventHandler()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            this->open = false;
            return;
        }
        if (Data::isPlayerAlive)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                audioManager.playMusic(2);
                Game::setGameState(GameState::PauseScreen);
                replayFile.close();
            }
            if (event.type == SDL_KEYUP)
            {
                player->setFlip(SDL_FLIP_NONE);
                player->setState(PlayerState::Idle);
            }   

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_e && isCloseTo >= 0 && player->isNearArena())
                    destroyArena();

                if (ladder != nullptr && player->isNearLadder())
                {
                    if (event.key.keysym.sym == SDLK_f)
                    {
                        //cout << "d";
                        climbLadder();     
                    }
                }
            }
        }
    }
    player->movePlayer();
}

/**
 * @brief Update objects
*/
void Game::update()
{
    saveReplayToList();

    // If game played by save remove spear
    if (!spearDeleted)
    {
        deleteSpearIfSave();
        spearDeleted = true;
    }

    if (arenaList.empty() && !isLadderSpawned)
        spawnLadder();

    setNearby();
 
    if (points >= this->previousPoints + 1000)
        spawnNewSpear();

    if (spear->isPlayerTouching(player->getAsset()) && !spear->isSpearTouched())
        spearPickup();
 
    enemyCollision();

    // updates enemy AI
    for (auto& currentEnemy : enemyList)
        currentEnemy.updateEnemyAI(*player, ENEMY_RANGE, ANIMATION_SPEED);

    arenaVisibility();

    // changes player animation
    if (player->getState() == PlayerState::Idle)
        player->updatePlayerAnimation(ANIMATION_SPEED);

    if (!Data::isPlayerAlive)
        Game::setGameState(GameState::GameOver);
}

/**
 * @brief Renders the changes
*/
void Game::render()
{
    Window::clear();

    map.drawMap();

    renderLadder();
  
    for (auto& currentArena : arenaList)
        arena.render(currentArena.second);

    renderSpear();
    player->render();

    for (auto& currentEnemy : enemyList)
        enemy.render(currentEnemy);

    renderUI();

    Window::present();
}

void Game::renderSpear()
{
    if (spear != nullptr)
        Window::draw(spearTexture, spear->getAsset());
}

void Game::renderLadder()
{
    if (ladder != nullptr)
        Window::draw(this->ladderTexture, ladder->getAsset());  
}

/**
 * @brief Destroy arena
*/
void Game::destroyArena()
{
    audioManager.playSound(6);

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

/**
 * @brief Climbs ladder
*/
void Game::climbLadder()
{
    audioManager.playSound(9);


    level.increaseLevel();
    level.nextLevel(*player, enemyList, arenaList, *ladder, isCloseTo, player->getHealth());

    this->isLadderSpawned = false;

    delete ladder;
    ladder = nullptr;
}

/**
 * @brief Deletes spear pickup if you continue from save
*/
void Game::deleteSpearIfSave()
{
    if (player->getAttack() > 0)
    {
        player->setAttack(0);
        delete spear;
        spear = nullptr;
    }
}

/**
 * @brief Logic when enemy touches player
*/
void Game::enemyCollision()
{
    if (player->getAttack() > 0)
    {
        for (auto it = enemyList.begin(); it != enemyList.end();)
        {
            if (player->isPlayerTouching(it->getAsset()))
            {
                audioManager.playSound(3);

                it = enemyList.erase(it);
                player->decreaseAttack();
                // level.decreaseEnemyCounter();

                points += 200;
            }
            else
                ++it;
        }
    }
    else 
    {
        for (auto& currentEnemy : enemyList)
        {
            if (currentEnemy.isPlayerTouching(player->getAsset()))
            {
                audioManager.playSound(2);

                player->changeHealth(-1);

                //moves enemy a bit / depends on pos
                if (player->getAsset().x > currentEnemy.getAsset().x)
                    currentEnemy.setX(max((currentEnemy.getAsset().x - 50), 0));
        
                else if (player->getAsset().x < currentEnemy.getAsset().x)
                    currentEnemy.setX(max((currentEnemy.getAsset().x + 50), 0));
                
                if (player->getAsset().y > currentEnemy.getAsset().y)
                    currentEnemy.setY(max((currentEnemy.getAsset().y - 50), 0));
                
                else if (player->getAsset().y < currentEnemy.getAsset().y)
                    currentEnemy.setY(max((currentEnemy.getAsset().y + 50), 0));

                currentEnemy.setState(EnemyState::Attacked);
                currentEnemy.setAttackTimer();
            }
        }   
    }
}

/**
 * @brief If player near arena becomes visible
*/
void Game::arenaVisibility()
{
    // arena visibility
    for (auto& entry : arenaList)
    {
        Arena& currentArena = entry.second;

        if (player->isNearby(player->getAsset(), currentArena.getAsset(), SIGHT_RANGE))
        {
            currentArena.setVisible(true);

            isCloseTo = entry.first;
        }
        else
            currentArena.setVisible(false);
    }
}

/**
 * @brief Logic for spear pickup
*/
void Game::spearPickup()
{
    player->increaseAttack();
    spear->setSpearTouched();

    delete spear;
    spear = nullptr;
}

/**
 * @brief Spawns new spear
*/
void Game::spawnNewSpear()
{
    audioManager.playSound(8);

    spear = new Spear;
    spear->spawnSpear(grid);

    this->previousPoints += 1000; 
}

/**
 * @brief Sets arena/ladder isNear
*/
void Game::setNearby()
{
    // sets arena/ladder not near
    player->setNearArena(false);
    if (ladder != nullptr)
    {
        if (!(player->isNearby(player->getAsset(), ladder->getAsset(), INTERACTION_RANGE_LADDER)))
            player->setNearLadder(false);
        else if (player->isNearby(player->getAsset(), ladder->getAsset(), INTERACTION_RANGE_LADDER))
                player->setNearLadder(true);
    }

    for (auto& entry : arenaList)
    {
        Arena& currentArena = entry.second;

        if (player->isNearby(player->getAsset(), currentArena.getAsset(), INTERACTION_RANGE_ARENA))
            player->setNearArena(true);
    }
}

/**
 * @brief Spawns ladder
*/
void Game::spawnLadder()
{
    audioManager.playSound(4);

    ladder = new Ladder ({max((rand() % GameSettings::WIDTH - LADDER_WIDTH), 0), max((rand() % GameSettings::HEIGHT - LADDER_HEIGHT), 0), LADDER_WIDTH, LADDER_HEIGHT});

    this->isLadderSpawned = true;
}

/**
 * @brief Renders UI
*/
void Game::renderUI()
{
    Window::createText(("Attack: " + to_string(player->getAttack())).c_str(), GameSettings::WIDTH, GameSettings::HEIGHT - 30);

    if (arenaList.size() < level.getArenaCounter())
        Window::createText(("Remaining arenas: " + to_string(arenaList.size())).c_str(), GameSettings::WIDTH, 50);

    if (player->isNearArena())
        Window::createText("Save bull [E]", (GameSettings::WIDTH / 2) + 160, GameSettings::HEIGHT - 35);

    if (player->isNearLadder())
        Window::createText(("Next level [F]"), (GameSettings::WIDTH / 2) + 180, GameSettings::HEIGHT - 35);

    Window::createText(("Level: " + to_string(level.getLevel())).c_str(), GameSettings::WIDTH / 2 + 100, 0);
    Window::createText(("Points: " + to_string(points)).c_str(), GameSettings::WIDTH, 0);

    Window::drawPlayerHealth(player->getHealth(), hearts_1Texture, hearts_2Texture, hearts_3Texture);
}

/**
 * @brief Plays game over screen
*/
void Game::gameOver()
{
    // renders only once
    if (this->gameEnded == false)
    {
        renderGameOver();
        this->gameEnded = true;
    }

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            this->open = false;
            return;
        }
   
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_r)
            {
                restart();
                Game::setGameState(GameState::Playing);
                this->gameEnded = false;

                audioManager.playMusic(1);
            }
        }
    }
}

/**
 * @brief Renders game over screen.
 * Saves the score and replay.
 * 
*/
void Game::renderGameOver()
{
    replayFile.close();

    audioManager.playSound(1);
    audioManager.playMusic(4);

    Window::clear();    

    SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 0);

    Window::createText("Game over", GameSettings::WIDTH / 2, GameSettings::HEIGHT / 2);
    Window::createText("Press [R] to restart", GameSettings::WIDTH / 2, GameSettings::HEIGHT / 2 + 200);

    Window::present();

    updateHighscores();

    if (points < getLowestScore() || points == 0)
        saveReplay();
}


/**
 * @brief Returns if game is open
 * @return True or False
*/
bool Game::isOpen()
{
    return this->open;
}

/**
 * @brief Restarts game
*/
void Game::restart()
{
    srand(time(NULL));

    spear = new Spear;
    spear->spawnSpear(grid);

    Data::isPlayerAlive = true;

    replayList.clear();

    grid.clear();
    // makes grid for spawns
    for (int i = 0; i < GameSettings::WIDTH / ENEMY_WIDTH; i++)
        for (int j = 0; j < GameSettings::HEIGHT / ENEMY_HEIGHT; j++)
            grid.insert({(make_pair(i,j)), false});

    player->reset(3, {max((rand() % (GameSettings::WIDTH - PLAYER_WIDTH)), 0), 
                      max((rand() % (GameSettings::HEIGHT - PLAYER_HEIGHT)), 0), PLAYER_WIDTH, PLAYER_HEIGHT});

    level.setArenaCounter(2 + rand() % 2);
    level.resetLevel();

    arenaList.clear();
    Arena::generateArenaPositions(level.getArenaCounter(), grid, arenaList);

    level.setEnemyCounter(1);
    enemyList.clear();
    Enemy::generateEnemyPositions(grid, *player, enemyList, 1);

    points = 0;

    player->setNearLadder(false);
}

/**
 * @brief Updates highscores.
 * To container then writes to file.
*/
void Game::updateHighscores()
{
    vector<pair<string,int>> highscores;
    
    ifstream highscoresInput("files/highscores.txt");
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

    highscoresData.open("files/highscores.txt", ios::trunc);
    highscores.resize(min(5, (int)highscores.size()));

    for (const auto &entry : highscores)
        highscoresData << left << setw(20) << entry.first << setw(5)<< entry.second << "\n";

    highscoresData.close();
}

/**
 * @brief Contiunues from save file
*/
void Game::continueGame()
{
    ifstream readSaveFile;
    readSaveFile.open("files/saveFile.txt");

    this->spearDeleted = false;

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
                player->setX(value1);
                player->setY(value2);
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
            else if (key == "Attack:")
            {
                iss >> value1;
                player->setAttack(value1);
            }
        }
    }
    Game::setGameState(GameState::Playing);
}

/**
 * @brief Saves game
*/
void Game::save()
{
    saveFile.open("files/saveFile.txt");

    if (saveFile.is_open())
    {
        saveFile << "Name: " << Data::playerName << "\n"; 
        saveFile << "Player: " << player->getAsset().x << "\t" << player->getAsset().y << "\n";

        for (auto entry : arenaList)
            saveFile << "Arena: " << entry.second.getAsset().x << "\t" << entry.second.getAsset().y << "\n";
        
        for (auto currentEnemy : enemyList)
            saveFile << "Enemy: " << currentEnemy.getAsset().x << "\t" << currentEnemy.getAsset().y << "\n";

        saveFile << "Level: " << level.getLevel() << "\n";
        saveFile << "Ladder: " << ladder->getAsset().x << "\t" << ladder->getAsset().y << "\n";
        saveFile << "Points: " << points << "\n";
        saveFile << "Attack: " << player->getAttack() << "\n";

        saveFile.close();
    }
    else
        cout << "Save file was not opened\n";
}

/**
 * @brief Plays replay.
 * Lowest score player can replay, not the most recent one (except if he's worst).
 * Chrono delay because SDL delays whole program.
*/
// void Game::replay()
// {
//     PlayerPosition playerPos;

//     ifstream readReplayFile;
//     readReplayFile.open("files/replayFile.bin", ios::binary);

//     startTimer();

//     const long long frameDuration = 15;

//     long long elapsedTime;

//     while (readReplayFile.read((char*) &playerPos, sizeof(PlayerPosition)))
//     {
//         player->setX(playerPos.x);
//         player->setY(playerPos.y);

//         render();

//         elapsedTime = getElapsedTime();
//         if (elapsedTime < frameDuration)
//             delay(frameDuration - elapsedTime);

//         startTimer();
//     }
//     readReplayFile.close();

//     Window::clear();

//     Window::createBlackScreen();
//     Window::createText("Replay over", GameSettings::WIDTH / 2 + 150, GameSettings::HEIGHT / 2);

//     Window::present();

//     delay(1250);  // if more then crash
// }


// Top with my delay --  bottom with SDL
// If a lot of replay it crashes

/**
 * @brief Plays replay.
 * Lowest score player can replay, not the most recent one (except if he's worst).
 * Chrono delay because SDL delays whole program.
*/
void Game::replay()
{
    PlayerPosition playerPos;

    ifstream readReplayFile;
    readReplayFile.open("files/replayFile.bin", ios::binary);

    while (readReplayFile.read((char*) &playerPos, sizeof(PlayerPosition)))
    {
        player->setX(playerPos.x);
        player->setY(playerPos.y);

        render();

        SDL_Delay(15);
    }
    readReplayFile.close();

    Window::clear();

    Window::createBlackScreen();
    Window::createText("Replay over", GameSettings::WIDTH / 2 + 150, GameSettings::HEIGHT / 2);

    Window::present();

    SDL_Delay(1250);
}

/**
 * @brief Gets current time
*/
long long Game::getCurrentTime() 
{
    return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
}

/**
 * @brief Delays 
 * @param milliseconds The ms time
*/
void Game::delay(int milliseconds) 
{
    long long start = getCurrentTime();

    while (getCurrentTime() - start < milliseconds)
    {
        // just waits
    }
}

/**
 * @brief Starts timer
*/
void Game::startTimer()
{
    startTime = chrono::steady_clock::now();
}

/**
 * @brief Gets elapsed time
 * @return Elapsed time
*/
long long Game::getElapsedTime() 
{
    auto endTime = chrono::steady_clock::now();

    return chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
}

/**
 * @brief Saves replay to container
*/
void Game::saveReplayToList()
{
    PlayerPosition playerPos;
    
    playerPos.x = player->getAsset().x;
    playerPos.y = player->getAsset().y;

    replayList.push_back(playerPos);
}

/**
 * @brief Saves replay to file
*/
void Game::saveReplay()
{
    if (!replayFile.is_open())
        replayFile.open("files/replayFile.bin", ios::binary);

    for (auto entry : replayList)
        replayFile.write((char*)&entry, sizeof(PlayerPosition));

    replayFile.close();
}

/**
 * @brief Gets lowest score from highscores.txt
 * @return Lowest score
*/
int Game::getLowestScore()
{
    ifstream readHighscoresData;
    readHighscoresData.open("files/highscores.txt");

    int lowestScore = INT_MAX;

    string line;

    while(getline(readHighscoresData, line))
    {
        istringstream iss(line);
        string playerName;
        int score;

        iss >> playerName >> score;

        if (score < lowestScore)
            lowestScore = score;
    }

    return lowestScore;
}

/**
 * @brief Sets game state
 * @param newGameState New game state
*/
void Game::setGameState(GameState newGameState)
{
    gameState = newGameState;
}

/**
 * @brief Gets game state
 * @return Game state
*/
GameState Game::getGameState()
{
    return gameState;
}

#endif
