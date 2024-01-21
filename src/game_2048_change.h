#ifndef GAME_2048_CHANGE_H_
#define GAME_2048_CHANGE_H_
#include <vector>
#include "game_2048_basic.h"
#include "game_2048_standard.h"

struct TileStatus {
    Pos pos_start, pos_end;
    int value;
    // won't be true at the same time
    bool will_appear;
    bool will_disappear;

    TileStatus(Pos p1, Pos p2, int v, bool app, bool dis) : 
        pos_start(p1), pos_end(p2), value(v), will_appear(app), will_disappear(dis) {}
};

class Game2048Change : public Game2048Standard {
    private:
        std::vector<TileStatus> newtiles;
        std::vector<TileStatus> movements;
        std::vector<TileStatus> current_tiles;
        Board board_mark = nullptr;
        int mark_offset;
    public:
        Game2048Change(int gamesize);
        ~Game2048Change();

        void random_add_brick();
        int start();
        void init_board_mark();
        int exec_move(Move move, bool trial = false);
        int shift_left();
        int merge_left();
        void find_movements();
        void find_current_tiles();
        std::vector<TileStatus>& getNewtiles();
        std::vector<TileStatus>& getMovements();
        std::vector<TileStatus>& getCurrentTiles();
};


#endif