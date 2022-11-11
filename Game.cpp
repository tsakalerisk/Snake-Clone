#include "Game.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

Game::Game(std::string name, const int width, const int height)
    : width(width), height(height) {
    if (!SDL_Init(name, width, height)) printf("SDL initialization error!");
}

Game::~Game() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Game::SDL_Init(std::string name, const int width, const int height) {
    bool success = true;
    if (::SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialiaze! SDL_Error: %s\n", SDL_GetError());
        success = false;
    } else {
        gWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, width, height,
                                   SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n",
                   SDL_GetError());
            success = false;
        } else {
            gRenderer =
                SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL_Error: %s\n",
                       SDL_GetError());
                success = false;
            } else {
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);

                if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
                    printf("SDL_Image could not initialize! SDL_Error: %s\n",
                           IMG_GetError());
                    success = false;
                }

                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                           TTF_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}