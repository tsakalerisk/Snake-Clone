#include "PauseState.hpp"

// Select the option where the cursor is
void PauseState::select(Game* game) {
    switch (getSelection()) {
        case CONTINUE:
            game->popState();
            break;
        case CHANGE_SKIN: {
            game->pushState(SkinMenuState::instance());
        } break;
        case QUIT:
            game->quit();
    }
}