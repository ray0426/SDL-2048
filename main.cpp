#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
// #include <SDL2/SDL_ttf.h>
// #include <SDL2/SDL_mixer.h>
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

// //Globally used font
// TTF_Font* gFont = NULL;

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
	SDL_SetRenderDrawColor( gRenderer, 0xFA, 0xF8, 0xEF, 0xFF );

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return false;
	}

	// Initialize game
	if ( game.init(gRenderer) != 0 ) {
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

	// //Open the font
	// gFont = TTF_OpenFont( "assets/fonts/ClearSans-Bold.ttf", 28 );
	// if( gFont == NULL ) {
	// 	printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
	// 	return false;
	// }

	return true;
}

void close()
{
	//Free game
	game.free();

	// //Free global font
	// TTF_CloseFont( gFont );
	// gFont = NULL;

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
					SDL_SetRenderDrawColor( gRenderer, 0xFA, 0xF8, 0xEF, 0xFF );
					SDL_RenderClear( gRenderer );

					//Update game
					game.update();

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

/*
TODO
title and score
readme
round square
https://www.google.com/search?q=sdl+ttf+width&sca_esv=601388725&ei=50eyZez1Mcrc2roP3oKi8A4&ved=0ahUKEwishcTNufiDAxVKrlYBHV6BCO4Q4dUDCBA&uact=5&oq=sdl+ttf+width&gs_lp=Egxnd3Mtd2l6LXNlcnAiDXNkbCB0dGYgd2lkdGgyBxAhGAoYoAFI0RxQiAdYkhtwAngBkAEAmAFUoAGjA6oBATe4AQPIAQD4AQHCAgoQABhHGNYEGLADwgIFEAAYgATCAgQQABgewgIGEAAYCBge4gMEGAAgQYgGAZAGCg&sclient=gws-wiz-serp
https://stackoverflow.com/questions/55864308/how-do-find-out-the-width-and-height-of-the-text-without-using-surface-in-sdl2
https://wiki.libsdl.org/SDL2_ttf/TTF_SizeText
https://www.google.com/search?q=sdl+multiple+text&oq=sdl+multiple+text&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBCDYxNDVqMGoxqAIAsAIA&sourceid=chrome&ie=UTF-8
https://gamedev.stackexchange.com/questions/46238/rendering-multiline-text-with-sdl-ttf
https://discourse.libsdl.org/t/displaying-multiple-lines-of-text-on-the-screen/5624
*/