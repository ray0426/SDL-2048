#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_
#include <SDL2/SDL.h>
#include "game_2048_change.h"
#include "utils.h"
#include "LTexture.h"
#include <vector>

class Tile;

class GameController {
    public:
        GameController(int gamesize);
        ~GameController();
        void free();
        bool init();
        bool loadMedia();
        void handleEvent( SDL_Event& e );
        void render(int window_width, int window_height);

        // Set gSpriteClips
        void setGSpriteClips(int idx, int x, int y, int w, int h);

        bool start();
        void create_tiles();
    private:
        int gamesize;
        Game2048Change gamelogic;
        Board board = nullptr, board_prev = nullptr;
        SDL_Rect gSpriteClips[ 20 ];
        LTexture gSpriteSheetTexture;
        Move move;
        std::vector<Tile> tiles;
};

class Tile {
    private:
        // position of center
        int x;
        int y;
        // side_length of the square
        int side_length;
        // animation scale
        double scale;
        // texture clip
        SDL_Rect &gSpriteClip;
    public:
        Tile(int x, int y, int l, double s, SDL_Rect& r);
        int get_x();
        int get_y();
        int get_render_x();
        int get_render_y();
        int get_side_length();
        double get_scale();
        SDL_Rect& get_gSpriteClip();
};

#endif