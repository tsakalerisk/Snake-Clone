#include "Snake.hpp"

#include <algorithm>

Snake::Snake(std::string skin) { mTextureWorm.loadFromFile(skin); };

void Snake::init(int width, int height) {
    mGameWidth = width;
    mGameHeight = height;
    reset();
}

void Snake::empty() {
    while (!mList.empty()) mList.pop_back();
}

void Snake::addNode(coord node) { mList.push_back(node); }

void Snake::grow() { mList.push_back(mList.back()); };

void Snake::reset() {
    empty();
    for (int i = 0; i < 3; i++) {
        addNode({mGameWidth / 2, mGameHeight / 2 + (i - 1) * 20});
    }
    mHeading = DEFAULT;
}

bool Snake::contains(coord item) {
    for (auto const& i : mList) {
        if (i == item) return true;
    }
    return false;
}

void Snake::render() {
    int i = 0;
    for (auto it = mList.begin(); it != mList.end(); it++) {
        double rotation;
        int tile = selectTile(it, &rotation, i);
        SDL_Rect clip = {20 * tile, 0, 20, 20};
        mTextureWorm.render(it->x, it->y, &clip, rotation);
        i++;
    }
}

BodyPart Snake::selectTile(std::list<coord>::iterator iter, double* rotation, int i) {
    if (*iter == *(mList.begin())) {
        *rotation = mHeading == DEFAULT ? 0 : mHeading * 90;
        return HEAD;
    } else if (*iter == *(std::prev(mList.end()))) {
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
    coord new_head = mList.front();
    switch (mHeading) {
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
    mList.pop_back();
    mList.push_front(new_head);
    return true;
}

void Snake::turn(SDL_Keycode direction) {
    switch (direction) {
        case SDLK_UP:
            if (mHeading != DOWN) {
                mHeading = UP;
            }
            break;
        case SDLK_DOWN:
            if (mHeading != UP && mHeading != DEFAULT) {
                mHeading = DOWN;
            }
            break;
        case SDLK_LEFT:
            if (mHeading != RIGHT) {
                mHeading = LEFT;
            }
            break;
        case SDLK_RIGHT:
            if (mHeading != LEFT) {
                mHeading = RIGHT;
            }
            break;
        default:
            break;
    }
}

void Snake::changeSkin(std::string path) { mTextureWorm.loadFromFile(path); }