#include <iostream>
#include "src/game_2048_standard.h"

using std::vector;
using std::cin;
using std::cout;
using std::endl;

int main() {
    char move;
    Game2048Standard game(4);
    game.init();
    game.start();
    game.print_board();
    while (!game.is_gameover()) {
        cin >> move;
        switch (move) {
        case 'w':
            game.exec_move(Move::UP);
            break;
        case 'd':
            game.exec_move(Move::RIGHT);
            break;
        case 's':
            game.exec_move(Move::DOWN);
            break;
        case 'a':
            game.exec_move(Move::LEFT);
            break;
        default:
            break;
        }
        game.print_board();
    }
    cout << "game over!" << endl;
    
    return 0;
}