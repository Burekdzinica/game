#ifndef WINDOW_HPP
#define WINDOW_HPP

using namespace std;

const extern int WIDTH, HEIGHT;

class Window
{
    private:  
        TTF_Font* font;
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        static SDL_Rect healthRect;

    public:
        Window();
        Window(const string& title, int WIDTH, int HEIGHT, const char* fontName, int fontSize);
        ~Window(); //destructor
        void init();
        static void createText(const char* textString, int x, int y);
        static void clear();
        static void present();
        SDL_Renderer* getRenderer();
        static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
        static void draw(SDL_Texture* tex, SDL_Rect dest); // if no srcRect
        void drawAnimation(SDL_Renderer* renderer, SDL_Rect srcRect, SDL_Rect destRect, SDL_Texture* imgTexture, SDL_RendererFlip flip);
        static void drawPlayerHealth(int playerHealth, SDL_Texture* hearts_1, SDL_Texture* hearts_2, SDL_Texture* hearts_3);
};

SDL_Rect Window::healthRect = {1, 1, 279, 66};

/**
 * @brief Default constructor for Window
*/
Window::Window()
{
    init();
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Reševanje bikca Ferdinanda", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GameSettings::WIDTH, GameSettings::HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL)
        cout << "Could not create window: " << SDL_GetError() << "\n";

    renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
        cout << "Could not create renderer: " << SDL_GetError() << "\n";

    this->font = TTF_OpenFont("fonts/pixel.ttf", 30);
    if (!font)
        cout << "Font initilization failed " << TTF_GetError() << "\n"; 

    Data::window = window;
    Data::renderer = renderer;
    Data::font = this->font;

    init();
}

/**
 * @brief Constructor for Window
*/
Window::Window(const string& title, int WIDTH, int HEIGHT, const char* fontName, int fontSize)
{
    init();
    SDL_Init(SDL_INIT_EVERYTHING);

    if (window == NULL)
        cout << "Could not create window: " << SDL_GetError() << "\n";

    renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        cout << "Could not create renderer: " << SDL_GetError() << "\n";

    this->font = TTF_OpenFont(fontName, fontSize);
    if (!font)
        cout << "Font initilization failed " << TTF_GetError() << "\n"; 

}

/**
 * @brief Destructor for Window
*/
Window::~Window() 
{  
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

/**
 * @brief initializes libraries
*/
void Window::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        cout << "SDL initialization failed: " << SDL_GetError() << "\n";

    if(TTF_Init() == -1)
        cout << "TTF initilization failed: " << TTF_GetError() << "\n";

    if(IMG_Init(IMG_INIT_PNG) == 0)
        cout << "IMG initialization failed: " << IMG_GetError() << "\n";
}

/** 
 * @brief Creates text on the screen
 * @param textString The text to draw
 * @param x The x position
 * @param y The y position 
*/
void Window::createText(const char* textString, int x, int y)
{
    SDL_Color color = {230, 230, 230};

    SDL_Surface* textSurface = TTF_RenderText_Solid(Data::font, textString, color);
    if (!textSurface)
        cout << "Text surface initilization failed: " << TTF_GetError() << "\n";
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Data::renderer, textSurface);
    SDL_FreeSurface(textSurface);

    int textWidth, textHeight;

    TTF_SizeText(Data::font, textString, &textWidth, &textHeight);

    SDL_Rect textRect = {abs(x - textWidth), y , textWidth, textHeight};

    SDL_RenderCopy(Data::renderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);

}

/**
 * @brief Clears the screen
*/
void Window::clear()
{
    SDL_RenderClear(Data::renderer);
}

/**
 * @brief Applies the renders to the screen
*/
void Window::present()
{
    SDL_RenderPresent(Data::renderer);
}

/**
 * @brief Returns window renderer
 * @return SDL_renderer
*/
SDL_Renderer* Window::getRenderer()
{
    return renderer;
}

/**
 * @brief Draws object with src rect
 * @param tex The texture to draw
 * @param src The source rectangle
 * @param dest The destination rectangle
*/
void Window::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(Data::renderer, tex, &src, &dest);
}

/**
 * @brief  Draws object without src rect
 * @param tex The texture to draw
 * @param dest The destination rectangle
*/
void Window::draw(SDL_Texture* tex, SDL_Rect dest)
{
    SDL_RenderCopy(Data::renderer, tex, NULL, &dest);
}

/**
 * @brief Draws animated objects
 * @param renderer The renderer
 * @param srcRect The source rectangle
 * @param destRect The destination rectangle
 * @param imgTexture The asset to draw
 * @param flip Flips the asset   
*/
void Window::drawAnimation(SDL_Renderer* renderer, SDL_Rect srcRect, SDL_Rect destRect, SDL_Texture* imgTexture, SDL_RendererFlip flip)
{
    if (imgTexture == NULL)
        cout << "Cannot create texture\n";

    SDL_RenderCopyEx(renderer, imgTexture, &srcRect, &destRect, 0, NULL, flip);
}

/**
 * @brief Draws player health
 * @param playerHealth The player health
 * @param hearts_1 The asset for 1 hp
 * @param hearts_2 The asset for 2 hp
 * @param hearts_3 The asset for 3 hp
*/
void Window::drawPlayerHealth(int playerHealth, SDL_Texture* hearts_1, SDL_Texture* hearts_2, SDL_Texture* hearts_3)
{
    switch (playerHealth)
    {
        case 3:
            Window::draw(hearts_1, healthRect);
            break;
        case 2:
            Window::draw(hearts_2, healthRect);
            break;
        case 1:
            Window::draw(hearts_3, healthRect);
            break;
    }
}

//     SDL_SetRenderDrawColor(renderer, 204, 144, 64, 255);

#endif 