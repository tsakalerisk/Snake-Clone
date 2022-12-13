#pragma once

#include "MenuState.hpp"
#include "PlayingState.hpp"
#include "Snake.hpp"

class SkinMenuState : public MenuState {
   public:
    static SkinMenuState* instance() {
        static SkinMenuState an_istance;
        return &an_istance;
    }

   private:
    SkinMenuState() : MenuState({"000", "BACK"}){};
    enum OptionsEnum { SKIN_NUMBER, BACK };

    void init(Game* game) override;
    void handleEvent(Game* game, SDL_Event e) override;
    void render(Game* game) override;
    void select(Game* game) override;

    bool skinExists(int n);
    bool previewSkin(int n);
    std::string getSkinPath(int n);

    SDL_Rect preview_rect;
    int skin_previewed = 0;
};