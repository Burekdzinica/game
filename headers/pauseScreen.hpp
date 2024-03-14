#ifndef PAUSE_SCREEN_HPP
#define PAUSE_SCREEN_HPP

#include <SDL2/SDL.h>

#include "entity.hpp"
#include "gameSettings.hpp"
#include "game.hpp"

const extern int NAME_WIDTH, NAME_HEIGHT;

class PauseScreen : public Menu
{
    private:
        SDL_Rect continueButton;
        SDL_Rect exitButton;
        SDL_Rect saveQuitButton;

    public:
        PauseScreen();
        void handleMouseClick(SDL_Renderer *renderer, Game* game); 
        void run(SDL_Renderer* renderer, Game* game);

};

PauseScreen::PauseScreen()
{
    this->continueButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT, NAME_WIDTH, NAME_HEIGHT};
    this->exitButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2), NAME_WIDTH, NAME_HEIGHT};
    this->saveQuitButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) + 100, NAME_WIDTH, NAME_HEIGHT};
}

void PauseScreen::handleMouseClick(SDL_Renderer* renderer, Game* game)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseX >= continueButton.x && mouseX <= continueButton.x + continueButton.w && mouseY >= continueButton.y && mouseY <= continueButton.y + continueButton.h)
        Data::inPauseScreen = false;

    else if (mouseX >= exitButton.x && mouseX <= exitButton.x + exitButton.w && mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h)
    {
        Data::inStartScreen = true;
        Data::inPauseScreen = false;  
        Data::inContinueScreen = true;
    }
    else if (mouseX >= saveQuitButton.x && mouseX <= saveQuitButton.x + saveQuitButton.w && mouseY >= saveQuitButton.y && mouseY <= saveQuitButton.y + saveQuitButton.h)
    {
        game->save();
        exit(0);
    }
}   

void PauseScreen::run(SDL_Renderer* renderer, Game* game)
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    createText(renderer, "Continue", continueButton);
    createText(renderer, "Main Menu", exitButton);
    createText(renderer, "Save & Quit", saveQuitButton);

    SDL_RenderPresent(renderer);

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (SDL_QUIT == event.type)
            exit(0);
        else if (event.type == SDL_MOUSEBUTTONDOWN)
            handleMouseClick(renderer, game);
    }
}





#endif