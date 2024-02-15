#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define DEBUG true

const int GAME_GAMESIZE = 4;

// pixel before resize
const int GAME_SPACING = 16;
const int GAME_BLOCK_SIZE = 240;

const int ASSET_BLOCK_SIZE = 256;

// side length of main game area
const int GAME_GAMEAREA_LENGTH = GAME_SPACING * (GAME_GAMESIZE + 1) + GAME_BLOCK_SIZE * GAME_GAMESIZE;

// game info height
const int GAME_INFO_HEIGHT = 0;//400;

const int GAME_TITLE_OFFSET_X = 5;
const int GAME_TITLE_OFFSET_Y = 50;
// const int GAME_SCORE_

const int GAME_TOTAL_WIDTH = GAME_GAMEAREA_LENGTH;
const int GAME_TOTAL_HEIGHT = GAME_INFO_HEIGHT + GAME_GAMEAREA_LENGTH;

// max side length
const double GAME_GAMEAREA_SCALE = (double)720 / GAME_TOTAL_WIDTH;
const int GAME_GAMEAREA_LENGTH_MAX = 720;


// Screen dimension constants
const int GAME_SCREEN_WIDTH = GAME_TOTAL_WIDTH * GAME_GAMEAREA_SCALE;
const int GAME_SCREEN_HEIGHT = GAME_TOTAL_HEIGHT * GAME_GAMEAREA_SCALE;

// animation time
const Uint32 GAME_MOVEMENT_TIME = 80;
const Uint32 GAME_ZOOM_TIME = 80;

#endif