#pragma once
#include <SDL.h>

#include <string>
#include <vector>

class GameState;

class Game {
   private:
    bool SDL_Init(std::string name, int width, int height);
    bool mRunning = true;
    std::vector<GameState*> mStates;

   public:
    const int width, height;
    const int info_height = 40, game_height = height - info_height;
    const SDL_Rect gGameRect = {0, 0, width, game_height};
    const SDL_Rect gInfoRect = {0, game_height, width, info_height};

    Game(std::string name, int width, int height);
    ~Game();

    void quit() { mRunning = false; }

    void changeState(GameState* state);
    void pushState(GameState* state);
    void popState();

    void handleEvent(SDL_Event e);
    void update(Uint32 elapsedTime);
    void render();
};