#include <SDL2/SDL.h>

class Player
{
    private:
        int health;
        SDL_Rect asset;
        int position_x, position_y;

    
    public:
        Player(int health, SDL_Rect asset, int x, int y)
        {
            this->health = health;
            this->asset = asset;
            this->position_x = x;
            this->position_y = y;
        }  

        int getHealth()
        {
            return this->health;
        }

        SDL_Rect getAsset() {
            return this->asset;
        }

        void setX(int newX) 
        {
            this->position_x = newX;
        }

        int getX()
        {
            return this->asset.x;
        }

};

