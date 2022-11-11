#include <SDL.h>

#include <list>

#include "Texture.hpp"
#include "coord.hpp"

extern SDL_Renderer* gRenderer;
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
    Texture gTextureWorm;
    std::list<coord> list;
    Heading heading;
    void empty();
    BodyPart SelectTile(std::list<coord>::iterator iter, double* rotation,
                        int i);
    void addNode(coord node);

   public:
    Snake();
    bool advance(int game_width, int game_height);
    bool contains(coord item);
    coord getHead() { return list.front(); };
    Heading getHeading() { return heading; };
    void grow();
    void render();
    void reset(int width, int height);
    void turn(Heading heading);
};