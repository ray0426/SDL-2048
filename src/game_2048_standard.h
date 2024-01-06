#ifndef GAME2048STANDARD_H_
#define GAME2048STANDARD_H_
#include <vector>

#define GAME_2048_MOVE_TYPES 4

typedef int** Board;
typedef std::pair<int, int> Pos;

enum class Move {
    STOP,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class Game2048Standard {
    private:
        int gamesize;
        Board board = nullptr;
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
        Board get_board();
        void print_board();
        Board create_empty_board();
        int delete_board(Board board_temp);
        Board copy_board(Board board_tmp);

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
        int rotate_clockwise();
};


#endif