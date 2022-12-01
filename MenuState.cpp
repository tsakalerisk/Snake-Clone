#include "MenuState.hpp"

void MenuState::init(Game* game) {
    mMenuRect.x = game->gGameRect.x;
    mMenuRect.h = options.size() * mFont.getHeight() + 1;
    mMenuRect.y = (game->gGameRect.h - mMenuRect.h) / 2;
    mMenuRect.w = game->gGameRect.w;
    setSelection(0);
}

void MenuState::cleanup() {}

void MenuState::pause() {}

void MenuState::resume() {}

void MenuState::handleEvent(Game* game, SDL_Event e) {
    if (e.type == SDL_QUIT)
        game->quit();
    else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                if (iter > options.begin()) iter--;
                break;
            case SDLK_DOWN:
                if (iter < options.end() - 1) iter++;
                break;
            case SDLK_SPACE:
            case SDLK_RETURN:
                select(game);
                break;
        }
    }
}

void MenuState::update(Game* game, Uint32 elapsed_time) {}

void MenuState::render(Game* game) {
    SDL_SetRenderDrawColor(gRenderer, 0x10, 0x10, 0x10, 0xff);
    SDL_RenderFillRect(gRenderer, &mMenuRect);

    for (int i = 0; i < options.size(); i++) {
        mTextTexture.loadFromText(options[i], mFont, {0xff, 0xff, 0xff});
        SDL_Rect text_rect;
        text_rect.x = (mMenuRect.x + mMenuRect.w - mTextTexture.getWidth()) / 2;
        text_rect.y = mMenuRect.y + i * mTextTexture.getHeight();
        text_rect.w = mTextTexture.getWidth();
        text_rect.h = mTextTexture.getHeight();
        mTextTexture.render(text_rect);
        if (i == getSelection()) {
            SDL_SetRenderDrawColor(gRenderer, 0x88, 0, 0, 0xff);
            SDL_RenderDrawLine(gRenderer, mMenuRect.x, text_rect.y, mMenuRect.x + mMenuRect.w,
                               text_rect.y);
            SDL_RenderDrawLine(gRenderer, mMenuRect.x, text_rect.y + text_rect.h,
                               mMenuRect.x + mMenuRect.w, text_rect.y + text_rect.h);
        }
    }
}