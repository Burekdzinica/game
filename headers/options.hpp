#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <unordered_map>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include "gameSettings.hpp"

const extern int nameWidth, nameHeight;

using namespace std;

struct TextSize
{
    int width, height;
};

class Options
{
    private:
        bool inOptions;
        bool inFullscreen;
        int counter;
        TTF_Font *font;
        SDL_Color textColor;
        unordered_map<string, TextSize> textCache;
        bool inResolutionOptions;
        bool resolutionOptionsOpen;
        bool backInOptions;
        SDL_Rect resolutionButton;
        SDL_Rect exitButton;
        SDL_Rect resolution_1920x1080_button;
        SDL_Rect resolution_1280x720_button;
        SDL_Rect resolution_800x600_button;
        SDL_Rect fullscreenButton;


    public:
        Options();
        bool isOpen() const;
        int getCounter();
        void setCounter(int newCounter);
        void createText(SDL_Renderer *renderer, const char* textString, SDL_Rect &destRect);
        void handleMouseClick(SDL_Renderer *renderer);
        void openResolutionOptions(SDL_Renderer *renderer);
        void open(SDL_Renderer *renderer);
};

Options::Options()
{
    this->font = TTF_OpenFont("fonts/test.ttf", 50);
    this->textColor = {255, 255, 255};
    this->inOptions = true;
    this->inFullscreen = false;
    this->counter = 0;
    this->inResolutionOptions = true;
    this->resolutionOptionsOpen = false;
    this->backInOptions = false;
    this->resolutionButton = {GameSettings::WIDTH / 2 - nameWidth / 2, (GameSettings::HEIGHT / 2) - nameHeight, nameWidth, nameHeight};
    this->exitButton = {GameSettings::WIDTH/ 2 - nameWidth / 2, (GameSettings::HEIGHT / 2) - nameHeight + 200, nameWidth, nameHeight};; 
    this->resolution_1920x1080_button = {GameSettings::WIDTH / 2 - nameWidth / 2, (GameSettings::HEIGHT / 2) - nameHeight, nameWidth, nameHeight};
    this->resolution_1280x720_button = {GameSettings::WIDTH / 2 - nameWidth / 2, (GameSettings::HEIGHT / 2) - nameHeight - 100, nameWidth, nameHeight};
    this->resolution_800x600_button = {GameSettings::WIDTH/ 2 - nameWidth / 2, (GameSettings::HEIGHT / 2) - nameHeight - 200, nameWidth, nameHeight};
    this->fullscreenButton = {(GameSettings::WIDTH - nameWidth) / 2, 0, nameWidth, nameHeight};
}

bool Options::isOpen() const
{
    return this->inOptions;
}

int Options::getCounter()
{
    return this->counter;
}

void Options::setCounter(int newCounter)
{
    this->counter = newCounter;
}

void Options::createText(SDL_Renderer *renderer, const char* textString, SDL_Rect &destRect)
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
        
    // TTF_SizeText(this->font, textString, &textSize.width, &textSize.height);

    destRect.w = textSize.width;
    destRect.h = textSize.height;

    destRect.x = (GameSettings::WIDTH - destRect.w) / 2;

    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void Options::handleMouseClick(SDL_Renderer *renderer)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (!resolutionOptionsOpen)
    {
        if (mouseX >= resolutionButton.x && mouseX <= resolutionButton.x + resolutionButton.w && mouseY >= resolutionButton.y && mouseY <= resolutionButton.y + resolutionButton.h)
        {
            openResolutionOptions(renderer);
            inResolutionOptions = true;
        }
        
        else if (mouseX >= exitButton.x && mouseX <= exitButton.x + exitButton.w && mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h)
            inOptions = false;
   
    }
    
    if (resolutionOptionsOpen)
    {
        if (mouseX >= resolution_1920x1080_button.x && mouseX <= resolution_1920x1080_button.x + resolution_1920x1080_button.w && mouseY >= resolution_1920x1080_button.y && mouseY <= resolution_1920x1080_button.y + resolution_1920x1080_button.h)
        {
            GameSettings::WIDTH = 1920;
            GameSettings::HEIGHT = 1080;
            SDL_SetWindowSize(GameSettings::window, GameSettings::WIDTH, GameSettings::HEIGHT);
        }
        else if (mouseX >= resolution_1280x720_button.x && mouseX <= resolution_1280x720_button.x + resolution_1280x720_button.w && mouseY >= resolution_1280x720_button.y && mouseY <= resolution_1280x720_button.y + resolution_1280x720_button.h)
        {
            GameSettings::WIDTH = 1280;
            GameSettings::HEIGHT = 720;
            SDL_SetWindowSize(GameSettings::window, GameSettings::WIDTH, GameSettings::HEIGHT);
        }
        else if (mouseX >= resolution_800x600_button.x && mouseX <= resolution_800x600_button.x + resolution_800x600_button.w && mouseY >= resolution_800x600_button.y && mouseY <= resolution_800x600_button.y + resolution_800x600_button.h)
        {
            GameSettings::WIDTH = 800;
            GameSettings::HEIGHT = 600;
            SDL_SetWindowSize(GameSettings::window, GameSettings::WIDTH, GameSettings::HEIGHT);
        }
        else if (mouseX >= exitButton.x && mouseX <= exitButton.x + exitButton.w && mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h)
        {
            inResolutionOptions = false;
            resolutionOptionsOpen = false;
            backInOptions = true;
        }
        else if (mouseX >= fullscreenButton.x && mouseX <= fullscreenButton.x + fullscreenButton.w && mouseY >= fullscreenButton.y && mouseY <= fullscreenButton.y + fullscreenButton.h)
        {
            if (!this->inFullscreen)
            {
                SDL_SetWindowFullscreen(GameSettings::window, SDL_WINDOW_FULLSCREEN);
                this->inFullscreen = true;
            }
            else
            {
                SDL_SetWindowFullscreen(GameSettings::window, 0);
                this->inFullscreen = false;
            }
        }
    }
}

void Options::openResolutionOptions(SDL_Renderer *renderer)
{
    this->resolutionOptionsOpen = true;
    while (inResolutionOptions)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
                exit(0);
            else if (event.type == SDL_MOUSEBUTTONDOWN)
                handleMouseClick(renderer);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        createText(renderer, "Exit", exitButton);

        createText(renderer, "Fullscreen", fullscreenButton);
        createText(renderer, "1920x1080", resolution_1920x1080_button);
        createText(renderer, "1280x720", resolution_1280x720_button);
        createText(renderer, "800x600", resolution_800x600_button);
        SDL_RenderPresent(renderer);
    }
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void Options::open(SDL_Renderer *renderer)
{
    this->inOptions = true;
    SDL_RenderClear(renderer);

    createText(renderer, "Resolution", resolutionButton);
    createText(renderer, "Exit", exitButton);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
    while (inOptions)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
                exit(0);
            else if (event.type == SDL_MOUSEBUTTONDOWN)
                handleMouseClick(renderer);
        }
        if (backInOptions)
        {
            SDL_RenderClear(renderer);
            createText(renderer, "Exit", exitButton);
            createText(renderer, "Resolution", resolutionButton);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderPresent(renderer);

            backInOptions = false;
        }
    }
    this->counter = 1;

    return START_SCREEN_HPP;
}

#endif