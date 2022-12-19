#include "DeathState.hpp"

#include "PlayingState.hpp"

void DeathState::init(Game* game) {
    std::string title = "YOU  DIED", subtitle = "PRESS  SPACE  TO  CONTINUE";
    mHeaderTexture.loadFromText(title, mFontLarge, {0xff, 0xff, 0xff}, 1);
    mSubtitleTexture.loadFromText(subtitle, mFontNormal, {0xff, 0xff, 0xff}, 1);
}

void DeathState::cleanup() {}

void DeathState::pause() {}

void DeathState::resume() {}

void DeathState::handleEvent(Game* game, SDL_Event e) {
    if (e.type == SDL_QUIT)
        game->quit();
    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
        game->popState();
        // recreate the playing state after a death
        game->changeState(PlayingState::instance());
    }
}

void DeathState::update(Game* game, Uint32 elapsed_time) {}

void DeathState::render(Game* game) {
    SDL_Rect header =
        mHeaderTexture.renderCentered(game->gGameRect, CENTER_HORIZONTAL | CENTER_VERTICAL);

    mSubtitleTexture.renderCentered(game->gGameRect, CENTER_HORIZONTAL, header.y + header.h);
}
