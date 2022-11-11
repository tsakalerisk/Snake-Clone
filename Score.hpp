#include <stdio.h>

#include <iomanip>
#include <sstream>

#include "Texture.hpp"

extern SDL_Renderer* gRenderer;

class Score {
   private:
    long value, highscore;
    Texture gTextTexture;
    TTF_Font* gFont = NULL;

   public:
    Score(TTF_Font* gFont) : gFont(gFont) {
        value = 0;
        highscore = GetHighScore();
    }

    long get() { return value; }

    void add(long amount) {
        if ((value += amount) > highscore) highscore = value;
    }

    void reset() {
        if (value > GetHighScore()) SetHighScore(value);
        value = 0;
    }

    long GetHighScore() {
        FILE* highscore_file = fopen("highscore.worm", "rb");
        long highscore = 0;
        if (highscore_file != NULL) {
            fread(&highscore, sizeof(long), 1, highscore_file);
            fclose(highscore_file);
        }
        return highscore;
    }

    void SetHighScore(long highscore) {
        FILE* highscore_file = fopen("highscore.worm", "wb");
        if (highscore_file != NULL) {
            fwrite(&highscore, sizeof(long), 1, highscore_file);
            fclose(highscore_file);
        }
    }

    void render(SDL_Rect infoRect, int screen_width) {
        SDL_Rect info_viewport = infoRect;
        info_viewport.x = screen_width / 2;
        info_viewport.w /= 2;
        SDL_RenderSetViewport(gRenderer, &info_viewport);
        SDL_SetRenderDrawColor(gRenderer, 0x10, 0x10, 0x10, 0xff);
        SDL_RenderFillRect(gRenderer, NULL);

        std::ostringstream score_stream;
        score_stream << "SCORE  " << std::setw(8) << std::setfill('0') << value;
        gTextTexture.loadFromRenderedText(score_stream.str(), gFont,
                                          {0xff, 0xff, 0xff});
        gTextTexture.render(info_viewport.w - gTextTexture.getWidth(), 0);

        std::ostringstream highscore_stream;
        highscore_stream << "HIGH  " << std::setw(8) << std::setfill('0')
                         << highscore;
        gTextTexture.loadFromRenderedText(highscore_stream.str(), gFont,
                                          {0xff, 0xff, 0xff});
        gTextTexture.render(info_viewport.w - gTextTexture.getWidth(),
                            gTextTexture.getHeight());
    }
};