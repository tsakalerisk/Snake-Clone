#include "PauseState.hpp"

// Select the option where the cursor is
void PauseState::select(Game* game) {
    switch (getSelection()) {
        case CONTINUE:
            game->popState();
            break;
        case CHANGE_SKIN: {
            // TODO:
            //  SDL_Surface* temp_surface = SDL_CreateRGBSurface(
            //      0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
            //  SDL_RenderReadPixels(gRenderer, NULL, 0, temp_surface->pixels,
            //                       temp_surface->pitch);
            //  SDL_Texture* temp_texture =
            //      SDL_CreateTextureFromSurface(gRenderer, temp_surface);
            //  SkinMenu();
            //  SDL_RenderCopy(gRenderer, temp_texture, NULL, NULL);
            //  SDL_RenderPresent(gRenderer);
            //  SDL_DestroyTexture(temp_texture);
            //  SDL_FreeSurface(temp_surface);
        } break;
        case QUIT:
            game->quit();
    }
}