#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>

using namespace std;

class Window
{
    private:  
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        TTF_Font *font;
        SDL_Texture *imgTexture;
        SDL_Rect healthRect;

    public:
        Window(const string &title, int width, int height);
        ~Window(); //destructor
        void init();
        void clear();
        void present();
        SDL_Renderer* getRenderer();
        void draw(SDL_Renderer* renderer, SDL_Rect destRec, const char* imgLocation);
        void drawPlayerHealth(int playerHealth);
        void drawArena();
};

Window::Window(const string &title, int WIDTH, int HEIGHT)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL)
        cout << "Could not create window: " << SDL_GetError() << "\n";

    renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
        cout << "Could not create renderer: " << SDL_GetError() << "\n";

    imgTexture = IMG_LoadTexture(renderer, "assets/map.png");
    if (imgTexture == NULL)
        cout << "Cannot load image";

    healthRect = {1, 1, 279, 66};
    
}

//deletes the window/renderer
Window::~Window() 
{  
    // TTF_CloseFont(font); // seg fault
    SDL_DestroyTexture(imgTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

//initializes all libraries
void Window::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        cout << "SDL initialization failed: " << SDL_GetError() << "\n";

    if(TTF_Init() == 1)
        cout << "TTF initilization failed: " << TTF_GetError() << "\n";

    if(IMG_Init(IMG_INIT_PNG) == 0)
        cout << "IMG initialization failed: " << IMG_GetError() << "\n";
}

// //clears the screen // used for moving assets
// void Window::clear() 
// {
//     SDL_SetRenderDrawColor(renderer, 204, 144, 64, 255);
//     SDL_RenderClear(renderer);
// }

void Window::clear()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imgTexture, NULL, NULL);
}

void Window::present()
{
    SDL_RenderPresent(renderer);
}

SDL_Renderer* Window::getRenderer()
{
    return renderer;
}

void Window::draw(SDL_Renderer *renderer, SDL_Rect destRect, const char* imgLocation)
{
    SDL_Surface *imgSurface = IMG_Load(imgLocation);
    if (imgSurface == NULL)
        cout << "Cannot find image\n";
    
    SDL_Texture *imgTexture = SDL_CreateTextureFromSurface(renderer, imgSurface);
    if (imgTexture == NULL)
        cout << "Cannot create texture\n";

    SDL_FreeSurface(imgSurface);
    SDL_RenderCopy(renderer, imgTexture, NULL, &destRect);
    SDL_DestroyTexture(imgTexture);
}

void Window::drawPlayerHealth(int playerHealth)
{
    switch (playerHealth)
    {
        case 3:
            draw(renderer, healthRect, "assets/3_hearts_reloaded.png");
            break;
        case 2:
            draw(renderer, healthRect, "assets/2_hearts_reloaded.png");
            break;
        case 1:
            draw(renderer, healthRect, "assets/1_hearts_reloaded.png");
            break;
    }
}

void Window::drawArena()
{
    for (const auto& entry : arenaList)
    {
        int i = entry.first;
        const Arena& currentArena = entry.second;

        if (player.isNearby(player.getAsset(), currentArena.getAsset(), 1500)) //&& !(arenaTracker.getLvlDone()))
        {
            window.draw(window.getRenderer(), currentArena.getAsset(), "assets/arena.png");

            isCloseTo = i;

            isPlayerNearArena = true;
        }
    }
}

#endif 
