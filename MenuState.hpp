#pragma once

#include <vector>

#include "GameState.hpp"
#include "Texture.hpp"
#include "Game.hpp"

extern std::string gResourcesPath;
using namespace std;

class MenuState : public GameState {
   public:
    void init(Game* game) override;
    void cleanup() override;

    void pause() override;
    void resume() override;

    void handleEvent(Game* game, SDL_Event e) override;
    void update(Game* game, Uint32 elapsed_time) override;
    void render(Game* game) override;

   protected:
    explicit MenuState(const vector<string>& options) : options(options) {}

    vector<string> options;
    vector<string>::const_iterator iter;
    inline int getSelection() { return iter - options.begin(); };
    inline void setSelection(int x) { iter = options.begin() + x; };

    virtual void select(Game* game) = 0;

    Texture mTextTexture;
    SDL_Rect mMenuRect{};
    Font mFont = Font(gResourcesPath + "ARCADECLASSIC.TTF", 20);
};