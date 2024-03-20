#ifndef HIGHSCORES_HPP
#define HIGHSCORES_HPP

#include <fstream>

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
        void handleMouseClick() override;
        void drawHighscores();
        void open();
};

/**
 * @brief Default constructor for Highscores
*/
Highscores::Highscores()
{
    this->inHighscores = true;
    this->exitButton = {GameSettings::WIDTH/ 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT + 200, NAME_WIDTH, NAME_HEIGHT};
    this->highscoresName = {(GameSettings::WIDTH - NAME_WIDTH) / 2, 0, NAME_HEIGHT, NAME_HEIGHT};
    this->replayButton = {(GameSettings::WIDTH / 2) + 250, GameSettings::HEIGHT / 2, NAME_WIDTH, NAME_HEIGHT};
}

/**
 * @brief Destructor for Highscores
*/
Highscores::~Highscores()
{
    TTF_CloseFont(font);
}

/**
 * @brief Returns if open
 * @return True or False
*/
bool Highscores::isOpen() const
{
    return this->inHighscores;
}

/**
 * @brief Sets open
 * @param newOpen New open
*/
void Highscores::setOpen(bool newOpen)
{
    this->inHighscores = newOpen;
}

/**
 * @brief Handles mouse clicks
*/
void Highscores::handleMouseClick()
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseX >= exitButton.x && mouseX <= exitButton.x + exitButton.w && mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h)
        this->inHighscores = false;
    
    else if (mouseX >= replayButton.x && mouseX <= replayButton.x + replayButton.w && mouseY >= replayButton.y && mouseY <= replayButton.y + replayButton.h)
    {
        Game::setGameState(GameState::Replay);
        this->inHighscores = false;
    }

}

/**
 * @brief Draws highscores in menu
*/
void Highscores::drawHighscores()
{
    ifstream highscoresData("files/highscores.txt");

    int yPos = GameSettings::HEIGHT / 2 - 300;

    string line;
    while (getline(highscoresData, line))
    {
        SDL_Rect tempRect = {GameSettings::WIDTH / 2, yPos, 0, 0};

        createText(Data::renderer, line.c_str(), tempRect);

        yPos += 50;
    }

    highscoresData.close();
}

/**
 * @brief Opens highscore menu
*/
void Highscores::open()
{
    Window::clear();

    createText(Data::renderer, "Exit", exitButton);
    createText(Data::renderer, "Replay", replayButton);
    createText(Data::renderer, "Highscores", highscoresName);

    drawHighscores();
    
    Window::present();
    while (inHighscores)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
                exit(0);
            else if (event.type == SDL_MOUSEBUTTONDOWN)
                handleMouseClick();

        }
    }
}

#endif 