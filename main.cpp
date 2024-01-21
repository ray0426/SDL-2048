#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
// #include <SDL2/SDL_mixer.h>
// #include <SDL2/SDL_ttf.h>
// #include <graphics.h>

#include "src/LTexture.h"
#include "src/LWindow.h"
#include "src/utils.h"
#include "src/game_controller.h"

// Current board of 2048
Board board;

// Current game
// Game2048Standard game(4);
GameController game(4);

// Current move
Move move;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Our custom window
LWindow gWindow;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gSceneTexture;

bool init()
{
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//Set texture filtering to linear
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		printf( "Warning: Linear texture filtering not enabled!" );
	}

	//Create window
	if( !gWindow.init() )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//Create renderer for window
	gRenderer = gWindow.createRenderer();
	if( gRenderer == NULL )
	{
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	
	//Initialize renderer color
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return false;
	}

	// Initialize game
	if ( game.init() != 0 ) {
		printf( "Game initialize error\n" );
		return false;
	}

	return true;
}

bool loadMedia()
{
	//Load sprite sheet texture
	if ( !game.loadMedia()) {
		printf( "Failed to load sprite sheet texture!\n" );
		return false;
	}

	return true;
}

void close()
{
	//Free game
	game.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	gWindow.free();

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			
			game.start();

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle window events
					gWindow.handleEvent( e );

					// Move
					game.handleEvent( e );
				}

				int window_width = gWindow.getWidth();
				int window_height = gWindow.getHeight();

				//Only draw when not minimized
				if( !gWindow.isMinimized() ) {
					//Clear screen
					SDL_SetRenderDrawColor( gRenderer, 0xBB, 0xAD, 0xA0, 0xFF );
					SDL_RenderClear( gRenderer );

					//Render game
					game.render(window_width, window_height);

					//Update screen
					SDL_RenderPresent( gRenderer );
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
