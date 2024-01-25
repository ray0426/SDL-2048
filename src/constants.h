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

// Screen dimension constants
const int GAME_SCREEN_WIDTH = 720;
const int GAME_SCREEN_HEIGHT = 720;

// animation time
const Uint32 GAME_MOVEMENT_TIME = 80;
const Uint32 GAME_ZOOM_TIME = 80;

#endif