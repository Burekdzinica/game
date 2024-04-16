#ifndef PAUSE_SCREEN_HPP
#define PAUSE_SCREEN_HPP

const extern int NAME_WIDTH, NAME_HEIGHT;

class PauseScreen : public Menu
{
    private:
        SDL_Rect continueButton;
        SDL_Rect exitButton;
        SDL_Rect saveQuitButton;

        AudioManager audioManager;

    public:
        PauseScreen();
        void handleMouseClick(Game* game); 
        void run(Game* game);

};

/**
 * @brief Default constructor for PauseScreen
*/
PauseScreen::PauseScreen()
{
    this->continueButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT, NAME_WIDTH, NAME_HEIGHT};
    this->exitButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2), NAME_WIDTH, NAME_HEIGHT};
    this->saveQuitButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) + 75, NAME_WIDTH, NAME_HEIGHT};
}

/**
 * @brief Handler for mouse clicks
 * @param game The game object
*/
void PauseScreen::handleMouseClick(Game* game)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseX >= continueButton.x && mouseX <= continueButton.x + continueButton.w && mouseY >= continueButton.y && mouseY <= continueButton.y + continueButton.h)
    {
        audioManager.playMusic(1);
        Game::setGameState(GameState::Playing);
    }

    else if (mouseX >= exitButton.x && mouseX <= exitButton.x + exitButton.w && mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h)
    {
        audioManager.playMusic(3);
        Game::setGameState(GameState::StartScreen);
    }
    else if (mouseX >= saveQuitButton.x && mouseX <= saveQuitButton.x + saveQuitButton.w && mouseY >= saveQuitButton.y && mouseY <= saveQuitButton.y + saveQuitButton.h)
    {
        game->save();
        exit(0);
    }
}   

/**
 * @brief Runs the pause screen
 * @param game The game object
*/
void PauseScreen::run(Game* game)
{
    Window::clear();
    
    Window::createBlackScreen();

    createText(Data::renderer, "Continue", continueButton);
    createText(Data::renderer, "Main Menu", exitButton);
    createText(Data::renderer, "Save & Quit", saveQuitButton);

    Window::present();

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (SDL_QUIT == event.type)
            exit(0);
         if (event.type == SDL_MOUSEBUTTONDOWN)
            handleMouseClick(game);
    }
}





#endif