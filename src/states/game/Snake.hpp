#pragma once

#include <SDL.h>

#include <list>

#include "../../utils/Texture.hpp"
#include "Coord.hpp"

extern std::string gResourcesPath;

typedef enum { DEFAULT = -1, UP, RIGHT, DOWN, LEFT } Heading;

enum BodyPart {
    HEAD,
    BODY_YELLOW,
    BODY_BLACK,
    CORNER_YELLOW,
    CORNER_BLACK,
    TAIL_BLACK,
    TAIL_YELLOW
};

class Snake {
   private:
    Texture mTextureWorm;
    std::list<coord> mList;
    Heading mHeading;
    int mGameWidth, mGameHeight;

    void empty();
    BodyPart selectTile(std::list<coord>::iterator iter, double* rotation, int i);
    void addNode(coord node);

   public:
    Snake(std::string skin);
    void init(int width, int height);
    bool advance(int game_width, int game_height);
    bool contains(coord item);
    coord getHead() { return mList.front(); };
    Heading getHeading() { return mHeading; };
    void grow();
    void render();
    void reset();
    void turn(SDL_Keycode direction);
    void changeSkin(std::string path);
};