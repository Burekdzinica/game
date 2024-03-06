#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <unordered_map>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include "gameSettings.hpp"

const extern int NAME_WIDTH, NAME_HEIGHT;

using namespace std;

class Options : public Menu
{
    private:
        bool inOptions;
        bool inFullscreen;
        int counter;
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
        void setOpen(bool newOpen);
        void handleMouseClick(SDL_Renderer *renderer) override;
        void openResolutionOptions(SDL_Renderer *renderer);
        void open(SDL_Renderer *renderer);
};

Options::Options()
{
    this->inOptions = true;
    this->inFullscreen = false;
    this->counter = 0;
    this->inResolutionOptions = true;
    this->resolutionOptionsOpen = false;
    this->backInOptions = false;

    this->resolutionButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT, NAME_WIDTH, NAME_HEIGHT};
    this->exitButton = {GameSettings::WIDTH/ 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT + 200, NAME_WIDTH, NAME_HEIGHT};
    this->resolution_1920x1080_button = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT, NAME_WIDTH, NAME_HEIGHT};
    this->resolution_1280x720_button = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT - 100, NAME_WIDTH, NAME_HEIGHT};
    this->resolution_800x600_button = {GameSettings::WIDTH/ 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT - 200, NAME_WIDTH, NAME_HEIGHT};
    this->fullscreenButton = {(GameSettings::WIDTH - NAME_WIDTH) / 2, 0, NAME_WIDTH, NAME_HEIGHT};
}

bool Options::isOpen() const
{
    return this->inOptions;
}

void Options::setOpen(bool newOpen)
{
    this->inOptions = newOpen;
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
            SDL_SetWindowSize(Data::window, GameSettings::WIDTH, GameSettings::HEIGHT);
        }
        else if (mouseX >= resolution_1280x720_button.x && mouseX <= resolution_1280x720_button.x + resolution_1280x720_button.w && mouseY >= resolution_1280x720_button.y && mouseY <= resolution_1280x720_button.y + resolution_1280x720_button.h)
        {
            GameSettings::WIDTH = 1280;
            GameSettings::HEIGHT = 720;
            SDL_SetWindowSize(Data::window, GameSettings::WIDTH, GameSettings::HEIGHT);
        }
        else if (mouseX >= resolution_800x600_button.x && mouseX <= resolution_800x600_button.x + resolution_800x600_button.w && mouseY >= resolution_800x600_button.y && mouseY <= resolution_800x600_button.y + resolution_800x600_button.h)
        {
            GameSettings::WIDTH = 800;
            GameSettings::HEIGHT = 600;
            SDL_SetWindowSize(Data::window, GameSettings::WIDTH, GameSettings::HEIGHT);
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
                SDL_SetWindowFullscreen(Data::window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                this->inFullscreen = true;
            }
            else
            {
                SDL_SetWindowFullscreen(Data::window, 0);
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