#ifndef HIGHSCORES_HPP
#define HIGHSCORES_HPP

#include <fstream>
#include <cstring>
#include <unordered_map>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include "gameSettings.hpp"

const extern int nameWidth, nameHeight;

using namespace std;

class Highscores
{
    private:
        TTF_Font *font;
        SDL_Color textColor;
        unordered_map<string, TextSize> textCache;
        bool inHighscores;
        SDL_Rect exitButton;
        SDL_Rect highscoresName;

    public:
        Highscores();
        ~Highscores();
        bool isOpen() const;
        void setOpen(bool newOpen);
        void createText(SDL_Renderer *renderer, const char* textString, SDL_Rect &destRect);
        void handleMouseClick(SDL_Renderer* renderer);
        void drawHighscores(SDL_Renderer* renderer);
        void open(SDL_Renderer* renderer);
};

Highscores::Highscores()
{
    this->font = TTF_OpenFont("fonts/pixel.ttf", 30);
    this->textColor = {255, 255, 255};
    this->inHighscores = true;
    this->exitButton = {GameSettings::WIDTH/ 2 - nameWidth / 2, (GameSettings::HEIGHT / 2) - nameHeight + 200, nameWidth, nameHeight};
    this->highscoresName = {(GameSettings::WIDTH - nameWidth) / 2, 0, nameHeight, nameHeight};
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

void Highscores::createText(SDL_Renderer *renderer, const char* textString, SDL_Rect &destRect)
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

void Highscores::handleMouseClick(SDL_Renderer *renderer)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseX >= exitButton.x && mouseX <= exitButton.x + exitButton.w && mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h)
        this->inHighscores = false;

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

#endif 