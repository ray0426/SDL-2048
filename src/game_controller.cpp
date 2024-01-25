#include "game_controller.h"
#include "constants.h"
#include "utils.h"
#include <cmath>
#include <iostream>

GameController::GameController(int gamesize) : gamelogic(gamesize), gamesize(gamesize) {}

GameController::~GameController() {
    DEBUG_MSG("gamecontroller destructor");
    // free(); // repeated, should think where to call free() is better
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
    return 0;
};

bool GameController::loadMedia() {
    if( !gSpriteSheetTexture.loadFromFile( "assets/2048_tileset_default.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		return false;
	}
	
    // 0
    setGSpriteClips( 0, 4 * ASSET_BLOCK_SIZE, 1 * ASSET_BLOCK_SIZE, ASSET_BLOCK_SIZE, ASSET_BLOCK_SIZE );
    // 2 ~ 65536
    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            setGSpriteClips( i + j * 4 + 1, j * ASSET_BLOCK_SIZE, i * ASSET_BLOCK_SIZE, ASSET_BLOCK_SIZE, ASSET_BLOCK_SIZE );
        }
    }
    // background
    setGSpriteClips( 17, 4 * ASSET_BLOCK_SIZE, 0 * ASSET_BLOCK_SIZE, ASSET_BLOCK_SIZE, ASSET_BLOCK_SIZE );
    
    return true;
};

// The correctness of this handler function should be check
void GameController::handleEvent( SDL_Event& e ) {
    if (move == Move::STOP && state == Gamestate::S_READY) {
        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if( currentKeyStates[ SDL_SCANCODE_UP ] && e.key.repeat == 0  )
        {
            move = Move::LEFT;
        }
        else if( currentKeyStates[ SDL_SCANCODE_DOWN ] && e.key.repeat == 0  )
        {
            move = Move::RIGHT;
        }
        else if( currentKeyStates[ SDL_SCANCODE_LEFT ] && e.key.repeat == 0  )
        {
            move = Move::UP;
        }
        else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] && e.key.repeat == 0  )
        {
            move = Move::DOWN;
        }
    }
}

void GameController::update() {
    // gamestate FSM
    switch (state) {
        case Gamestate::S_READY:
            if (move != Move::STOP) {
                DEBUG_MSG(move);
                gamelogic.exec_move(move);

                move = Move::STOP;

                if (!gamelogic.get_same()) {
                    state = Gamestate::S_MOVE;
                    // add some time counter
                    t_animation_start = SDL_GetTicks();
                }
            }
            break;
        case Gamestate::S_MOVE:
                // add some time counter
                t_animation_now = SDL_GetTicks();
                // std::cout << "S_MOVE, " << t_animation_now - t_animation_start << std::endl;

                if (t_animation_now - t_animation_start >= GAME_MOVEMENT_TIME) {
                    state = Gamestate::S_ZOOM;
                    t_animation_start = t_animation_now;
                }
            break;
        case Gamestate::S_ZOOM:
                // add some time counter
                t_animation_now = SDL_GetTicks();
                // std::cout << "S_ZOOM, " << t_animation_now - t_animation_start << std::endl;

                if (t_animation_now - t_animation_start >= GAME_ZOOM_TIME) {
                    state = Gamestate::S_READY;
                }
            break;
        default:
            break;
    }
};

void GameController::render(int window_width, int window_height) {
    double scale = (double)std::min(window_width, window_height) / GAME_GAMEAREA_LENGTH;
    double block_scale = scale * GAME_BLOCK_SIZE / ASSET_BLOCK_SIZE;
    int gamearea_offset_x = std::max(int((window_width - GAME_GAMEAREA_LENGTH * scale) / 2), 0);
    int gamearea_offset_y = std::max(int((window_height - GAME_GAMEAREA_LENGTH * scale) / 2), 0);

    create_tiles();
    for ( int i = 0; i < 4; i++ ) {
    	for ( int j = 0; j < 4; j++ ) {
    		gSpriteSheetTexture.render( 
                gamearea_offset_x + (j * (GAME_BLOCK_SIZE + GAME_SPACING) + GAME_SPACING) * scale,
                gamearea_offset_y + (i * (GAME_BLOCK_SIZE + GAME_SPACING) + GAME_SPACING) * scale,
                &gSpriteClips[ 0 ],
                block_scale
                );
    	}
    }
    for (int i = 0; i < tiles.size(); i++) {
        gSpriteSheetTexture.render( 
            gamearea_offset_x + tiles[i].get_render_x() * scale,
            gamearea_offset_y + tiles[i].get_render_y() * scale,
            &tiles[i].get_gSpriteClip(),
            tiles[i].get_scale() * block_scale
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
    create_tiles();
    state = Gamestate::S_READY;

    return 0;
}

void GameController::create_tiles() {
    tiles.clear();
    t_animation_now = SDL_GetTicks();
    Uint32 t_diff = t_animation_now - t_animation_start;
    switch (state) {
        case Gamestate::S_READY: {
            std::vector<TileStatus>& current_tiles = gamelogic.getCurrentTiles();
            for (int i = 0; i < current_tiles.size(); i++) {
                tiles.push_back(
                    Tile(
                        get_pixel_from_pos(current_tiles[i].pos_end.first),
                        get_pixel_from_pos(current_tiles[i].pos_end.second),
                        GAME_BLOCK_SIZE,
                        1,
                        gSpriteClips[current_tiles[i].value]
                    )
                );
            }
            break;
        }
        case Gamestate::S_MOVE: {
            std::vector<TileStatus>& current_tiles = gamelogic.getMovements();
            for (int i = 0; i < current_tiles.size(); i++) {
                Pos start = current_tiles[i].pos_start;
                Pos end = current_tiles[i].pos_end;
                
                int x = shift_interpolation(start.first, end.first, t_diff, GAME_MOVEMENT_TIME);
                int y = shift_interpolation(start.second, end.second, t_diff, GAME_MOVEMENT_TIME);
                tiles.push_back(
                    Tile(
                        x,
                        y,
                        GAME_BLOCK_SIZE,
                        1,
                        gSpriteClips[current_tiles[i].value]
                    )
                );
            }
            break;
        }
        case Gamestate::S_ZOOM: {
            std::vector<TileStatus>& newtiles = gamelogic.getNewtiles();
            for (int i = 0; i < newtiles.size(); i++) {
                Pos start = newtiles[i].pos_start;
                Pos end = newtiles[i].pos_end;
                
                double scale = scale_interpolation(0, 1, t_diff, GAME_ZOOM_TIME);
                tiles.push_back(
                    Tile(
                        get_pixel_from_pos(end.first),
                        get_pixel_from_pos(end.second),
                        GAME_BLOCK_SIZE,
                        scale,
                        gSpriteClips[newtiles[i].value]
                    )
                );
            }
            std::vector<TileStatus>& movements = gamelogic.getMovements();
            for (int i = 0; i < movements.size(); i++) {
                Pos start = movements[i].pos_start;
                Pos end = movements[i].pos_end;
                double scale;

                if (movements[i].will_disappear) {
                    if ((double)t_diff / GAME_ZOOM_TIME < 0.5) {
                        scale = scale_interpolation(1, 1.3, t_diff, GAME_ZOOM_TIME);
                    } else {
                        scale = scale_interpolation(1.3, 1, t_diff, GAME_ZOOM_TIME);
                    }
                    tiles.push_back(
                        Tile(
                            get_pixel_from_pos(end.first),
                            get_pixel_from_pos(end.second),
                            GAME_BLOCK_SIZE,
                            scale,
                            gSpriteClips[movements[i].value]
                        )
                    );
                } else {
                    tiles.push_back(
                        Tile(
                            get_pixel_from_pos(end.first),
                            get_pixel_from_pos(end.second),
                            GAME_BLOCK_SIZE,
                            1,
                            gSpriteClips[movements[i].value]
                        )
                    );
                }
            }
            break;
        }
        default: {
            break;
        }
    }
}

int GameController::get_pixel_from_pos(int p) {
    return p * (GAME_BLOCK_SIZE + GAME_SPACING) + GAME_SPACING + GAME_BLOCK_SIZE / 2;
}

int GameController::shift_interpolation(int p1, int p2, Uint32 time_now, Uint32 time_total)
{
    double pixel1 = get_pixel_from_pos(p1);
    double pixel2 = get_pixel_from_pos(p2);
    int pixel_now = (pixel2 - pixel1) / time_total * time_now + pixel1;
    return pixel_now;
}

double GameController::scale_interpolation(double size1, double size2, Uint32 time_now, Uint32 time_total) {
    return (size2 - size1) / time_total * time_now + size1;
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

std::ostream &operator << ( std::ostream& os, Move move ) {
    switch (move) {
        case Move::STOP:
            return os << "[STOP]";
            break;
        case Move::UP:
            return os << "[LEFT]";
            break;
        case Move::RIGHT:
            return os << "[DOWN]";
            break;
        case Move::DOWN:
            return os << "[RIGHT]";
            break;
        case Move::LEFT:
            return os << "[UP]";
            break;
        default:
            return os;
            break;
    }
    return os;
}