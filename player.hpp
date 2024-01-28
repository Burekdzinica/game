#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>

class Player
{
    private:
        int health;
        SDL_Rect asset;
        int position_x, position_y;

    
    public:
        Player(int health, SDL_Rect asset, int x, int y);
        int getHealth();
        SDL_Rect *getAsset();
        void setX(int newX);
        void setY(int newY);
        int getX();
        int getY();

};

#endif

