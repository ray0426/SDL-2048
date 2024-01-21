#include "game_controller.h"
#include "constants.h"
#include <cmath>
#include <iostream>

GameController::GameController(int gamesize) : gamelogic(gamesize), gamesize(gamesize) {}

GameController::~GameController() {
    DEBUG_MSG("gamecontroller destructor");
    free();
}

void GameController::free() {
    DEBUG_MSG("free");
    gamelogic.delete_board(board);
    gamelogic.delete_board(board_prev);

    //Free loaded images
    gSpriteSheetTexture.free();
}

bool GameController::init() {
    gamelogic.init();
    board = gamelogic.copy_board(gamelogic.get_board());
    return 0;
};

bool GameController::loadMedia() {
    if( !gSpriteSheetTexture.loadFromFile( "assets/2048_tileset_default.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		return false;
	}
	
    // 0
    setGSpriteClips( 0, 4 * 256, 1 * 256, 256, 256 );
    // 2 ~ 65536
    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            setGSpriteClips( i + j * 4 + 1, j * 256, i * 256, 256, 256 );
        }
    }
    // background
    setGSpriteClips( 17, 4 * 256, 0 * 256, 256, 256 );
    
    return true;
};

void GameController::handleEvent( SDL_Event& e ) {
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    if( currentKeyStates[ SDL_SCANCODE_UP ] && e.key.repeat == 0  )
    {
        gamelogic.delete_board(board_prev);
        board_prev = board;
        move = Move::LEFT;
        gamelogic.exec_move(move);
        DEBUG_MSG("LEFT");
    }
    else if( currentKeyStates[ SDL_SCANCODE_DOWN ] && e.key.repeat == 0  )
    {
        gamelogic.delete_board(board_prev);
        board_prev = board;
        move = Move::RIGHT;
        gamelogic.exec_move(move);
        DEBUG_MSG("RIGHT");
    }
    else if( currentKeyStates[ SDL_SCANCODE_LEFT ] && e.key.repeat == 0  )
    {
        gamelogic.delete_board(board_prev);
        board_prev = board;
        move = Move::UP;
        gamelogic.exec_move(move);
        DEBUG_MSG("UP");
    }
    else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] && e.key.repeat == 0  )
    {
        gamelogic.delete_board(board_prev);
        board_prev = board;
        move = Move::DOWN;
        gamelogic.exec_move(move);
        DEBUG_MSG("DOWN");
    }
    else
    {
        move = Move::STOP;
    }
    if (move != Move::STOP) {
        board = gamelogic.copy_board(gamelogic.get_board());
        create_tiles();
    }
};

void GameController::render(int window_width, int window_height) {
    double scale = (double)std::min(window_width, window_height) / SCREEN_WIDTH;
    for ( int i = 0; i < 4; i++ ) {
    	for ( int j = 0; j < 4; j++ ) {
    		gSpriteSheetTexture.render( 
                (j * 270 + 7) * scale,
                (i * 270 + 7) * scale,
                &gSpriteClips[ 0 ],
                scale
                );
    	}
    }
    for (int i = 0; i < tiles.size(); i++) {
        gSpriteSheetTexture.render( 
            tiles[i].get_render_x() * scale,
            tiles[i].get_render_y() * scale,
            &tiles[i].get_gSpriteClip(),
            tiles[i].get_scale() * scale
        );
    }
};

void GameController::setGSpriteClips(int idx, int x, int y, int w, int h) {
    gSpriteClips[ idx ].x = x;
    gSpriteClips[ idx ].y = y;
    gSpriteClips[ idx ].w = w;
    gSpriteClips[ idx ].h = h;
}

bool GameController::start() {
    gamelogic.start();
    board = gamelogic.copy_board(gamelogic.get_board());
    create_tiles();

    return 0;
}

void GameController::create_tiles() {
    tiles.clear();
    std::vector<TileStatus>& current_tiles = gamelogic.getCurrentTiles();
    for (int i = 0; i < current_tiles.size(); i++) {
        tiles.push_back(
            Tile(
                (current_tiles[i].pos_end.first * 270 + 7 + 256 / 2),
                (current_tiles[i].pos_end.second * 270 + 7 + 256 / 2),
                256,
                1,
                gSpriteClips[current_tiles[i].value]
            )
        );
    }
}

Tile::Tile(int x, int y, int l, double s, SDL_Rect& r) : x(x), y(y), side_length(l), scale(s), gSpriteClip(r) {};

int Tile::get_x() {
    return x;
}

int Tile::get_y() {
    return y;
}

int Tile::get_render_x() {
    return x - side_length * scale / 2;
}

int Tile::get_render_y() {
    return y - side_length * scale / 2;
}

int Tile::get_side_length() {
    return side_length;
}

SDL_Rect& Tile::get_gSpriteClip() {
    return gSpriteClip;
}

double Tile::get_scale() {
    return scale;
}