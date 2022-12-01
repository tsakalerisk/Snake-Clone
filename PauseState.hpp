#pragma once

#include <vector>

#include "MenuState.hpp"
#include "Texture.hpp"

using namespace std;

class PauseState : public MenuState {
   public:
    static PauseState* instance() {
        static PauseState an_istance;
        return &an_istance;
    }

   private:
    PauseState() : MenuState({"CONTINUE", "CHANGE  SKIN", "QUIT"}) {}
    enum OptionsEnum { CONTINUE, CHANGE_SKIN, QUIT };

    void select(Game* game) override;
};