#include "DeathState.hpp"

#include "PlayingState.hpp"

DeathState::DeathState() {
    TTF_SetFontOutline(mFontLargeOutline.getFont(), outline);
    TTF_SetFontOutline(mFontNormalOutline.getFont(), outline);
}

void DeathState::init(Game* game) {
    std::string title = "YOU  DIED", subtitle = "PRESS  SPACE  TO  CONTINUE";
    mHeaderTexture.loadFromText(title, mFontLarge, {0xff, 0xff, 0xff});
    mHeaderTextureOutline.loadFromText(title, mFontLargeOutline, {0x00, 0x00, 0x00});
    mSubtitleTexture.loadFromText(subtitle, mFontNormal, {0xff, 0xff, 0xff});
    mSubtitleTextureOutline.loadFromText(subtitle, mFontNormalOutline, {0x00, 0x00, 0x00});
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

// TODO: remove
SDL_Rect center(SDL_Rect container, Texture& texture);

// TODO: improve
void DeathState::render(Game* game) {
    SDL_Rect title_rect = center(game->gGameRect, mHeaderTextureOutline);
    mHeaderTextureOutline.render(title_rect);
    mHeaderTexture.render(title_rect.x + outline, title_rect.y + outline);

    mSubtitleTextureOutline.render((game->width - mSubtitleTextureOutline.getWidth()) / 2,
                                   title_rect.y + title_rect.h);
    mSubtitleTexture.render((game->width - mSubtitleTexture.getWidth()) / 2,
                            title_rect.y + title_rect.h + outline);
}

// TODO: remove
SDL_Rect center(SDL_Rect container, Texture& texture) {
    SDL_Rect rect;
    rect.x = (container.w - texture.getWidth()) / 2;
    rect.y = (container.h - texture.getHeight()) / 2;
    rect.w = texture.getWidth();
    rect.h = texture.getHeight();
    return rect;
}