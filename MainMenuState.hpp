#pragma once

#include <vector>

#include "MenuState.hpp"
#include "Texture.hpp"

extern std::string gResourcesPath;
using namespace std;

class MainMenuState : public MenuState {
   public:
    static MainMenuState* instance() {
        static MainMenuState an_istance;
        return &an_istance;
    }

   private:
    MainMenuState() : MenuState({"START", "CHANGE  SKIN","QUIT"}) {}
    enum OptionsEnum { START, CHANGE_SKIN, QUIT };

    void select(Game* game) override;
};