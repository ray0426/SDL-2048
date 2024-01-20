#include <iostream>
#include <cmath>
#include "game_2048_standard.h"
#include <time.h>
#include <vector>
#include <cassert>

#define LIMIT 1000

using std::vector;
using std::cin;
using std::cout;
using std::endl;

int random_with_weight(const float weights[], int size);

Game2048Standard::Game2048Standard(int gamesize) : gamesize(gamesize) {}

Game2048Standard::~Game2048Standard() {
    delete_board(board);
    delete_board(board_tmp);
}

int Game2048Standard::init() {
    delete_board(board);
    delete_board(board_tmp);
    board = create_empty_board();
    board_tmp = create_empty_board();
    srand(time(NULL));
    random_add_brick();
    random_add_brick();

    return 0;
}

Board Game2048Standard::get_board() {
    return board;
}

void Game2048Standard::print_board() {
    int width = floor(log10(max_tile + 1)) + 1;
    cout << "===================" << endl;
    cout << "total score: " << total_score << 
            ", total moves: " << total_moves << endl;
    // cout << "new count: " << new_count << 
    //         ", delete count: " << delete_count << endl;
    for (int i = 0; i < gamesize; i++) {
        for (int j = 0; j < gamesize; j++) {
            cout.width(width);
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

Board Game2048Standard::create_empty_board() {
    Board board_tmp = new int* [gamesize];
    for (int i = 0; i < gamesize; i++) {
        board_tmp[i] = new int [gamesize]{0};
    }
    // new_count++;
    return board_tmp;
}

void Game2048Standard::clear_board(Board &board) {
    for (int i = 0; i < gamesize; i++) {
        for (int j = 0; j < gamesize; j++) {
            board[i][j] = 0;
        }
    }
}

int Game2048Standard::delete_board(Board &board_tmp) {
    if (board_tmp != nullptr) {
        for (int i = 0; i < gamesize; i++) {
            delete [] board_tmp[i];
        }
        delete board_tmp;
        board_tmp = nullptr;
    }
    // delete_count++;
    return 0;
}

Board Game2048Standard::copy_board(Board board_in) {
    if (board_in == nullptr) assert(false);
    Board board_copied = create_empty_board();
    for (int i = 0; i < gamesize; i++) {
        for (int j = 0; j < gamesize; j++) {
            board_copied[i][j] = board_in[i][j];
        }
    }
    return board_copied;
}

void Game2048Standard::swap_board(Board &board_1, Board &board_2) {
    Board board_tmp;
    board_tmp = board_1;
    board_1 = board_2;
    board_2 = board_tmp;
}

int Game2048Standard::compare_board(Board board1, Board board2) {
    if (board1 == nullptr || board2 == nullptr) return -1;
    for (int i = 0; i < gamesize; i++) {
        for (int j = 0; j < gamesize; j++) {
            if (board1[i][j] != board2[i][j]) return 1;
        }
    }
    return 0;
}

vector<Pos> Game2048Standard::find_empties() {
    vector<Pos> empties;

    for (int i = 0; i < gamesize; i++) {
        for (int j = 0; j < gamesize; j++) {
            if (board[i][j] == 0) {
                empties.push_back(std::make_pair(i, j));
            }
        }
    }
    return empties;
}

int Game2048Standard::has_empty() {
    for (int i = 0; i < gamesize; i++) {
        for (int j = 0; j < gamesize; j++) {
            if (board[i][j] == 0) return 1;
        }
    }
    return 0;
}

int Game2048Standard::is_gameover() {
    Board board_origin;
    int same;

    for (int i = 0; i < GAME_2048_MOVE_TYPES; i++) {
        board_origin = copy_board(board);
        same = exec_move(moves[i], true);
        if (!same) {
            // If difference
            delete_board(board);
            board = board_origin;
            return 0;
        }
        delete_board(board_origin);
    }

    return 1;
}

void Game2048Standard::add_brick(Pos pos, int num) {
    board[pos.first][pos.second] = num;
    max_tile = std::max(max_tile, num);
}

void Game2048Standard::random_add_brick() {
    int ran_idx;
    int ran_num;
    Pos pos;
    vector<Pos> empties = find_empties();

    if (empties.size() == 0) {
        cout << "[Error] No empty" << endl;
        return;
    }

    ran_idx = rand() % empties.size();
    pos = empties.at(ran_idx);
    ran_num = random_with_weight(tile_prob, 2) + 1;
    add_brick(pos, ran_num);
}

int Game2048Standard::exec_move(Move move, bool trial) {
    int score = 0;
    Board board_origin = copy_board(board);
    int same;

    switch (move) {
        case Move::UP:
            rotate_clockwise(board);
            rotate_clockwise(board);
            rotate_clockwise(board);
            shift_left();
            score = merge_left();
            shift_left();
            rotate_clockwise(board);
            break;
        case Move::RIGHT:
            rotate_clockwise(board);
            rotate_clockwise(board);
            shift_left();
            score = merge_left();
            shift_left();
            rotate_clockwise(board);
            rotate_clockwise(board);
            break;
        case Move::DOWN:
            rotate_clockwise(board);
            shift_left();
            score = merge_left();
            shift_left();
            rotate_clockwise(board);
            rotate_clockwise(board);
            rotate_clockwise(board);
            break;
        case Move::LEFT:
            shift_left();
            score = merge_left();
            shift_left();
            break;
        default:
            cout << "[Error] Invalid move" << endl;
            break;
    }
    same = !compare_board(board, board_origin);
    delete_board(board_origin);

    if (!trial) {
        total_score += score;
        if (!same) {
            total_moves++;
            random_add_brick();
        }
    }

    if (same) return 1;
    return 0;
}

int Game2048Standard::rotate_clockwise(Board &board_rotate) {
    clear_board(board_tmp);
    for (int i = 0; i < gamesize; i++) {
        for (int j = 0; j < gamesize; j++) {
            board_tmp[i][j] = board_rotate[gamesize - j - 1][i];
        }
    }
    swap_board(board_rotate, board_tmp);

    return 0;
}

int Game2048Standard::shift_left() {
    for (int i = 0; i < gamesize; i++) {
        int k = 0;
        for (int j = 0; j < gamesize; j++) {
            if (board[i][j] != 0) {
                board[i][k] = board[i][j];
                k++;
            }
        }
        for (; k < gamesize; k++) {
            board[i][k] = 0;
        }
    }

    return 0;
}

int Game2048Standard::merge_left() {
    int score = 0;
    for (int i = 0; i < gamesize; i++) {
        for (int j = 0; j < gamesize - 1; j++) {
            if (board[i][j] != 0) {
                if (board[i][j] == board[i][j + 1]) {
                    board[i][j] = board[i][j] + 1;
                    score += (1 << board[i][j]);
                    max_tile = std::max(max_tile, board[i][j]);
                    board[i][j + 1] = 0;
                    j++;
                }
            }
        }
    }

    return score;
}

int random_with_weight(const float weights[], int size) {
    const int limit = 1000;
    float rand_float = 1. * (rand() % limit) / limit;

    if (rand_float < weights[0]) return 0;

    for (int i = 1; i < size; i++) {
        if (rand_float < weights[i]) return i;
    }
    return -1;
}

// TODO: 
// (). change board to binary (similar to https://github.com/nneonneo/2048-ai)
