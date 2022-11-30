#include "PlayingState.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

#include <ctime>

#include "DeathState.hpp"
#include "Game.hpp"
#include "PauseState.hpp"

void PlayingState::init(Game* game) {
    loadMedia();
    snake = new Snake(game->width, game->game_height);
    do {
        fruit.x = rand() % (game->width / 20) * 20;
        fruit.y = rand() % (game->game_height / 20) * 20;
    } while (snake->contains(fruit));
    srand((int)time(NULL));
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
                snake->turn(UP);
                break;
            case SDLK_DOWN:
                snake->turn(DOWN);
                break;
            case SDLK_LEFT:
                snake->turn(LEFT);
                break;
            case SDLK_RIGHT:
                snake->turn(RIGHT);
                break;
            case SDLK_SPACE:
                snake->grow();
                break;
            case SDLK_ESCAPE:
                game->pushState(PauseState::instance());
                break;
        }
    }
}

void PlayingState::update(Game* game, Uint32 elapsed_time) {
    if (snake->getHeading() != DEFAULT) {
        if (!snake->advance(game->width, game->game_height)) {
            game->pushState(DeathState::instance());
            snake->reset();
            score.reset();
            return;
        } else {
            if (snake->getHead() == fruit) {
                snake->grow();
                score.add(5);
                do {
                    fruit.x = rand() % (game->width / 20) * 20;
                    fruit.y = rand() % (game->game_height / 20) * 20;
                } while (snake->contains(fruit));
            }
        }
    }
}

void PlayingState::render(Game* game) {
    SDL_RenderSetViewport(gRenderer, NULL);
    gTextureBackground.render(game->gGameRect);

    snake->render();

    gTextureApple.render({fruit.x, fruit.y, 20, 20});

    score.render(game->gInfoRect);
    SDL_RenderSetViewport(gRenderer, NULL);

    SDL_RenderPresent(gRenderer);
}

bool PlayingState::loadMedia() {
    bool success = true;
    gTextureBackground.loadFromFile(gResourcesPath + "ground.png");
    gTextureApple.loadFromFile(gResourcesPath + "apple.png");
    return success;
}