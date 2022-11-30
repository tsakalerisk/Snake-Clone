#pragma once

#include <vector>

#include "GameState.hpp"
#include "Texture.hpp"

extern std::string gResourcesPath;
using namespace std;

class PauseState : public GameState {
   public:
    static PauseState* instance() {
        static PauseState an_istance;
        return &an_istance;
    }

    void init(Game* game) override;
    void cleanup() override;

    void pause() override;
    void resume() override;

    void handleEvent(Game* game, SDL_Event e) override;
    void update(Game* game, Uint32 elapsed_time) override;
    void render(Game* game) override;

   private:
    enum OptionsEnum { CONTINUE, CHANGE_SKIN, QUIT };
    vector<string> options = {"CONTINUE", "CHANGE  SKIN","QUIT"};
    vector<string>::iterator iter;
    int getSelection();
    void setSelection(int x);

    Texture mTextTexture;
    SDL_Rect mMenuRect;
    Font mFont = Font(gResourcesPath + "ARCADECLASSIC.TTF", 20);

    void select(Game* game);
};