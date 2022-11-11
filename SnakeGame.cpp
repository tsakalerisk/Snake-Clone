#include "SnakeGame.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>

SnakeGame::SnakeGame(std::string name, const int width, const int height)
    : Game(name, width, height), score(NULL) {
    loadMedia();
    score = Score(gFontNormal);
    init();
    quit = false;
    Uint32 start_time = SDL_GetTicks();
    while (true) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) handle_event(e, &quit);
        if (quit) break;
        if (currentState == PLAYING) {  // hack, will remove
            Uint32 curr_time = SDL_GetTicks();
            if (curr_time - start_time > 130) {
                loop(curr_time - start_time);
                start_time = curr_time;
            }
        }
    }
};

SnakeGame::~SnakeGame() {
    TTF_CloseFont(gFontNormal);
    gFontNormal = NULL;
}

void SnakeGame::init() {
    reset();
    srand((int)time(NULL));
    SDL_SetRenderDrawColor(gRenderer, 0x10, 0x10, 0x10, 0xff);
    SDL_RenderFillRect(gRenderer, &gInfoRect);
    std::string text = "PRESS  ESC  TO  PAUSE";
    gTextTexture.loadFromRenderedText(text, gFontNormal, {0xff, 0xff, 0xff});
    gTextTexture.render(gInfoRect.x, gInfoRect.y);
    render();
}

void SnakeGame::handle_event(SDL_Event e, bool* quit) {
    if (e.type == SDL_QUIT) {
        *quit = true;
    } else
        switch (currentState) {
            case PLAYING:
                handle_game_event(e);
                break;
            case DEAD:
                handle_dead_event(e);
                break;
        }
}

void SnakeGame::handle_game_event(SDL_Event e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                snake.turn(UP);
                break;
            case SDLK_DOWN:
                snake.turn(DOWN);
                break;
            case SDLK_LEFT:
                snake.turn(LEFT);
                break;
            case SDLK_RIGHT:
                snake.turn(RIGHT);
                break;
            case SDLK_SPACE:
                snake.grow();
                break;
            case SDLK_ESCAPE:
                // PauseMenu();
                break;
        }
    }
}

void SnakeGame::handle_dead_event(SDL_Event e) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
        currentState = PLAYING;
    }
}

void SnakeGame::loop(Uint32 elapsed_time) {
    if (snake.getHeading() != DEFAULT) {
        if (!snake.advance(width, game_height)) {
            onDeath();
            return;
        } else {
            if (snake.getHead() == fruit) {
                snake.grow();
                score.add(5);
                do {
                    fruit.x = rand() % (width / 20) * 20;
                    fruit.y = rand() % (game_height / 20) * 20;
                } while (snake.contains(fruit));
            }
        }
    }
    render();
}

void SnakeGame::onDeath() {
    gTextTexture.loadFromRenderedText("YOU  DIED", gFontLarge,
                                      {0xff, 0xff, 0xff});
    SDL_Rect title = {(width - gTextTexture.getWidth()) / 2,
                      (game_height - gTextTexture.getHeight()) / 2,
                      gTextTexture.getWidth(), gTextTexture.getHeight()};
    gTextTexture.render(title.x, title.y);

    gTextTexture.loadFromRenderedText("PRESS  SPACE  TO  CONTINUE", gFontNormal,
                                      {0xff, 0xff, 0xff});
    gTextTexture.render((width - gTextTexture.getWidth()) / 2,
                        title.y + title.h);
    SDL_RenderPresent(gRenderer);
    currentState = DEAD;
    reset();
}

void SnakeGame::reset() {
    snake.reset(width, height);
    score.reset();
    do {
        fruit.x = rand() % (width / 20) * 20;
        fruit.y = rand() % (game_height / 20) * 20;
    } while (snake.contains(fruit));
}

void SnakeGame::render() {
    SDL_RenderSetViewport(gRenderer, NULL);
    gTextureBackground.render({gGameRect.x, gGameRect.y, width, game_height});

    snake.render();

    gTextureApple.render({fruit.x, fruit.y, 20, 20});

    score.render(gInfoRect, width);
    SDL_RenderSetViewport(gRenderer, NULL);

    SDL_RenderPresent(gRenderer);
}

bool SnakeGame::loadMedia() {
    bool success = true;
    gTextureBackground.loadFromFile(gResourcesPath + "ground.png");
    gTextureApple.loadFromFile(gResourcesPath + "apple.png");
    gFontNormal =
        TTF_OpenFont((gResourcesPath + "ARCADECLASSIC.TTF").c_str(), 20);
    if (gFontNormal == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    gFontLarge =
        TTF_OpenFont((gResourcesPath + "ARCADECLASSIC.TTF").c_str(), 48);
    if (gFontLarge == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    return success;
}