#ifndef GAME_SETTINGS_HPP
#define GAME_SETTINGS_HPP

namespace GameSettings
{
    int WIDTH = 1720;
    int HEIGHT = 820;
}

namespace Data
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    bool isPlayerAlive = true;
    std::string playerName;
}

struct TextSize
{
    int width, height;
};

#endif
