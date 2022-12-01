#pragma once

#include <SDL_ttf.h>

#include <string>

class Font {
   private:
    TTF_Font* mFont = nullptr;

   public:
    Font(const std::string& path, int ptsize) {
        mFont = TTF_OpenFont(path.c_str(), ptsize);
        if (mFont == nullptr) printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }

    ~Font() {
        TTF_CloseFont(mFont);
        mFont = nullptr;
    }

    inline TTF_Font* getFont() { return mFont; }
    inline int getHeight() { return TTF_FontHeight(mFont); }
};