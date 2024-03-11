#ifndef START_SCREEN_HPP
#define START_SCREEN_HPP

#include <unordered_map>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include "options.hpp"
#include "gameSettings.hpp"
#include "highscores.hpp"
#include "entity.hpp"

using namespace std;

const int NAME_WIDTH = 200, NAME_HEIGHT = 100;
const int GAME_NAME_WIDTH = 1000, GAME_NAME_HEIGHT = 100;

class StartScreen : public Menu
{
    private:
        bool uiCreated;
        bool exitedOptions;
        int count;
        Options options;
        Highscores highscores;
        string playerName;
        SDL_Texture *imgTexture;

        SDL_Rect screenRect;
        SDL_Rect playButton; 
        SDL_Rect gameName;
        SDL_Rect inputNameRect;
        SDL_Rect playerNameRect;
        SDL_Rect optionsButton;
        SDL_Rect highscoresButton;
        SDL_Rect resetNameButton;
        SDL_Rect newGameButton;
        SDL_Rect continueButton;

    public:
        StartScreen(SDL_Renderer *renderer);
        ~StartScreen();
        void createUI(SDL_Renderer *renderer);
        void handleMouseClick(SDL_Renderer *renderer) override;
        void createPlayerName(SDL_Renderer *renderer);
        void renderUI(SDL_Renderer* renderer);
        void resetPlayerName();
        void run(SDL_Renderer *renderer);
        void run(SDL_Renderer *renderer, bool continueGame);
};

StartScreen::StartScreen(SDL_Renderer *renderer)
{
    this->uiCreated = false;
    this->exitedOptions = false;
    this->imgTexture = IMG_LoadTexture(renderer, "assets/startScreen.png");
    this->count = 0;

    this->playButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2), NAME_WIDTH, NAME_HEIGHT};
    this->gameName = {(GameSettings::WIDTH - GAME_NAME_WIDTH) / 2, 0, GAME_NAME_WIDTH, GAME_NAME_HEIGHT};
    this->inputNameRect = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT, NAME_WIDTH, NAME_HEIGHT};
    this->playerNameRect = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2), NAME_WIDTH, NAME_HEIGHT};
    this->optionsButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT) - NAME_HEIGHT - 200, NAME_WIDTH, NAME_HEIGHT};
    this->highscoresButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT) - NAME_HEIGHT - 260, NAME_WIDTH, NAME_HEIGHT};
    this->resetNameButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2 - 300, (GameSettings::HEIGHT / 2) - 300, NAME_WIDTH, NAME_HEIGHT};
    this->newGameButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2), NAME_WIDTH, NAME_HEIGHT};
    this->continueButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT, NAME_WIDTH, NAME_HEIGHT};
}

StartScreen::~StartScreen()
{
    TTF_CloseFont(this->font);
}

void StartScreen::createUI(SDL_Renderer *renderer)
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imgTexture, NULL, NULL);

    createText(renderer, "Resevanje bikca Ferdinda", gameName);

    SDL_RenderPresent(renderer);

    if (count == 0)
    {
        createPlayerName(renderer);
        count++;
    }
    

    createText(renderer, "Reset name", resetNameButton);
    createText(renderer, "Play", playButton);
    createText(renderer, "Scores", highscoresButton);

    playerNameRect.x = inputNameRect.x + inputNameRect.w - playerNameRect.w;
    playerNameRect.y = GameSettings::HEIGHT / 4;

    if (!playerName.empty())
        createText(renderer, playerName.c_str(), playerNameRect);

    createText(renderer, "Options", optionsButton);

    SDL_RenderPresent(renderer);
}

void StartScreen::handleMouseClick(SDL_Renderer *renderer)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (Data::inContinueScreen)
    {
        if (mouseX >= continueButton.x && mouseX <= continueButton.x + continueButton.w && mouseY >= continueButton.y && mouseY <= continueButton.y + continueButton.h)
            Data::continueGame = true;
        
        else if (mouseX >= newGameButton.x && mouseX <= newGameButton.x + newGameButton.w && mouseY >= newGameButton.y && mouseY <= newGameButton.y + newGameButton.h)
            Data::resetGame = true;
    }
    else if (!Data::inContinueScreen)
    {
        if (mouseX >= playButton.x && mouseX <= playButton.x + playButton.w && mouseY >= playButton.y && mouseY <= playButton.y + playButton.h)
        {
            Data::inStartScreen = false;
            this->uiCreated = false;
        }

        else if (mouseX >= highscoresButton.x && mouseX <= highscoresButton.x + highscoresButton.w && mouseY >= highscoresButton.y && mouseY <= highscoresButton.y + highscoresButton.h)
            highscores.open(renderer);

        else if (mouseX >= optionsButton.x && mouseX <= optionsButton.x + optionsButton.w && mouseY >= optionsButton.y && mouseY <= optionsButton.y + optionsButton.h)
            options.open(renderer);
            
        else if (mouseX >= resetNameButton.x && mouseX <= resetNameButton.x + resetNameButton.w && mouseY >= resetNameButton.y && mouseY <= resetNameButton.y + resetNameButton.h)
        {
            resetPlayerName();
            this->count = 0;
            createUI(renderer);
        }
    }
}

void StartScreen::createPlayerName(SDL_Renderer* renderer)
{   
    createText(renderer, "Enter name: ", inputNameRect);
    bool nameTyped = false;

    while (!nameTyped)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
                exit(0);
            else if (event.type == SDL_TEXTINPUT)
            {
                playerName += event.text.text;;
                renderUI(renderer);
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && !playerName.empty())
                {
                    playerName.pop_back();
                    renderUI(renderer);
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN || event.key.keysym.scancode == SDL_SCANCODE_KP_ENTER)  
                    nameTyped = true;
            }
        }
        SDL_RenderPresent(renderer);
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imgTexture, NULL, NULL);
    createText(renderer, "Resevanje bikca Ferdinanda", gameName);

    Data::playerName = playerName;

    ofstream saveFile;
    saveFile.open("saveFile.txt");
    saveFile << "Name: " <<  playerName << "\n";
}

void StartScreen::renderUI(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer);
    
    SDL_RenderCopy(renderer, imgTexture, NULL, NULL);
    createText(renderer, "Resevanje bikca Ferdinda", gameName);
    createText(renderer, "Enter name: ", inputNameRect);

    if (!playerName.empty())
        createText(renderer, playerName.c_str(), playerNameRect);
}

void StartScreen::resetPlayerName()
{
    this->playerName.clear();
}

void StartScreen::run(SDL_Renderer *renderer)
{
    if (!uiCreated)
    {
        createUI(renderer);
        uiCreated = true;
    }

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (SDL_QUIT == event.type)
            exit(0);
        else if (event.type == SDL_MOUSEBUTTONDOWN)
            handleMouseClick(renderer);
    }

    if (!options.isOpen())
    {
        createUI(renderer);

        options.setOpen(true);

        SDL_RenderPresent(renderer);
    }
    if (!highscores.isOpen())
    {  
        createUI(renderer);

        highscores.setOpen(true);

        SDL_RenderPresent(renderer);
    }
}

void StartScreen::run(SDL_Renderer *renderer, bool continueGame)
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imgTexture, NULL, NULL);

    createText(renderer, "Resevanje bikca Ferdinda", gameName);
    createText(renderer, "Continue", continueButton);
    createText(renderer, "New Game", newGameButton);

    ifstream file("saveFile.txt");
    if (file.is_open())
    {
        getline(file, Data::playerName);
        file.close();
    }

    createText(renderer, Data::playerName.c_str(), resetNameButton);

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (SDL_QUIT == event.type)
            exit(0);
        else if (event.type == SDL_MOUSEBUTTONDOWN)
            handleMouseClick(renderer);
    }

   SDL_RenderPresent(renderer);
}



#endif 