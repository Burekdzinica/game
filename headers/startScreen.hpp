#ifndef START_SCREEN_HPP
#define START_SCREEN_HPP

#include <unordered_map>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

using namespace std;

const extern int WIDTH, HEIGHT;

const int nameWidth = 200, nameHeight = 100;
const int gameNameWidth = 1000, gameNameHeight = 100;

struct TextSize
{
    int width, height;
};

class StartScreen
{
    private:
        bool quit;
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


    public:
        StartScreen(SDL_Renderer *renderer);
        ~StartScreen();
        void createText(SDL_Renderer *renderer, const char* textString, SDL_Rect &destRect);
        void createUI(SDL_Renderer *renderer);
        void handleMouseClick(SDL_Renderer *renderer);
        void createPlayerName(SDL_Renderer *renderer);
        void renderUI(SDL_Renderer* renderer);
        void openOptions(SDL_Renderer* renderer);
        void run(SDL_Renderer *renderer);
};

StartScreen::StartScreen(SDL_Renderer *renderer)
{
    this->quit = false;
    this->imgTexture = IMG_LoadTexture(renderer, "assets/startScreen.png");
    this->font = TTF_OpenFont("fonts/test.ttf", 50);
    this->textColor = {255, 255, 255};

    this->screenRect = {0, 0, WIDTH, HEIGHT};
    this->playButton = {WIDTH / 2 - nameWidth / 2, (HEIGHT / 2), nameWidth, nameHeight};
    this->gameName = {(WIDTH - gameNameWidth) / 2, 0, gameNameWidth, gameNameHeight};
    this->inputNameRect = {WIDTH / 2 - nameWidth / 2, (HEIGHT / 2) - nameHeight, nameWidth, nameHeight};
    this->playerNameRect = {WIDTH / 2 - nameWidth / 2, (HEIGHT / 2), nameWidth, nameHeight};
    this->optionsButton = {WIDTH / 2 - nameWidth / 2, (HEIGHT) - nameHeight, nameWidth, nameHeight};

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

    destRect.x = (WIDTH - destRect.w) / 2;

    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void StartScreen::createUI(SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, imgTexture, NULL, &this->screenRect);

    createText(renderer, "Resevanje bikca Ferdinda", gameName);

    SDL_RenderPresent(renderer);
    createPlayerName(renderer);

    createText(renderer, "Play", playButton);

    playerNameRect.x = inputNameRect.x + inputNameRect.w - playerNameRect.w;
    playerNameRect.y = HEIGHT / 4;
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

    else if (mouseX >= optionsButton.x && mouseX <= optionsButton.x + optionsButton.w && mouseY >= optionsButton.y && mouseY <= optionsButton.y + optionsButton.h)
        openOptions(renderer);
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
                playerName += event.text.text;
                // createText(renderer, playerName.c_str(), playerNameRect);
                renderUI(renderer);
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && !playerName.empty())
                {
                    playerName.pop_back();
                    renderUI(renderer);
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)  
                    nameTyped = true;
            }
        }
        SDL_RenderPresent(renderer);
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imgTexture, NULL, &screenRect);
    createText(renderer, "Resevanje bikca Ferdinanda", gameName);

}

void StartScreen::renderUI(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer);
    
    SDL_RenderCopy(renderer, imgTexture, NULL, &this->screenRect);
    createText(renderer, "Resevanje bikca Ferdinda", gameName);
    createText(renderer, "Enter name: ", inputNameRect);
    createText(renderer, playerName.c_str(), playerNameRect);

    SDL_RenderPresent(renderer);
}

void StartScreen::openOptions(SDL_Renderer* renderer)
{
    bool inOptions = true;
    while (inOptions)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
                exit(0);
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
                    inOptions = false;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        
    }
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




        SDL_RenderPresent(renderer);
    }
}



#endif