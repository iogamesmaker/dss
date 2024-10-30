#define WEB // uncomment for wasm building

// Program 100% written in the default KDE Plasma editor, using the "Comic Mono" font, with light mode.
// about 3% of code is commented, nice
#include "game.hpp"


// Shitton of classes because someone online told me they're good and very professional looking
Game *game = nullptr;
RandomEngine* RE = nullptr;
TextureManager* TM = nullptr;
SoundManager* SM = nullptr;
Stars* SB = nullptr;
TextEngine* TE = nullptr;
GUIManager* GUI = nullptr;
Ships* SH = nullptr;
ItemManager* IM = nullptr;

// half-broken emscripten shit, game doesn't work because browsers don't support all the shit I'm using:
// No flac audio support
// No PNG support?!?!??!! what the fuck (fixed, needed to preload ass ets)

// probably will try and fix this
// potential fixes:
// use mp3
// i don't think this error is true, because what the fuck

#ifdef WEB
void gameLoop() {
    game->render();
    if(!game->running()) {
        emscripten_cancel_main_loop();
    }
}
#endif

int main()
{

#ifdef WEB
    emscripten_set_main_loop(gameLoop, 60, true); // Main loop for Emscripten, don't believe it's just this easy.
#endif

    game = new Game();
    game->init("Deep Space Starships 0", 320, 200);

#ifndef WEB
    while(game->running())
    {
        game->render();
    }
#endif

    game->clean();
    return 0;
}
