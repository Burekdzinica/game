#ifndef OPTIONS_HPP
#define OPTIONS_HPP

const extern int NAME_WIDTH, NAME_HEIGHT;

using namespace std;

class Options : public Menu 
{
    private:
        bool resolutionChange;
        bool inOptions;
        bool inFullscreen;
        bool inResolutionOptions;
        bool resolutionOptionsOpen;
        bool backInOptions;

        SDL_Rect resolutionButton;
        SDL_Rect exitButton;
        SDL_Rect resolution_1920x1080_button;
        SDL_Rect resolution_1280x720_button;
        SDL_Rect resolution_800x600_button;
        SDL_Rect fullscreenButton;

    public:
        Options();
        bool isOpen() const;
        void setOpen(bool newOpen);
        void handleMouseClick() override;
        void openResolutionOptions();
        void drawResolutions();
        void drawOptions();
        void open();
};

/**
 * @brief Default constructor for Options
*/
Options::Options()
{
    this->resolutionChange = true;
    this->inOptions = true;
    this->inFullscreen = false;
    this->inResolutionOptions = true;
    this->resolutionOptionsOpen = false;
    this->backInOptions = false;

    this->resolutionButton = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT, NAME_WIDTH, NAME_HEIGHT};
    this->exitButton = {GameSettings::WIDTH/ 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT + 200, NAME_WIDTH, NAME_HEIGHT};
    this->resolution_1920x1080_button = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT, NAME_WIDTH, NAME_HEIGHT};
    this->resolution_1280x720_button = {GameSettings::WIDTH / 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT - 100, NAME_WIDTH, NAME_HEIGHT};
    this->resolution_800x600_button = {GameSettings::WIDTH/ 2 - NAME_WIDTH / 2, (GameSettings::HEIGHT / 2) - NAME_HEIGHT - 200, NAME_WIDTH, NAME_HEIGHT};
    this->fullscreenButton = {(GameSettings::WIDTH - NAME_WIDTH) / 2, 0, NAME_WIDTH, NAME_HEIGHT};
}

/**
 * @brief Returns if options are open
 * @return True or False
*/
bool Options::isOpen() const
{
    return this->inOptions;
}

/**
 * @brief Sets new open
 * @param newOpen New open
*/
void Options::setOpen(bool newOpen)
{
    this->inOptions = newOpen;
}

/**
 * @brief Handles mouse clicks
*/
void Options::handleMouseClick()
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (!resolutionOptionsOpen)
    {
        if (mouseX >= resolutionButton.x && mouseX <= resolutionButton.x + resolutionButton.w && mouseY >= resolutionButton.y && mouseY <= resolutionButton.y + resolutionButton.h)
        {
            openResolutionOptions();
            inResolutionOptions = true;
        }
        
        else if (mouseX >= exitButton.x && mouseX <= exitButton.x + exitButton.w && mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h)
            inOptions = false;
   
    }
    
    if (resolutionOptionsOpen)
    {
        if (mouseX >= resolution_1920x1080_button.x && mouseX <= resolution_1920x1080_button.x + resolution_1920x1080_button.w && mouseY >= resolution_1920x1080_button.y && mouseY <= resolution_1920x1080_button.y + resolution_1920x1080_button.h)
        {
            this->resolutionChange = true;
            GameSettings::WIDTH = 1920;
            GameSettings::HEIGHT = 1080;
            SDL_SetWindowSize(Data::window, GameSettings::WIDTH, GameSettings::HEIGHT);
        }
        else if (mouseX >= resolution_1280x720_button.x && mouseX <= resolution_1280x720_button.x + resolution_1280x720_button.w && mouseY >= resolution_1280x720_button.y && mouseY <= resolution_1280x720_button.y + resolution_1280x720_button.h)
        {
            this->resolutionChange = true;
            GameSettings::WIDTH = 1280;
            GameSettings::HEIGHT = 720;
            SDL_SetWindowSize(Data::window, GameSettings::WIDTH, GameSettings::HEIGHT);
        }
        else if (mouseX >= resolution_800x600_button.x && mouseX <= resolution_800x600_button.x + resolution_800x600_button.w && mouseY >= resolution_800x600_button.y && mouseY <= resolution_800x600_button.y + resolution_800x600_button.h)
        {
            this->resolutionChange = true;
            GameSettings::WIDTH = 800;
            GameSettings::HEIGHT = 600;
            SDL_SetWindowSize(Data::window, GameSettings::WIDTH, GameSettings::HEIGHT);
        }
        else if (mouseX >= exitButton.x && mouseX <= exitButton.x + exitButton.w && mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h)
        {
            this->resolutionChange = true;
            inResolutionOptions = false;
            resolutionOptionsOpen = false;
            backInOptions = true;
        }
        else if (mouseX >= fullscreenButton.x && mouseX <= fullscreenButton.x + fullscreenButton.w && mouseY >= fullscreenButton.y && mouseY <= fullscreenButton.y + fullscreenButton.h)
        {
            this->resolutionChange = true;
            if (!this->inFullscreen)
            {
                SDL_SetWindowFullscreen(Data::window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                this->inFullscreen = true;
            }
            else
            {
                SDL_SetWindowFullscreen(Data::window, 0);
                this->inFullscreen = false;
            }
        }
    }
}

/**
 * @brief Opens resolution options
*/
void Options::openResolutionOptions()
{
    this->resolutionOptionsOpen = true;
    while (inResolutionOptions)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
                exit(0);
            else if (event.type == SDL_MOUSEBUTTONDOWN)
                handleMouseClick();
        }
        if (this->resolutionChange)
        {
            drawResolutions();

            this->resolutionChange = false;
        }
    }
    Window::clear();
    Window::present();
}

/**
 * @brief Draws resolutions options
*/
void Options::drawResolutions()
{
    Window::clear();

    SDL_SetRenderDrawColor(Data::renderer, 0, 0, 0, 255);

    createText(Data::renderer, "Exit", exitButton);
    createText(Data::renderer, "Fullscreen", fullscreenButton);
    createText(Data::renderer, "1920x1080", resolution_1920x1080_button);
    createText(Data::renderer, "1280x720", resolution_1280x720_button);
    createText(Data::renderer, "800x600", resolution_800x600_button);

    Window::present();
}

/**
 * @brief Draws option settings
*/
void Options::drawOptions()
{
    Window::clear();

    createText(Data::renderer, "Resolution", resolutionButton);
    createText(Data::renderer, "Exit", exitButton);
    
    SDL_SetRenderDrawColor(Data::renderer, 0, 0, 0, 255);
    
    Window::present();
}

/**
 * @brief Opens the options
*/
void Options::open()
{
    this->inOptions = true;

    drawOptions();

    while (inOptions)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
                exit(0);
            else if (event.type == SDL_MOUSEBUTTONDOWN)
                handleMouseClick();
        }
        if (backInOptions)
        {
            drawOptions();

            backInOptions = false;
        }
    }

    return START_SCREEN_HPP;
}

#endif