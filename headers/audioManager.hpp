#pragma once

#include <SDL2/SDL_mixer.h>

class AudioManager
{
    private:
        Mix_Chunk* gameOver;
        Mix_Chunk* playerHit;
        Mix_Chunk* enemyKill;
        Mix_Chunk* ladderSpawn;
        Mix_Chunk* walk;
        Mix_Chunk* arenaDestroy;
        Mix_Chunk* spearPickup;
        Mix_Chunk* spearSpawn;
        Mix_Chunk* newLevel;

        Mix_Music* mainMusic;
        Mix_Music* pauseMusic;
        Mix_Music* menuMusic;
        Mix_Music* gameOverMusic;

    public:
        AudioManager();
        ~AudioManager();

        void playSound(int id);
        void playMusic(int id);    
};

/**
 * @brief Default constructor for AudioManager
*/
AudioManager::AudioManager()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

    this->gameOver = Mix_LoadWAV("audio/gameOver.mp3");
    if (!this->gameOver)
        std::cout << "Game over sound error" << Mix_GetError() << "\n";

    this->playerHit = Mix_LoadWAV("audio/playerHit.wav");
    if (!this->playerHit)
        std::cout << "Player hit sound error: " << Mix_GetError() << "\n";
        
    this->enemyKill = Mix_LoadWAV("audio/enemyKill.wav");
    if (!this->enemyKill)
        std::cout << "Enemy kill sound error: " << Mix_GetError() << "\n";
        
    this->ladderSpawn = Mix_LoadWAV("audio/ladderSpawn.wav");
    if (!this->ladderSpawn)
        std::cout << "Level complete sound error: " << Mix_GetError() << "\n";
        
    this->walk = Mix_LoadWAV("audio/walk.mp3");
    if (!this->walk)
        std::cout << "Walk sound error: " << Mix_GetError() << "\n";
        
    this->arenaDestroy = Mix_LoadWAV("audio/arenaDestroy.wav");
    if (!this->arenaDestroy)
        std::cout << "Arena destroy sound error: " << Mix_GetError() << "\n";
        
    this->spearPickup = Mix_LoadWAV("audio/spearPickup.mp3");
    if (!this->spearPickup)
        std::cout << "Spear pickup sound error: " << Mix_GetError() << "\n";

    this->spearSpawn = Mix_LoadWAV("audio/spearSpawn.mp3");
    if (!this->spearSpawn)
        std::cout << "Spear spawn sound error: " << Mix_GetError() << "\n";   

    this->newLevel = Mix_LoadWAV("audio/newLevel.wav");
    if (!this->newLevel)
        std::cout << "New level sound error: " << Mix_GetError() << "\n";        
     


    this->mainMusic = Mix_LoadMUS("audio/mainMusic.mp3");
    if (!this->mainMusic)
        std::cout << "Music error: " << Mix_GetError() << "\n";
        
    this->pauseMusic = Mix_LoadMUS("audio/pauseMusic.mp3");
    if (!this->pauseMusic)
        std::cout << "Pause screen error: " << Mix_GetError() << "\n";
        
    this->menuMusic = Mix_LoadMUS("audio/menuMusic.mp3");
    if (!this->menuMusic)
        std::cout << "Menu music error: " << Mix_GetError() << "\n";

    this->gameOverMusic = Mix_LoadMUS("audio/gameOverMusic.mp3");
    if (!this->gameOverMusic)
        std::cout << "Game over music error: " << Mix_GetError() << "\n";

}

/**
 * @brief Destructor for AudioManager
*/
AudioManager::~AudioManager()
{
    Mix_FreeChunk(this->gameOver);
    Mix_FreeChunk(this->playerHit);
    Mix_FreeChunk(this->arenaDestroy);
    Mix_FreeChunk(this->enemyKill);
    Mix_FreeChunk(this->ladderSpawn);
    Mix_FreeChunk(this->walk);
    Mix_FreeChunk(this->spearPickup);
    Mix_FreeChunk(this->spearSpawn);
    Mix_FreeChunk(this->newLevel);

    Mix_FreeMusic(this->mainMusic);
    Mix_FreeMusic(this->pauseMusic);
    Mix_FreeMusic(this->menuMusic);
    Mix_FreeMusic(this->gameOverMusic);

    Mix_CloseAudio();
}

/**
 * @brief Plays sound effect
 * @param id The sound effect ID
*/
void AudioManager::playSound(int id)
{
    switch (id)
    {
        case 1:
            Mix_PlayChannel(-1, this->gameOver, 0);
            break;

        case 2:
            Mix_PlayChannel(-1, this->playerHit, 0);
            break;

        case 3:
            Mix_PlayChannel(-1, this->enemyKill, 0);
            break;

        case 4:
            Mix_PlayChannel(-1, this->ladderSpawn, 0);
            break;

        case 5:
            Mix_PlayChannel(-1, this->walk, 0);
            break;

        case 6:
            Mix_PlayChannel(-1, this->arenaDestroy, 0);
            break;

        case 7:
            Mix_PlayChannel(-1, this->spearPickup, 0);
            break;

        case 8: 
            Mix_PlayChannel(-1, this->spearSpawn, 0);
            break;

        case 9:
            Mix_PlayChannel(-1, this->newLevel, 0);
            break;
        
        default:
            std::cout << "Unknown sound: " << id << "\n";
            break;
    }  
}

/**
 * @brief Plays music
 * @param id The music ID
*/
void AudioManager::playMusic(int id)
{
    switch (id)
    {
        case 1:
            Mix_PlayMusic(this->mainMusic, -1);
            break;
        
        case 2:
            Mix_PlayMusic(this->pauseMusic, -1);
            break;

        case 3:
            Mix_PlayMusic(this->menuMusic, -1);
            break;
        
        case 4:
            Mix_PlayMusic(this->gameOverMusic, -1);
            break;
        
        default:
            std::cout << "Unkown id: " << id << "\n";
            break;
    }
}
