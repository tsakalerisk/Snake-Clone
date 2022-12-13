#include "Game.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

std::string gResourcesPath = "..\\Resources\\";

SDL_Window* gWindow;
SDL_Renderer* gRenderer;

int main(int argc, char* agrs[]) {
    Game("Worm", SCREEN_WIDTH, SCREEN_HEIGHT);
    return 0;
}
