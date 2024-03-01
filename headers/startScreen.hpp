#ifndef START_SCREEN_HPP
#define START_SCREEN_HPP

#include <unordered_map>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include "options.hpp"
#include "gameSettings.hpp"
#include "highscores.hpp"

using namespace std;

const int nameWidth = 200, nameHeight = 100;
const int gameNameWidth = 1000, gameNameHeight = 100;

class StartScreen
{
    private:
        bool quit;
        bool exitedOptions;
        Options options;
        Highscores highscores;
        unordered_map<string, TextSize> textCache;
        string playerName;
        SDL_Texture *imgTexture;
        TTF_Font *font;
        SDL_Color textColor;
        SDL_Rect screenRect;
        SDL_Rect playButton; 
        SDL_Rect gameName;
        SDL_Rect inputNameRect;
        SDL_Rect playerNameRect;
        SDL_Rect optionsButton;
        SDL_Rect highscoresButton;

    public:
        StartScreen(SDL_Renderer *renderer);
        ~StartScreen();
        void createText(SDL_Renderer *renderer, const char* textString, SDL_Rect &destRect);
        void createUI(SDL_Renderer *renderer);
        void handleMouseClick(SDL_Renderer *renderer);
        void createPlayerName(SDL_Renderer *renderer);
        void renderUI(SDL_Renderer* renderer);
        void run(SDL_Renderer *renderer);
};

StartScreen::StartScreen(SDL_Renderer *renderer)
{
    this->quit = false;
    this->exitedOptions = false;
    this->imgTexture = IMG_LoadTexture(renderer, "assets/startScreen.png");
    this->font = TTF_OpenFont("fonts/pixel.ttf", 30);
    this->textColor = {255, 255, 255};

    this->playButton = {GameSettings::WIDTH / 2 - nameWidth / 2, (GameSettings::HEIGHT / 2), nameWidth, nameHeight};
    this->gameName = {(GameSettings::WIDTH - gameNameWidth) / 2, 0, gameNameWidth, gameNameHeight};
    this->inputNameRect = {GameSettings::WIDTH / 2 - nameWidth / 2, (GameSettings::HEIGHT / 2) - nameHeight, nameWidth, nameHeight};
    this->playerNameRect = {GameSettings::WIDTH / 2 - nameWidth / 2, (GameSettings::HEIGHT / 2), nameWidth, nameHeight};
    this->optionsButton = {GameSettings::WIDTH / 2 - nameWidth / 2, (GameSettings::HEIGHT) - nameHeight - 200, nameWidth, nameHeight};
    this->highscoresButton = {GameSettings::WIDTH / 2 - nameWidth / 2, (GameSettings::HEIGHT) - nameHeight - 260, nameWidth, nameHeight};
}

StartScreen::~StartScreen()
{
    TTF_CloseFont(font);
}

void StartScreen::createText(SDL_Renderer *renderer, const char* textString, SDL_Rect &destRect)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(this->font, textString, textColor);
    if (textSurface == NULL) 
        cout << "Initialization textSurface failed \n";

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL)
        cout << "Initialization textTexture failed \n";

    TextSize textSize;

    auto cacheIt = textCache.find(textString);
    if(cacheIt == textCache.end())
    {
        TTF_SizeText(this->font, textString, &textSize.width, &textSize.height);
        textCache[textString] = textSize;
    }
    else
        textSize = cacheIt->second;
        
    // TTF_SizeText(this->font, textString, &textWidth, &textHeight);

    destRect.w = textSize.width;
    destRect.h = textSize.height;

    destRect.x = (GameSettings::WIDTH - destRect.w) / 2;

    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void StartScreen::createUI(SDL_Renderer *renderer)
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imgTexture, NULL, NULL);

    createText(renderer, "Resevanje bikca Ferdinda", gameName);

    SDL_RenderPresent(renderer);
    createPlayerName(renderer);

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

    if (mouseX >= playButton.x && mouseX <= playButton.x + playButton.w && mouseY >= playButton.y && mouseY <= playButton.y + playButton.h)
        this->quit = true;

    else if (mouseX >= highscoresButton.x && mouseX <= highscoresButton.x + highscoresButton.w && mouseY >= highscoresButton.y && mouseY <= highscoresButton.y + highscoresButton.h)
        highscores.open(renderer);

    else if (mouseX >= optionsButton.x && mouseX <= optionsButton.x + optionsButton.w && mouseY >= optionsButton.y && mouseY <= optionsButton.y + optionsButton.h)
        options.open(renderer);
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

void StartScreen::run(SDL_Renderer *renderer)
{
    createUI(renderer);
    
    while(!this->quit)
    {
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
            playerName.clear();

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
    Data::playerName = playerName;
}



#endif 