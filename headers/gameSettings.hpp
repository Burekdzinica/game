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
    bool isPlayerAlive = true;
    std::string playerName;
    bool inStartScreen = true;
    bool inPauseScreen = false;
    bool inContinueScreen = true;
    bool resetGame = false;
    bool continueGame = false;
    bool isReplayFileOpen = false;
}

struct TextSize
{
    int width, height;
};

#endif
