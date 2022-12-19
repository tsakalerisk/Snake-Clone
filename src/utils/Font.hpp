#pragma once

#include <SDL_ttf.h>

#include <string>

class Font {
   private:
    TTF_Font* mFont = nullptr;
    TTF_Font* mFontOutline = nullptr;
    int mOutline;

   public:
    Font(const std::string& path, int ptsize, int outline = 0) : mOutline(outline) {
        mFont = TTF_OpenFont(path.c_str(), ptsize);
        mFontOutline = TTF_OpenFont(path.c_str(), ptsize);
        if (mFont == nullptr || mFontOutline == nullptr)
            printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        TTF_SetFontOutline(mFontOutline, outline);
    }

    ~Font() {
        TTF_CloseFont(mFont);
        mFont = nullptr;
        mFontOutline = nullptr;
    }

    inline TTF_Font* getFont() { return mFont; }
    inline TTF_Font* getFontOutline() { return mFontOutline; }
    inline int getOutline() { return mOutline; }
    inline int getHeight() { return TTF_FontHeight(mFont); }
};