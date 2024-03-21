#ifndef MAP_HPP
#define MAP_HPP

#include "gameSettings.hpp"

class Map
{
    private:
        int map[100][100];
        SDL_Rect src, dest;
        SDL_Texture* dirt;
    
    public:
        Map();
        ~Map();
        void drawMap();
};

#define LOAD_TEXTURE(renderer, imgPath) SDL_CreateTextureFromSurface(renderer, IMG_Load(imgPath))

/**
 * @brief Default constructor for Map
*/
Map::Map()
{
    this->dirt = LOAD_TEXTURE(Data::renderer, "assets/dirt_no_blur.png");

    this->src.x = src.y = 0;
    this->src.w = dest.w = 64;
    this->src.h = dest.h = 64;
    this->dest.x = dest.y = 0;
}

/**
 * @brief Destructor for Map
*/
Map::~Map()
{
    SDL_DestroyTexture(this->dirt);
}

/**
 * @brief Draws the tilemap
*/
void Map::drawMap()
{
    for (int row = 0; row < 50; row++)
    {
        for (int column = 0; column < 50; column++)
        {
            this->dest.x = column * 64;
            this->dest.y = row * 64;
            
            Window::draw(dirt, src, dest);
        }
    }
}

#endif
