#pragma once

#include "Font.hpp"
#include "GameState.hpp"
#include "Score.hpp"
#include "Snake.hpp"
#include "Texture.hpp"
#include "coord.hpp"

class PlayingState : public GameState {
   public:
    static PlayingState* instance() {
        static PlayingState an_istance;
        return &an_istance;
    }

    PlayingState();

    void init(Game* game) override;
    void cleanup() override{};

    void pause() override{};
    void resume() override{};

    void handleEvent(Game* game, SDL_Event e) override;
    void update(Game* game, Uint32 elapsed_time) override;
    void render(Game* game) override;

    void changeSnakeSkin(std::string path) { snake.changeSkin(path); }

    Texture gTextureBackground;
    Texture gTextureApple;
    Texture gTextureWorm;
    Texture gTextTexture;
    Font mFontNormal = Font(gResourcesPath + "ARCADECLASSIC.TTF", 20);

    Snake snake;
    coord fruit{};
    Score score;
};