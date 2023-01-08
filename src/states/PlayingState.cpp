#include "PlayingState.hpp"

#include <SDL.h>

#include <ctime>

#include "../Game.hpp"
#include "DeathState.hpp"
#include "menu-states/PauseState.hpp"

PlayingState::PlayingState() : mSnake(gResourcesPath + "worm.png") {
    mTextureBackground.loadFromFile(gResourcesPath + "ground.png");
    mTextureApple.loadFromFile(gResourcesPath + "apple.png");
}

void PlayingState::init(Game* game) {
    mScore.reset();
    mSnake.init(game->width, game->game_height);
    do {
        mFruit.x = rand() % (game->width / 20) * 20;
        mFruit.y = rand() % (game->game_height / 20) * 20;
    } while (mSnake.contains(mFruit));
    SDL_SetRenderDrawColor(gRenderer, 0x10, 0x10, 0x10, 0xff);
    SDL_RenderFillRect(gRenderer, &game->gInfoRect);
    std::string text = "PRESS  ESC  TO  PAUSE";
    mTextTexture.loadFromText(text, mFontNormal, {0xff, 0xff, 0xff});
    mTextTexture.render(game->gInfoRect.x, game->gInfoRect.y);
}

void PlayingState::handleEvent(Game* game, SDL_Event e) {
    if (e.type == SDL_QUIT) {
        game->quit();
    } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_DOWN:
            case SDLK_LEFT:
            case SDLK_RIGHT:
                if (!mMovedThisTurn) {
                    mSnake.turn(e.key.keysym.sym);
                    mMovedThisTurn = true;
                }
                break;
            case SDLK_SPACE:
                mSnake.grow();
                break;
            case SDLK_ESCAPE:
                game->pushState(PauseState::instance());
                break;
        }
    }
}

void PlayingState::update(Game* game, Uint32 elapsed_time) {
    mMovedThisTurn = false;
    if (mSnake.getHeading() != DEFAULT) {
        if (!mSnake.advance(game->width, game->game_height)) {
            mScore.writeHighScore();
            game->pushState(DeathState::instance());
            return;
        } else {
            if (mSnake.getHead() == mFruit) {
                mSnake.grow();
                mScore.add(5);
                do {
                    mFruit.x = rand() % (game->width / 20) * 20;
                    mFruit.y = rand() % (game->game_height / 20) * 20;
                } while (mSnake.contains(mFruit));
            }
        }
    }
}

void PlayingState::render(Game* game) {
    SDL_RenderSetViewport(gRenderer, nullptr);
    mTextureBackground.render(game->gGameRect);

    mSnake.render();

    mTextureApple.render({mFruit.x, mFruit.y, 20, 20});

    mScore.render(game->gInfoRect);
    SDL_RenderSetViewport(gRenderer, nullptr);
}