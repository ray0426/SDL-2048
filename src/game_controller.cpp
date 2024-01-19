#include "game_controller.h"

GameController::GameController(int gamesize) : gamelogic(gamesize), gamesize(gamesize) {}

GameController::~GameController() {
    DEBUG_MSG("gamecontroller destructor");
    free();
}

void GameController::free() {
    DEBUG_MSG("free");
    gamelogic.~Game2048Standard();

    //Free loaded images
    gSpriteSheetTexture.free();
}

bool GameController::init() {
    return gamelogic.init();
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
        move = Move::LEFT;
        gamelogic.exec_move(move);
        DEBUG_MSG("UP");
    }
    else if( currentKeyStates[ SDL_SCANCODE_DOWN ] && e.key.repeat == 0  )
    {
        move = Move::RIGHT;
        gamelogic.exec_move(move);
        DEBUG_MSG("DOWN");
    }
    else if( currentKeyStates[ SDL_SCANCODE_LEFT ] && e.key.repeat == 0  )
    {
        move = Move::UP;
        gamelogic.exec_move(move);
        DEBUG_MSG("LEFT");
    }
    else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] && e.key.repeat == 0  )
    {
        move = Move::DOWN;
        gamelogic.exec_move(move);
        DEBUG_MSG("RIGHT");
    }
    else
    {
        move = Move::STOP;
    }
};

void GameController::render() {
    Board board = gamelogic.get_board();

    for ( int i = 0; i < 4; i++ ) {
    	for ( int j = 0; j < 4; j++ ) {
    		gSpriteSheetTexture.render( i * 270 + 7, j * 270 + 7, &gSpriteClips[ board[i][j] ] );
    	}
    }
};

void GameController::setGSpriteClips(int idx, int x, int y, int w, int h) {
    gSpriteClips[ idx ].x = x;
    gSpriteClips[ idx ].y = y;
    gSpriteClips[ idx ].w = w;
    gSpriteClips[ idx ].h = h;
}

bool GameController::start() {
    return (gamelogic.init() == 0);
}