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
// No PNG support?!?!??!! what the fuck
// SDL not built with haptic force?????????????????????? okay

// probably will try and fix this
// potential fixes:
// use mp3
// i don't think this error is true, because what the fuck
// disable haptic force shit, why does SDL2 have that anyway i'm not gonna make a mobile app wtf

#ifdef WEB
void gameLoop() {
    game->render();
}
#endif

int main()
{
    game = new Game();
    game->init("Deep Space Starships 0", 320, 200);

#ifdef WEB
    emscripten_set_main_loop(gameLoop, 0, 1); // more emscripten shit
#else

    while(game->running())
    {
        game->render(); // main thread is only the render function because multithreading is cool
    }

#endif

    game->clean();
    return 0;
}
