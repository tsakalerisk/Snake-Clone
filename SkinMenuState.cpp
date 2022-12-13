#include "SkinMenuState.hpp"

void SkinMenuState::init(Game* game) {
    MenuState::init(game);

    preview_rect.x = game->gGameRect.x;
    preview_rect.h = 3 * 20;
    preview_rect.y = (game->gGameRect.h - mMenuRect.h - preview_rect.h) / 2;
    preview_rect.w = game->gGameRect.w;

    mMenuRect.y = preview_rect.y + preview_rect.h;

    previewSkin(skin_previewed);
}

void SkinMenuState::handleEvent(Game* game, SDL_Event e) {
    MenuState::handleEvent(game, e);
    if (e.type != SDL_KEYDOWN || getSelection() != SKIN_NUMBER) return;
    switch (e.key.keysym.sym) {
        case SDLK_LEFT:
            previewSkin(skin_previewed - 1);
            break;
        case SDLK_RIGHT:
            previewSkin(skin_previewed + 1);
            break;
    }
}

void SkinMenuState::render(Game* game) {
    MenuState::render(game);

    SDL_SetRenderDrawColor(gRenderer, 0x10, 0x10, 0x10, 0xff);
    SDL_RenderFillRect(gRenderer, &preview_rect);

    Texture preview_texture(getSkinPath(skin_previewed));
    vector<BodyPart> parts = {TAIL_BLACK, BODY_YELLOW, BODY_BLACK, BODY_YELLOW, BODY_BLACK, HEAD};

    for (int i = 0; i < parts.size(); i++) {
        SDL_Rect src = {parts[i] * 20, 0, 20, 20};
        SDL_Rect dst = {(preview_rect.x + preview_rect.w - 6 * 20) / 2 + i * 20,
                        preview_rect.y + (preview_rect.h - 20) / 2, 20, 20};
        preview_texture.render(dst, &src, 90);
    }

    Texture arrow_texture(gResourcesPath + "arrow.png");
    if (skinExists(skin_previewed - 1)) {
        SDL_Rect dst = {(preview_rect.x + preview_rect.w - 30) / 2 - 100,
                        preview_rect.y + (preview_rect.h - 30) / 2, 30, 30};
        arrow_texture.render(dst);
    }
    if (skinExists(skin_previewed + 1)) {
        SDL_Rect dst = {(preview_rect.x + preview_rect.w - 30) / 2 + 100,
                        preview_rect.y + (preview_rect.h - 30) / 2, 30, 30};
        arrow_texture.render(dst, nullptr, 0, nullptr, SDL_FLIP_HORIZONTAL);
    }
}

void SkinMenuState::select(Game* game) {
    if (getSelection() == BACK) {
        PlayingState::instance()->changeSnakeSkin(getSkinPath(skin_previewed));
        game->popState();
    }
}

bool SkinMenuState::previewSkin(int n) {
    if (!skinExists(n)) return false;
    skin_previewed = n;
    if (skin_previewed == 0)
        options[SKIN_NUMBER] = "DEFAULT  SKIN";
    else
        options[SKIN_NUMBER] = "SKIN  " + std::to_string(skin_previewed);
}

bool SkinMenuState::skinExists(int n) {
    if (FILE* temp_file = fopen(getSkinPath(n).c_str(), "r")) {
        fclose(temp_file);
        return true;
    } else
        return false;
}

std::string SkinMenuState::getSkinPath(int n) {
    std::string path = gResourcesPath + "worm";
    if (n != 0) path += std::to_string(n);
    path += ".png";
    return path;
}