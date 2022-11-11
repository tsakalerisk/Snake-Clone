#include <SDL.h>
#include <SDL_ttf.h>

#include "Game.hpp"
#include "Score.hpp"
#include "Snake.hpp"
#include "Texture.hpp"
#include "coord.hpp"

extern std::string gResourcesPath;

class SnakeGame : public Game {
   public:
    Texture gTextureBackground;
    Texture gTextureApple;
    Texture gTextureWorm;
    Texture gTextTexture;
    TTF_Font* gFontNormal;
    TTF_Font* gFontLarge;

    const int info_height = 40, game_height = height - info_height;
    const SDL_Rect gGameRect = {0, 0, width, game_height};
    const SDL_Rect gInfoRect = {0, game_height, width, info_height};

    enum State { PLAYING, DEAD };

    Snake snake;
    coord fruit;
    Score score;
    State currentState = PLAYING;

    SnakeGame(std::string name, const int width, const int height);
    ~SnakeGame();
    void init();
    void onDeath();
    void handle_event(SDL_Event e, bool* quit);
    void handle_game_event(SDL_Event e);
    void handle_dead_event(SDL_Event e);
    bool loadMedia();
    void loop(Uint32 elapsed_time);
    void render();
    void reset();
};