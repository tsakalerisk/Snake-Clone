#include <SDL.h>
#include <SDL_ttf.h>

#include <queue>
#include <string>

class Game {
   private:
    bool SDL_Init(std::string name, const int width, const int height);

   public:
    std::queue<SDL_Event> KeyPressQ;

    bool quit;

    const int width, height;

    Game(std::string name, const int width, const int height);
    ~Game();
    virtual void init() = 0;
    virtual void loop(Uint32 elapsed_time) = 0;
    virtual void handle_event(SDL_Event e, bool* quit) = 0;
};