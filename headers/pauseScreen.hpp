#ifndef PAUSE_SCREEN_HPP
#define PAUSE_SCREEN_HPP

#include <SDL2/SDL.h>

#include "entity.hpp"
#include "gameSettings.hpp"

const extern int NAME_WIDTH, NAME_HEIGHT;

class PauseScreen : public Menu
{
    private:
        SDL_Rect continueButton;
        SDL_Rect exitButton;

    public:
        PauseScreen();
        void handleMouseClick(SDL_Renderer *renderer) override; 
        void run(SDL_Renderer* renderer);

};

PauseScreen::PauseScreen()
{
    this->continueButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT, NAME_WIDTH, NAME_HEIGHT};
    this->exitButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2), NAME_WIDTH, NAME_HEIGHT};
}

void PauseScreen::handleMouseClick(SDL_Renderer* renderer)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseX >= continueButton.x && mouseX <= continueButton.x + continueButton.w && mouseY >= continueButton.y && mouseY <= continueButton.y + continueButton.h)
    {
        Data::isReplayFileOpen = false;
        Data::inPauseScreen = false;
    }

    else if (mouseX >= exitButton.x && mouseX <= exitButton.x + exitButton.w && mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h)
    {
        Data::inStartScreen = true;
        Data::inPauseScreen = false;
        // Data::playerName.clear();   
        Data::inContinueScreen = true;
    }
}   

void PauseScreen::run(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    createText(renderer, "Continue", continueButton);
    createText(renderer, "Exit", exitButton);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (SDL_QUIT == event.type)
            exit(0);
        else if (event.type == SDL_MOUSEBUTTONDOWN)
            handleMouseClick(renderer);
    }
}




#endif