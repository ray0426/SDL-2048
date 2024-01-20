#ifndef GAME_2048_BASIC_H_
#define GAME_2048_BASIC_H_

typedef std::pair<int, int> Pos;

#define GAME_2048_MOVE_TYPES 4

enum class Move {
    STOP,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

#endif