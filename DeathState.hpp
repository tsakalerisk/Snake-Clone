#pragma once

#include "GameState.hpp"
#include "Texture.hpp"

extern std::string gResourcesPath;

class DeathState : public GameState {
   public:
    static DeathState* instance() {
        static DeathState an_istance;
        return &an_istance;
    }

    DeathState();

    void init(Game* game) override;
    void cleanup() override;

    void pause() override;
    void resume() override;

    void handleEvent(Game* game, SDL_Event e) override;
    void update(Game* game, Uint32 elapsed_time) override;
    void render(Game* game) override;

   private:
    const int outline = 1;
    const std::string mFontName = "ARCADECLASSIC.TTF";

    Texture mHeaderTexture, mHeaderTextureOutline, mSubtitleTexture, mSubtitleTextureOutline;
    Font mFontLarge = Font(gResourcesPath + mFontName, 48);
    Font mFontLargeOutline = Font(gResourcesPath + mFontName, 48);
    Font mFontNormal = Font(gResourcesPath + mFontName, 20);
    Font mFontNormalOutline = Font(gResourcesPath + mFontName, 20);
};