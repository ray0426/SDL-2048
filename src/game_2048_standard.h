#ifndef GAME_2048_STANDARD_H_
#define GAME_2048_STANDARD_H_
#include <vector>
#include "game_2048_basic.h"

typedef int** Board;

class Game2048Standard {
    protected:
        int gamesize;
        Board board = nullptr, board_tmp = nullptr;
        int max_tile = 0;
        int total_moves = 0;
        int total_score = 0;
        const float tile_prob[2] = {0.9, 1.0};
        const Move moves[GAME_2048_MOVE_TYPES] = {Move::UP, Move::RIGHT, Move::DOWN, Move::LEFT};

        // int new_count = 0;
        // int delete_count = 0;
    public:
        Game2048Standard(int gamesize);
        ~Game2048Standard();
        int init();
        int start();
        Board get_board();
        void print_board();
        void print_board(Board board, bool row_major = true);
        Board create_empty_board();
        void clear_board(Board &board);
        int delete_board(Board &board_temp);
        Board copy_board(Board board_tmp);
        void swap_board(Board &board_1, Board &board_2);

        /**
         * Compare two `Board`
         *  \returns 0 if two board are the same; 1 if are not the same;
         *           -1 if one of them is `nullptr`.
         */
        int compare_board(Board board1, Board board2);
        std::vector<std::pair<int, int>> find_empties();
        int has_empty();

        /**
         * Check is the current board game over
         *  \returns 0 if not gameover; 1 if gameover
         */
        int is_gameover();
        void random_add_brick();
        int exec_move(Move move, bool trial = false);
        void add_brick(Pos pos, int num);
        int shift_left();
        int merge_left();
        int rotate_clockwise(Board &board_rotate);
};

int random_with_weight(const float weights[], int size);

#endif