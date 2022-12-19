#include "PlayingState.hpp"

#include <SDL.h>

#include <ctime>

#include "../Game.hpp"
#include "DeathState.hpp"
#include "menu-states/PauseState.hpp"

PlayingState::PlayingState() : snake(gResourcesPath + "worm.png") {
    gTextureBackground.loadFromFile(gResourcesPath + "ground.png");
    gTextureApple.loadFromFile(gResourcesPath + "apple.png");
}

void PlayingState::init(Game* game) {
    score.reset();
    snake.init(game->width, game->game_height);
    do {
        fruit.x = rand() % (game->width / 20) * 20;
        fruit.y = rand() % (game->game_height / 20) * 20;
    } while (snake.contains(fruit));
    SDL_SetRenderDrawColor(gRenderer, 0x10, 0x10, 0x10, 0xff);
    SDL_RenderFillRect(gRenderer, &game->gInfoRect);
    std::string text = "PRESS  ESC  TO  PAUSE";
    gTextTexture.loadFromText(text, mFontNormal, {0xff, 0xff, 0xff});
    gTextTexture.render(game->gInfoRect.x, game->gInfoRect.y);
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
                    snake.turn(e.key.keysym.sym);
                    mMovedThisTurn = true;
                }
                break;
            case SDLK_SPACE:
                snake.grow();
                break;
            case SDLK_ESCAPE:
                game->pushState(PauseState::instance());
                break;
        }
    }
}

void PlayingState::update(Game* game, Uint32 elapsed_time) {
    mMovedThisTurn = false;
    if (snake.getHeading() != DEFAULT) {
        if (!snake.advance(game->width, game->game_height)) {
            score.writeHighScore();
            game->pushState(DeathState::instance());
            return;
        } else {
            if (snake.getHead() == fruit) {
                snake.grow();
                score.add(5);
                do {
                    fruit.x = rand() % (game->width / 20) * 20;
                    fruit.y = rand() % (game->game_height / 20) * 20;
                } while (snake.contains(fruit));
            }
        }
    }
}

void PlayingState::render(Game* game) {
    SDL_RenderSetViewport(gRenderer, nullptr);
    gTextureBackground.render(game->gGameRect);

    snake.render();

    gTextureApple.render({fruit.x, fruit.y, 20, 20});

    score.render(game->gInfoRect);
    SDL_RenderSetViewport(gRenderer, nullptr);
}