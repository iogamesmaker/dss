em++ ./src/*.cpp \
    -I./includes \
    -s USE_SDL=2 \
    -s USE_SDL_IMAGE=2 \
    -s SDL2_IMAGE_FORMATS='["png","jpeg"]' \
    -s USE_SDL_MIXER=2 \
    -s SDL2_MIXER_FORMATS='["flac", "mp3"]' \
    -s NO_EMSCRIPTEN_TRACING=1 \
    --preload-file assets \
    -o index.html
