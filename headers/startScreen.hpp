#ifndef START_SCREEN_HPP
#define START_SCREEN_HPP

#include "options.hpp"
#include "highscores.hpp"

using namespace std;

const int NAME_WIDTH = 250, NAME_HEIGHT = 75;
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
        SDL_Texture* imgTexture;

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
        StartScreen();
        ~StartScreen();
        void createUI();
        void handleMouseClick();
        void createPlayerName();
        void renderUI();
        void resetPlayerName();
        void run(bool continueGame);
        void runFromSave();
        void runFromStart();
        void setUiCreated();
};

/**
 * @brief Default constructor for StartScreen
*/
StartScreen::StartScreen()
{
    this->uiCreated = false;
    this->exitedOptions = false;
    this->imgTexture = IMG_LoadTexture(Data::renderer, "assets/startScreen.png");
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

/**
 * @brief Destructor for StartScreen
*/
StartScreen::~StartScreen()
{
    TTF_CloseFont(this->font);
}

/**
 * @brief Creates UI
*/
void StartScreen::createUI()
{
    Window::clear();
    SDL_RenderCopy(Data::renderer, imgTexture, NULL, NULL);

    createText(Data::renderer, "Resevanje bikca Ferdinda", gameName);

    Window::present();

    if (count == 0)
    {
        createPlayerName();
        count++;
    }
    

    createText(Data::renderer, "Reset name", resetNameButton);
    createText(Data::renderer, "Play", playButton);
    createText(Data::renderer, "Scores", highscoresButton);

    playerNameRect.x = inputNameRect.x + inputNameRect.w - playerNameRect.w;
    playerNameRect.y = GameSettings::HEIGHT / 4;

    if (!playerName.empty())
        createText(Data::renderer, playerName.c_str(), playerNameRect);

    createText(Data::renderer, "Options", optionsButton);

    Window::present();
}

/**
 * @brief Handles mouse clicks
*/
void StartScreen::handleMouseClick()
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (Game::getGameState() == GameState::ContinueScreen)
    {
        if (mouseX >= continueButton.x && mouseX <= continueButton.x + continueButton.w && mouseY >= continueButton.y && mouseY <= continueButton.y + continueButton.h)
            Game::setGameState(GameState::ContinueGame);
        
        else if (mouseX >= newGameButton.x && mouseX <= newGameButton.x + newGameButton.w && mouseY >= newGameButton.y && mouseY <= newGameButton.y + newGameButton.h)
            Game::setGameState(GameState::ResetGame);
    }
    else if (Game::getGameState() == GameState::StartScreen)
    {
        if (mouseX >= playButton.x && mouseX <= playButton.x + playButton.w && mouseY >= playButton.y && mouseY <= playButton.y + playButton.h)
            Game::setGameState(GameState::Playing);
   
        else if (mouseX >= highscoresButton.x && mouseX <= highscoresButton.x + highscoresButton.w && mouseY >= highscoresButton.y && mouseY <= highscoresButton.y + highscoresButton.h)
            highscores.open();

        else if (mouseX >= optionsButton.x && mouseX <= optionsButton.x + optionsButton.w && mouseY >= optionsButton.y && mouseY <= optionsButton.y + optionsButton.h)
            options.open();
            
        else if (mouseX >= resetNameButton.x && mouseX <= resetNameButton.x + resetNameButton.w && mouseY >= resetNameButton.y && mouseY <= resetNameButton.y + resetNameButton.h)
        {
            resetPlayerName();
            this->count = 0;
            createUI();
        }
    }
}

/**
 * @brief Creates player name
*/
void StartScreen::createPlayerName()
{   
    createText(Data::renderer, "Enter name: ", inputNameRect);
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
                renderUI();
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && !playerName.empty())
                {
                    playerName.pop_back();
                    renderUI();
                }
                if ((event.key.keysym.scancode == SDL_SCANCODE_RETURN || event.key.keysym.scancode == SDL_SCANCODE_KP_ENTER) && playerName.empty()) 
                    cout << "Name can't be empty\n";

                else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN || event.key.keysym.scancode == SDL_SCANCODE_KP_ENTER)  
                    nameTyped = true;
            }
        }
        Window::present();
    }
    Window::clear();
    SDL_RenderCopy(Data::renderer, imgTexture, NULL, NULL);
    createText(Data::renderer, "Resevanje bikca Ferdinanda", gameName);

    Data::playerName = playerName;

    ofstream saveFile;
    saveFile.open("saveFile.txt");
    saveFile << "Name: " <<  playerName << "\n";
    saveFile.close();
}

/**
 * @brief Creates UI for typing name
*/
void StartScreen::renderUI()
{
    Window::clear();
    
    SDL_RenderCopy(Data::renderer, imgTexture, NULL, NULL);
    createText(Data::renderer, "Resevanje bikca Ferdinda", gameName);
    createText(Data::renderer, "Enter name: ", inputNameRect);

    if (!playerName.empty())
        createText(Data::renderer, playerName.c_str(), playerNameRect);
}

/**
 * @brief Resets player name
*/
void StartScreen::resetPlayerName()
{
    this->playerName.clear();
}

/**
 * @brief Runs startScreen
 * @param continueGame If true then continue from save
*/
void StartScreen::run(bool continueGame)
{
    if (continueGame)
        runFromSave();
    else
        runFromStart();
}

/**
 * @brief Runs from start
*/
void StartScreen::runFromStart()
{
   if (!uiCreated)
    {
        createUI();
        uiCreated = true;
    }

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (SDL_QUIT == event.type)
            exit(0);
        else if (event.type == SDL_MOUSEBUTTONDOWN)
            handleMouseClick();
    }

    if (!options.isOpen())
    {
        createUI();

        options.setOpen(true);

        Window::present();
    }
    if (!highscores.isOpen())
    {  
        createUI();

        highscores.setOpen(true);

        Window::present();
    } 
}

/**
 * @brief Runs from save
*/
void StartScreen::runFromSave()
{
    Window::clear();

    SDL_RenderCopy(Data::renderer, imgTexture, NULL, NULL);

    createText(Data::renderer, "Resevanje bikca Ferdinda", gameName);
    createText(Data::renderer, "Continue", continueButton);
    createText(Data::renderer, "New Game", newGameButton);

    ifstream file("saveFile.txt");
    if (file.is_open())
    {
        getline(file, Data::playerName);
        file.close();
    }

    createText(Data::renderer, Data::playerName.c_str(), resetNameButton);

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (SDL_QUIT == event.type)
            exit(0);
        else if (event.type == SDL_MOUSEBUTTONDOWN)
            handleMouseClick();
    }

   Window::present();
}

/**
 * @brief Sets UI is created
*/
void StartScreen::setUiCreated()
{
    this->uiCreated = false;
}

#endif 