#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

#include "Font.hpp"

extern SDL_Renderer* gRenderer;

class Texture {
   public:
    // Initializes variables
    Texture();

    // Deallocates memory
    ~Texture();

    // Loads image at specified path
    bool loadFromFile(const std::string& path);

    // Creates image from font string
    bool loadFromText(const std::string& textureText, Font& font, SDL_Color textColor);

    // Deallocates texture
    void free();

    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set blending
    void setBlendMode(SDL_BlendMode blending);

    // Set alpha modulation
    void setAlpha(Uint8 alpha);

    void render(SDL_Rect renderQuad, SDL_Rect* clip = nullptr, double angle = 0.0,
                SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
    // Renders texture at given point
    void render(int x = 0, int y = 0, SDL_Rect* clip = nullptr, double angle = 0.0,
                SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

    // Gets image dimensions
    int getWidth() const;
    int getHeight() const;

   private:
    // The actual hardware texture
    SDL_Texture* mTexture;

    // Image dimensions
    int mWidth;
    int mHeight;
};