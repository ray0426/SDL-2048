#include "game_2048_change.h"
#include <iostream>
#include <time.h>
#include <vector>
#include <cmath>

using std::vector;
using std::cin;
using std::cout;
using std::endl;

Game2048Change::Game2048Change(int gamesize) : Game2048Standard(gamesize) {
    board_mark = create_empty_board();
    mark_offset = ceil(log2(gamesize * gamesize));
}

Game2048Change::~Game2048Change() {
    delete_board(board_mark);
}

void Game2048Change::random_add_brick() {
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
    
    newtiles.push_back(TileStatus(pos, pos, ran_num, true, false));
}

int Game2048Change::start() {
    srand(time(NULL));
    newtiles.clear();
    movements.clear();
    current_tiles.clear();
    random_add_brick();
    random_add_brick();
    find_current_tiles();

    return 0;
}

void Game2048Change::init_board_mark() {
    int mark = 0;
    for (int i = 0; i < gamesize; i++) {
        for (int j = 0; j < gamesize; j++) {
            if (board[i][j] != 0) {
                board_mark[i][j] = mark;
            } else {
                board_mark[i][j] = -1;
            }
            mark++;
        }
    }
}

int Game2048Change::exec_move(Move move, bool trial) {
    int score = 0;
    Board board_origin = copy_board(board);
    int same;

    init_board_mark();
    // print_board(board_mark, false);
    // std::cout << "============ before move" << std::endl;
    // print_board(board, false);

    switch (move) {
        case Move::UP:
            rotate_clockwise(board);
            rotate_clockwise(board);
            rotate_clockwise(board);
            rotate_clockwise(board_mark);
            rotate_clockwise(board_mark);
            rotate_clockwise(board_mark);
            shift_left();
            score = merge_left();
            shift_left();
            rotate_clockwise(board);
            rotate_clockwise(board_mark);
            break;
        case Move::RIGHT:
            rotate_clockwise(board);
            rotate_clockwise(board);
            rotate_clockwise(board_mark);
            rotate_clockwise(board_mark);
            shift_left();
            score = merge_left();
            shift_left();
            rotate_clockwise(board);
            rotate_clockwise(board);
            rotate_clockwise(board_mark);
            rotate_clockwise(board_mark);
            break;
        case Move::DOWN:
            rotate_clockwise(board);
            rotate_clockwise(board_mark);
            shift_left();
            score = merge_left();
            shift_left();
            rotate_clockwise(board);
            rotate_clockwise(board);
            rotate_clockwise(board);
            rotate_clockwise(board_mark);
            rotate_clockwise(board_mark);
            rotate_clockwise(board_mark);
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
            newtiles.clear();
            movements.clear();
            current_tiles.clear();
            random_add_brick();
            find_movements();
            find_current_tiles();

            // std::cout << "============ now" << std::endl;
            // print_board(board_mark, false);
            std::cout << "============ now" << std::endl;
            print_board(board, false);
        }
    }

    if (same) return 1;
    return 0;
}

int Game2048Change::shift_left() {
    for (int i = 0; i < gamesize; i++) {
        int k = 0;
        for (int j = 0; j < gamesize; j++) {
            if (board[i][j] != 0) {
                board[i][k] = board[i][j];
                
                board_mark[i][k] = board_mark[i][j];
                k++;
            }
        }
        for (; k < gamesize; k++) {
            board[i][k] = 0;
            board_mark[i][k] = -1;
        }
    }

    return 0;
}

int Game2048Change::merge_left() {
    int score = 0;
    for (int i = 0; i < gamesize; i++) {
        for (int j = 0; j < gamesize - 1; j++) {
            if (board[i][j] != 0) {
                if (board[i][j] == board[i][j + 1]) {
                    board[i][j] = board[i][j] + 1;
                    score += (1 << board[i][j]);
                    max_tile = std::max(max_tile, board[i][j]);
                    board[i][j + 1] = 0;
                    
                    board_mark[i][j] = board_mark[i][j] + (board_mark[i][j + 1] << mark_offset) + (1 << (mark_offset * 2));
                    board_mark[i][j + 1] = -1;
                    j++;
                }
            }
        }
    }

    return score;
}

void Game2048Change::find_movements() {
    for (int i = 0; i < gamesize; i++) {
        for (int j = 0; j < gamesize; j++) {
            int value = board_mark[i][j];
            if (value < 0) {
                continue;
            } else if ((value >> (mark_offset * 2)) == 0) {
                movements.push_back(
                    TileStatus(
                        std::make_pair(value / gamesize, value % gamesize),
                        std::make_pair(i, j),
                        board[i][j],
                        false,
                        false
                    )
                );
            } else {
                movements.push_back(
                    TileStatus(
                        std::make_pair((value % (1 << mark_offset)) / gamesize, (value % (1 << mark_offset)) % gamesize),
                        std::make_pair(i, j),
                        board[i][j] - 1,
                        false,
                        false
                    )
                );
                value = (value >> mark_offset) % (1 << mark_offset);
                movements.push_back(
                    TileStatus(
                        std::make_pair(value / gamesize, value % gamesize),
                        std::make_pair(i, j),
                        board[i][j] - 1,
                        false,
                        true
                    )
                );
            }
        }
    }
}

void Game2048Change::find_current_tiles() {
    for (int i = 0; i < gamesize; i++) {
        for (int j = 0; j < gamesize; j++) {
            if (board[i][j] != 0) {
                current_tiles.push_back(
                    TileStatus(
                        std::make_pair(i, j),
                        std::make_pair(i, j),
                        board[i][j],
                        false,
                        false
                    )
                );
            }
        }
    }
}

std::vector<TileStatus>& Game2048Change::getNewtiles(){
    return newtiles;
}

std::vector<TileStatus>& Game2048Change::getMovements() {
    return movements;
}

std::vector<TileStatus>& Game2048Change::getCurrentTiles() {
    return current_tiles;
}