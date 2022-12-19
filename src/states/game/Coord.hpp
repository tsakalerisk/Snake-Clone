#pragma once

typedef struct {
    int x, y;
} coord;

inline bool operator==(const coord& lhs, const coord& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}