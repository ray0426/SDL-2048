#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_
#include <SDL2/SDL.h>
#include "game_2048_standard.h"
#include "utils.h"
#include "LTexture.h"

class GameController {
    public:
        GameController(int gamesize);
        ~GameController();
        void free();
        bool init();
        bool loadMedia();
        void handleEvent( SDL_Event& e );
        void render();

        // Set gSpriteClips
        void setGSpriteClips(int idx, int x, int y, int w, int h);

        bool start();
    private:
        int gamesize;
        Game2048Standard gamelogic;
        SDL_Rect gSpriteClips[ 20 ];
        LTexture gSpriteSheetTexture;
        Move move;
};

#endif