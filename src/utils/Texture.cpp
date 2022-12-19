#include "Texture.hpp"
#include "../Game.hpp"

Texture::Texture() {
    // Initialize
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

Texture::Texture(const std::string& path) : Texture() {
    loadFromFile(path);
}

Texture::~Texture() {
    // Deallocate
    free();
}

bool Texture::loadFromFile(const std::string& path) {
    // Get rid of preexisting texture
    free();

    // The final texture
    SDL_Texture* newTexture = nullptr;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == nullptr) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(),
                   SDL_GetError());
        } else {
            // Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    // Return success
    mTexture = newTexture;
    return mTexture != nullptr;
}

bool Texture::loadFromText(const std::string& textureText, Font& font, SDL_Color textColor,
                           int outline, SDL_Color outlineColor) {
    // Get rid of preexisting texture
    free();

    // Render text surface
    SDL_Surface* textSurface =
        TTF_RenderText_Blended(font.getFont(), textureText.c_str(), textColor);

    if (outline > 0) {
        TTF_SetFontOutline(font.getFont(), outline);
        SDL_Surface* outlineSurface =
            TTF_RenderText_Blended(font.getFont(), textureText.c_str(), outlineColor);
        TTF_SetFontOutline(font.getFont(), 0);

        SDL_Rect textRect = {outline, outline, textSurface->w, textSurface->h};
        SDL_BlitSurface(textSurface, nullptr, outlineSurface, &textRect);
        SDL_FreeSurface(textSurface);
        textSurface = outlineSurface;
    }

    if (textSurface == nullptr) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    // Create texture from surface pixels
    mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (mTexture == nullptr) {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Get image dimensions
    mWidth = textSurface->w;
    mHeight = textSurface->h;

    // Get rid of old surface
    SDL_FreeSurface(textSurface);

    mOutline = outline;

    // Return success
    return true;
}

void Texture::free() {
    // Free texture if it exists
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    // Modulate texture rgb
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending) {
    // Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

void Texture::setAlpha(Uint8 alpha) {
    // Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Texture::render(SDL_Rect renderQuad, SDL_Rect* clip, double angle, SDL_Point* center,
                     SDL_RendererFlip flip) {
    // Set clip rendering dimensions
    if (clip != nullptr) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    if (SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip))
        printf("Render error. %s\n", SDL_GetError());
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center,
                     SDL_RendererFlip flip) {
    // Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    render(renderQuad, clip, angle, center, flip);
}

SDL_Rect Texture::renderCentered(SDL_Rect container, int centerAxis, int otherDimension,
                                 SDL_Rect* clip, double angle, SDL_Point* center,
                                 SDL_RendererFlip flip) {
    int x = centerAxis & CENTER_HORIZONTAL ? (container.w - mWidth) / 2 : otherDimension;
    int y = centerAxis & CENTER_VERTICAL ? (container.h - mHeight) / 2 : otherDimension;
    render(x, y, clip, angle, center, flip);
    return {x, y, mWidth, mHeight};
}

int Texture::getWidth() const { return mWidth; }

int Texture::getHeight() const { return mHeight; }