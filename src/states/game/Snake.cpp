#include "Snake.hpp"

#include <algorithm>

Snake::Snake(std::string skin) { mTextureWorm.loadFromFile(skin); };

void Snake::init(int width, int height) {
    gameWidth = width;
    gameHeight = height;
    reset();
}

void Snake::empty() {
    while (!list.empty()) list.pop_back();
}

void Snake::addNode(coord node) { list.push_back(node); }

void Snake::grow() { list.push_back(list.back()); };

void Snake::reset() {
    empty();
    for (int i = 0; i < 3; i++) {
        addNode({gameWidth / 2, gameHeight / 2 + (i - 1) * 20});
    }
    heading = DEFAULT;
}

bool Snake::contains(coord item) {
    for (auto const& i : list) {
        if (i == item) return true;
    }
    return false;
}

void Snake::render() {
    int i = 0;
    for (auto it = list.begin(); it != list.end(); it++) {
        double rotation;
        int tile = SelectTile(it, &rotation, i);
        SDL_Rect clip = {20 * tile, 0, 20, 20};
        mTextureWorm.render(it->x, it->y, &clip, rotation);
        i++;
    }
}

BodyPart Snake::SelectTile(std::list<coord>::iterator iter, double* rotation, int i) {
    if (*iter == *(list.begin())) {
        *rotation = heading == DEFAULT ? 0 : heading * 90;
        return HEAD;
    } else if (*iter == *(std::prev(list.end()))) {
        auto prev = std::prev(iter);

        // hack to figure out orientation when two tails are on top
        // of each other when the worm grows
        if (*prev == *iter) prev--;

        if (prev->x == iter->x) {  // up or down
            *rotation = 180 * (prev->y == iter->y + 20);
        } else {  // left or right
            *rotation = 90 + 180 * (prev->x == iter->x - 20);
        }
        return i % 2 == 0 ? TAIL_BLACK : TAIL_YELLOW;
    } else {
        auto pred = std::prev(iter);
        auto next = std::next(iter);

        // vertical or horizontal line
        if (pred->x == next->x || pred->y == next->y) {
            *rotation = 90 * (pred->y == next->y);
            return i % 2 == 0 ? BODY_BLACK : BODY_YELLOW;
        } else {
            if (pred->y == iter->y - 20 || next->y == iter->y - 20) {  // J or L
                *rotation = 90 * !(pred->x == iter->x - 20 || next->x == iter->x - 20);
            } else {  // gamma or mirror gamma
                *rotation = 180 + 90 * (pred->x == iter->x - 20 || next->x == iter->x - 20);
            }
            return i % 2 == 0 ? CORNER_BLACK : CORNER_YELLOW;
        }
    }
}

bool Snake::advance(int game_width, int game_height) {
    coord new_head = list.front();
    switch (heading) {
        case UP:
            if (new_head.y >= 20) {
                new_head.y -= 20;
            } else
                return false;
            break;
        case DOWN:
            if (new_head.y < game_height - 20) {
                new_head.y += 20;
            } else
                return false;
            break;
        case LEFT:
            if (new_head.x >= 20) {
                new_head.x -= 20;
            } else
                return false;
            break;
        case RIGHT:
            if (new_head.x < game_width - 20) {
                new_head.x += 20;
            } else
                return false;
            break;
        default:
            break;
    }
    if (contains(new_head)) return false;
    list.pop_back();
    list.push_front(new_head);
    return true;
}

void Snake::turn(Heading direction) {
    switch (direction) {
        case UP:
            if (heading != DOWN) {
                heading = UP;
            }
            break;
        case DOWN:
            if (heading != UP && heading != DEFAULT) {
                heading = DOWN;
            }
            break;
        case LEFT:
            if (heading != RIGHT) {
                heading = LEFT;
            }
            break;
        case RIGHT:
            if (heading != LEFT) {
                heading = RIGHT;
            }
            break;
        default:
            break;
    }
}

void Snake::changeSkin(std::string path) { mTextureWorm.loadFromFile(path); }