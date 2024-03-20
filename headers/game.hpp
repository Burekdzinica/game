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

#include "player.hpp"
#include "window.hpp"
#include "arena.hpp"
#include "ladder.hpp"
#include "gameSettings.hpp"
#include "map.hpp"
#include "enemy.hpp"
#include "level.hpp"
#include "spear.hpp"

using namespace std;

const int ARENA_WIDTH = 62, ARENA_HEIGHT = 62;
const int PLAYER_WIDTH = 88, PLAYER_HEIGHT = 105;
const int ENEMY_WIDTH = 74, ENEMY_HEIGHT = 125;
const int LADDER_WIDTH = 74, LADDER_HEIGHT = 74;

const int ANIMATION_SPEED = 200;

const int SIGHT_RANGE = 200;
// const int SIGHT_RANGE = 2000;
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
};

class Game
{
    private:
        ofstream highscoresData;
        ofstream saveFile;
        ofstream replayFile;
        SDL_Texture *playerSpearTexture;
        SDL_Texture *playerNoSpearTexture;
        SDL_Texture *enemyTexture;
        SDL_Texture *arenaTexture;
        SDL_Texture *ladderTexture;
        SDL_Texture *hearts_1Texture;
        SDL_Texture *hearts_2Texture;
        SDL_Texture *hearts_3Texture;
        SDL_Texture* spearTexture;
        
        static GameState gameState;
        Window window;
        Spear* spear = nullptr;
        Map map;
        Player player;
        Level level;
        Ladder ladder;
        vector <Enemy> enemyList;
        vector<PlayerPosition> replayList;
        unordered_map <pair<int, int>, bool, PairHash> grid;
        unordered_map <int, Arena> arenaList;
        int points;
        int isCloseTo;
        bool open;

    public:
        Game();
        ~Game();
        void loadTextures();
        void setup();
        void eventHandler();
        void update();
        void render();
        bool isOpen();
        void restart();
        void updateHighscores();
        void continueGame();
        void save();
        void replay();
        void saveReplayToList();
        void saveReplay();
        int getLowestScore();
        static void setGameState(GameState newGameState);
        static GameState getGameState();
};

// Default gameState
GameState Game::gameState = GameState::ContinueScreen;

/**
 * @brief Default contructor for Game, calling Map constructor
*/
Game::Game() : map()
{
    this->points = 0;
    this-> isCloseTo = -1;
    this->open = true;
}

/**
 * @brief Destructor for Game
*/
Game::~Game()
{
    SDL_DestroyTexture(spearTexture);
    SDL_DestroyTexture(playerSpearTexture);
    SDL_DestroyTexture(playerNoSpearTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(arenaTexture);
    SDL_DestroyTexture(ladderTexture);
    SDL_DestroyTexture(hearts_1Texture);
    SDL_DestroyTexture(hearts_2Texture);
    SDL_DestroyTexture(hearts_3Texture);
}

/**
 * @brief Loads all the textures
*/
void Game::loadTextures()
{
    #define LOAD_TEXTURE(renderer, imgPath) SDL_CreateTextureFromSurface(renderer, IMG_Load(imgPath))

    playerSpearTexture = LOAD_TEXTURE(window.getRenderer(), "assets/player_spear.png");
    playerNoSpearTexture = LOAD_TEXTURE(window.getRenderer(), "assets/player_no_spear.png");
    enemyTexture = LOAD_TEXTURE(window.getRenderer(), "assets/enemy_reloaded.png");
    arenaTexture = LOAD_TEXTURE(window.getRenderer(), "assets/arena.png");
    ladderTexture = LOAD_TEXTURE(window.getRenderer(), "assets/ladder.png");
    hearts_1Texture = LOAD_TEXTURE(window.getRenderer(), "assets/3_hearts_reloaded.png");
    hearts_2Texture = LOAD_TEXTURE(window.getRenderer(), "assets/2_hearts_reloaded.png");
    hearts_3Texture = LOAD_TEXTURE(window.getRenderer(), "assets/1_hearts_reloaded.png");
    spearTexture = LOAD_TEXTURE(window.getRenderer(), "assets/spear.png");
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
    Enemy::generateEnemyPositions(grid, player, enemyList, 1); 

    // this->player = Player(3, {max((rand() % GameSettings::WIDTH - PLAYER_WIDTH), 0), max((rand() % GameSettings::HEIGHT - PLAYER_HEIGHT), 0), PLAYER_WIDTH, PLAYER_HEIGHT});
    this->player = Player(3, {rand() % (GameSettings::WIDTH - PLAYER_WIDTH) + PLAYER_WIDTH, rand() % (GameSettings::HEIGHT - PLAYER_HEIGHT) + PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT});

    this->ladder = Ladder({max((rand() % GameSettings::WIDTH - LADDER_WIDTH), 0), max((rand() % GameSettings::HEIGHT - LADDER_HEIGHT), 0), LADDER_WIDTH, LADDER_HEIGHT});
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
            if (event.type == SDL_KEYUP)
            {
                player.setFlip(SDL_FLIP_NONE);
                player.setState(PlayerState::Idle);
            }   
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                Game::setGameState(GameState::PauseScreen);
                replayFile.close();
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
                //spawns door for next lvl 
                if (arenaList.empty())
                {
                    if (player.isNearby(player.getAsset(), ladder.getAsset(), INTERACTION_RANGE_LADDER))
                    {
                        player.setNearLadder(true);
                        if (event.type == SDL_KEYDOWN)
                        {
                            if (event.key.keysym.sym == SDLK_f)
                            {
                                level.nextLevel(player, enemyList, arenaList, ladder, isCloseTo, player.getHealth());
                                level.setLevel();
                            }
                        }
                    }
                }
            }
        }
        else if (!Data::isPlayerAlive)
        {
            if (event.key.keysym.sym == SDLK_r)
                restart();
        }
    }
    player.movePlayer();
}

/**
 * @brief Update objects
*/
void Game::update()
{
    saveReplayToList();

    // if you continue from save this makes it so it doesn't spawn spear --> not good solution
    static bool preventSpear = false;
    if (player.getAttack() > 0 && preventSpear == false)
    {
        delete spear;
        spear = nullptr;
    }
    
    if (preventSpear == false)
        preventSpear = true;

    player.setNearArena(false);
    if (!(player.isNearby(player.getAsset(), ladder.getAsset(), INTERACTION_RANGE_LADDER)) && arenaList.empty())
        player.setNearLadder(false);

    static int previousPoints = 0;
    if (points >= previousPoints + 1000)
    {
        spear = new Spear;
        spear->spawnSpear(grid);

        previousPoints += 1000;
    }

    if (spear->isPlayerTouching(player.getAsset()) && !spear->isSpearTouched())
    {
        player.increaseAttack();
        spear->setSpearTouched();

        delete spear;
        spear = nullptr;
    }
 
    if (player.getAttack() > 0)
    {
        for (auto it = enemyList.begin(); it != enemyList.end();)
        {
            if (player.isPlayerTouching(it->getAsset()))
            {
                it = enemyList.erase(it);
                player.decreaseAttack();
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
            if (currentEnemy.isPlayerTouching(player.getAsset()))
            {
                player.changeHealth(-1);

                //moves enemy a bit / depends on pos
                if (player.getAsset().x > currentEnemy.getAsset().x)
                    currentEnemy.setX(max((currentEnemy.getAsset().x - 50), 0));

                else if (player.getAsset().x < currentEnemy.getAsset().x)
                    currentEnemy.setX(max((currentEnemy.getAsset().x + 50), 0));
                
                if (player.getAsset().y > currentEnemy.getAsset().y)
                    currentEnemy.setY(max((currentEnemy.getAsset().y - 50), 0));
                
                else if (player.getAsset().y < currentEnemy.getAsset().y)
                    currentEnemy.setY(max((currentEnemy.getAsset().y + 50), 0));

                currentEnemy.setState(EnemyState::Attacked);
                currentEnemy.setAttackTimer();
            }
        }   
    }
    for (auto& currentEnemy : enemyList)
        currentEnemy.updateEnemyAI(player, ENEMY_RANGE, ANIMATION_SPEED);

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
    }

    for (auto& entry : arenaList)
    {
        Arena& currentArena = entry.second;

        if (player.isNearby(player.getAsset(), currentArena.getAsset(), INTERACTION_RANGE_ARENA))
            player.setNearArena(true);
    }

    if (player.getState() == PlayerState::Idle)
        player.updatePlayerAnimation(ANIMATION_SPEED);

}

/**
 * @brief Renders the changes
*/
void Game::render()
{
    window.clear();

    map.drawMap();

    if (arenaList.empty())
        Window::draw(ladderTexture, ladder.getAsset());

    for (auto& currentArena : arenaList)
        if (currentArena.second.getVisible() || currentArena.second.isForcedVisible())
            Window::draw(arenaTexture, currentArena.second.getAsset());

    if (spear != nullptr)
        Window::draw(spearTexture, spear->getAsset());

    if (player.getAttack() <= 0)
        window.drawAnimation(window.getRenderer(), player.getSrcRect(), player.getAsset(), playerNoSpearTexture, player.getFlip());
    else
        window.drawAnimation(window.getRenderer(), player.getSrcRect(), player.getAsset(), playerSpearTexture, player.getFlip());

    for (auto& currentEnemy : enemyList)
        window.drawAnimation(window.getRenderer(),currentEnemy.getSrcRect(), currentEnemy.getAsset(), enemyTexture, currentEnemy.getFlip());


    Window::createText(("Attack: " + to_string(player.getAttack())).c_str(), GameSettings::WIDTH, GameSettings::HEIGHT - 35);

    if (arenaList.size() < level.getArenaCounter())
        Window::createText(("Remaining arenas: " + to_string(arenaList.size())).c_str(), GameSettings::WIDTH, 50);

    if (player.isNearArena())
        Window::createText("Save bull [E]", (GameSettings::WIDTH / 2) + 180, GameSettings::HEIGHT - 50);

    if (player.isNearLadder())
        Window::createText(("Next level [F]"), (GameSettings::WIDTH / 2) + 200, GameSettings::HEIGHT - 50);

    Window::createText(("Level: " + to_string(level.getLevel())).c_str(), GameSettings::WIDTH / 2 + 100, 0);
    Window::createText(("Points: " + to_string(points)).c_str(), GameSettings::WIDTH, 0);

    Window::drawPlayerHealth(player.getHealth(), hearts_1Texture, hearts_2Texture, hearts_3Texture);
    // last renders overlay above renders

    // game over screen
    if (!(Data::isPlayerAlive))
    {
        replayFile.close();

        SDL_RenderClear(window.getRenderer());     
        SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 0);

        Window::createText("Game over", GameSettings::WIDTH / 2, GameSettings::HEIGHT / 2);
        Window::createText("Press [R] to restart", GameSettings::WIDTH / 2, GameSettings::HEIGHT / 2 + 200);

        window.present();

        updateHighscores();
        if (points < getLowestScore() || points == 0)
            saveReplay();

    }
    window.present();  
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

    player.setX(max((rand() % (GameSettings::WIDTH - PLAYER_WIDTH)), 0));
    player.setY(max((rand() % (GameSettings::HEIGHT - PLAYER_HEIGHT)), 0));
    player.setHealth(3);
    player.setAttack(0);

    level.setArenaCounter(2 + rand() % 2);
    level.resetLevel();

    arenaList.clear();
    Arena::generateArenaPositions(level.getArenaCounter(), grid, arenaList);

    level.setEnemyCounter(1);
    enemyList.clear();
    Enemy::generateEnemyPositions(grid, player, enemyList, 1);

    ladder.setX(max((rand() % GameSettings::WIDTH - LADDER_WIDTH), 0));
    ladder.setY(max((rand() % GameSettings::HEIGHT - LADDER_HEIGHT), 0));

    points = 0;

    player.setNearLadder(false);
}

/**
 * @brief Updates highscores.
 * To container then writes to file.
*/
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
            else if (key == "Attack:")
            {
                iss >> value1;
                player.setAttack(value1);
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

    saveFile << "Name: " << Data::playerName << "\n"; 
    saveFile << "Player: " << player.getAsset().x << "\t" << player.getAsset().y << "\n";

    for (auto entry : arenaList)
        saveFile << "Arena: " << entry.second.getAsset().x << "\t" << entry.second.getAsset().y << "\n";
    
    for (auto currentEnemy : enemyList)
        saveFile << "Enemy: " << currentEnemy.getAsset().x << "\t" << currentEnemy.getAsset().y << "\n";

    saveFile << "Level: " << level.getLevel() << "\n";
    saveFile << "Ladder: " << ladder.getAsset().x << "\t" << ladder.getAsset().y << "\n";
    saveFile << "Points: " << points << "\n";
    saveFile << "Attack: " << player.getAttack() << "\n";

    saveFile.close();
}

/**
 * @brief Plays replay.
 * Lowest score player can replay, not the most recent one (except if he's worst).
*/
void Game::replay()
{
    PlayerPosition playerPos;

    ifstream readReplayFile;
    readReplayFile.open("files/replayFile.bin", ios::binary);

    while (readReplayFile.read((char*) &playerPos, sizeof(PlayerPosition)))
    {
        player.setX(playerPos.x);
        player.setY(playerPos.y);

        render();
        SDL_Delay(15); // very bad --> crashes game
    }
    readReplayFile.close();

    SDL_RenderClear(Data::renderer);
    SDL_SetRenderDrawColor(Data::renderer, 0, 0, 0, 255);
    Window::createText("Replay over", GameSettings::WIDTH / 2 + 200, GameSettings::HEIGHT / 2);
    SDL_RenderPresent(Data::renderer);

    SDL_Delay(3000);

}

/**
 * @brief Saves replay to container
*/
void Game::saveReplayToList()
{
    PlayerPosition playerPos;
    
    playerPos.x = player.getAsset().x;
    playerPos.y = player.getAsset().y;

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
