#include "MainMenuState.hpp"

// Select the option where the cursor is
void MainMenuState::select(Game* game) {
    switch (getSelection()) {
        case START:
            game->popState();
            break;
        case CHANGE_SKIN: {
            game->pushState(SkinMenuState::instance());
        } break;
        case QUIT:
            game->quit();
    }
}