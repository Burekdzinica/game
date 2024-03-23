#include <iostream>

#include "headers/game.hpp"
#include "headers/startScreen.hpp"
#include "headers/pauseScreen.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    const int FPS = 60;
    int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    Game game;
    game.setup();

    StartScreen startscreen;
    PauseScreen pausescreen;

    while (game.isOpen())
    {
       frameStart = SDL_GetTicks();

        switch (Game::getGameState())
        {
            case GameState::ContinueScreen:
                startscreen.run(true);

                break;
            
            case GameState::ContinueGame:
                game.continueGame();

                break;
            
            case GameState::StartScreen:
                startscreen.run(false);

                break;

            case GameState::PauseScreen:
                startscreen.setUiCreated();
                pausescreen.run(&game);
    
                break;

            case GameState::Replay:
                game.replay();
                
                startscreen.setUiCreated();
                Game::setGameState(GameState::StartScreen);

                break;

            case GameState::ResetGame:
                game.restart();
                Game::setGameState(GameState::Playing);

                break;

            case GameState::Playing:
                game.eventHandler();
                game.update();

                game.render();

                break;
        
        }
        
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }
   
    return EXIT_SUCCESS;
}
