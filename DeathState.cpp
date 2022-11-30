#include "DeathState.hpp"

void DeathState::init(Game* game) {
    mHeaderTexture.loadFromText("YOU  DIED", mFontLarge, {0xff, 0xff, 0xff});
    mSubtitleTexture.loadFromText("PRESS  SPACE  TO  CONTINUE", mFontNormal,
                                  {0xff, 0xff, 0xff});
}

void DeathState::cleanup() {}

void DeathState::pause() {}

void DeathState::resume() {}

void DeathState::handleEvent(Game* game, SDL_Event e) {
    if (e.type == SDL_QUIT)
        game->quit();
    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
        game->popState();
    }
}

void DeathState::update(Game* game, Uint32 elapsed_time) {}

void DeathState::render(Game* game) {
    SDL_Rect title = {(game->width - mHeaderTexture.getWidth()) / 2,
                      (game->game_height - mHeaderTexture.getHeight()) / 2,
                      mHeaderTexture.getWidth(), mHeaderTexture.getHeight()};
    mHeaderTexture.render(title.x, title.y);
    mSubtitleTexture.render((game->width - mSubtitleTexture.getWidth()) / 2,
                            title.y + title.h);
    SDL_RenderPresent(gRenderer);
}