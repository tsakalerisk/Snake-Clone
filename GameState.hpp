#pragma once

#include "Game.hpp"

class GameState {
   public:
    virtual void init(Game* game) = 0;
    virtual void cleanup() = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual void handleEvent(Game* game, SDL_Event e) = 0;
    virtual void update(Game* game, Uint32 elapsed_time) = 0;
    virtual void render(Game* game) = 0;
};