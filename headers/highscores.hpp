#ifndef HIGHSCORES_HPP
#define HIGHSCORES_HPP

#include <fstream>
#include <cstring>
#include <unordered_map>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include "gameSettings.hpp"

const extern int NAME_WIDTH, NAME_HEIGHT;

using namespace std;

class Highscores : public Menu
{
    private:
        bool inHighscores;

        SDL_Rect exitButton;
        SDL_Rect highscoresName;
        SDL_Rect replayButton;

    public:
        Highscores();
        ~Highscores();
        bool isOpen() const;
        void setOpen(bool newOpen);
        void handleMouseClick(SDL_Renderer* renderer) override;
        void drawHighscores(SDL_Renderer* renderer);
        void open(SDL_Renderer* renderer);
        void replay(SDL_Renderer* renderer);
};

Highscores::Highscores()
{
    this->inHighscores = true;
    this->exitButton = {GameSettings::WIDTH/ 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT + 200, NAME_WIDTH, NAME_HEIGHT};
    this->highscoresName = {(GameSettings::WIDTH - NAME_WIDTH) / 2, 0, NAME_HEIGHT, NAME_HEIGHT};
    this->replayButton = {(GameSettings::WIDTH / 2) + 250, GameSettings::HEIGHT / 2, NAME_WIDTH, NAME_HEIGHT};
}

Highscores::~Highscores()
{
    TTF_CloseFont(font);
}

bool Highscores::isOpen() const
{
    return this->inHighscores;
}

void Highscores::setOpen(bool newOpen)
{
    this->inHighscores = newOpen;
}

void Highscores::handleMouseClick(SDL_Renderer *renderer)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseX >= exitButton.x && mouseX <= exitButton.x + exitButton.w && mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h)
        this->inHighscores = false;
    
    else if (mouseX >= replayButton.x && mouseX <= replayButton.x + replayButton.w && mouseY >= replayButton.y && mouseY <= replayButton.y + replayButton.h)
        replay(renderer);

}

void Highscores::drawHighscores(SDL_Renderer* renderer)
{
    ifstream highscoresData("highscores.txt");

    int yPos = GameSettings::HEIGHT / 2 - 300;

    string line;
    while (getline(highscoresData, line))
    {
        SDL_Rect tempRect = {GameSettings::WIDTH / 2, yPos, 0, 0};

        createText(renderer, line.c_str(), tempRect);

        yPos += 50;
    }

    highscoresData.close();
}

void Highscores::open(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer);

    createText(renderer, "Exit", exitButton);
    createText(renderer, "Replay", replayButton);
    createText(renderer, "Highscores", highscoresName);

    drawHighscores(renderer);
    SDL_RenderPresent(renderer);
    while (inHighscores)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
                exit(0);
            else if (event.type == SDL_MOUSEBUTTONDOWN)
                handleMouseClick(renderer);

        }
    }
}

void Highscores::replay(SDL_Renderer* renderer)
{

}

#endif 