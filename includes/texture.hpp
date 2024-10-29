#pragma once

class TextureManager
{
public:
    TextureManager(SDL_Renderer* mainRenderer);
    ~TextureManager();

    SDL_Rect customRectOffset(SDL_Rect rect, float offsetX, float offsetY);
    SDL_Rect rectOffset(int index, float offsetX, float offsetY);
    void updateRects();

    void unloadAllTextures();
    void unloadTexture(int index);
    void loadTexture(const char* path, int index);
    SDL_Texture* returnTexture(const char* path);

    void renderScaledTexture(SDL_Texture* texture, float scale, float offsetX, float offsetY);
    void renderCustom(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst);
    void renderLoadedTexture(int index, float offsetX, float offsetY);
    void renderTexture(SDL_Texture* texture, float offsetX, float offsetY);
    void renderRotatedScaledTexture(SDL_Texture* texture, float angle, float scale, float offsetX, float offsetY, SDL_Point center);

    SDL_Rect rotateRect(SDL_Rect rect, float angle);

    SDL_Texture* texture(int index);
    SDL_Rect rect(int index);

    SDL_Rect textureToRect(SDL_Texture* texture);
    SDL_Rect resizeRect(SDL_Rect rect, float scale);

    SDL_Rect scaleToScreen(SDL_Rect rect);

    SDL_Texture* returnRustTexture() {return rustTexture;}

    float aspectRatio() {return windowAspect;}

private:
    std::vector<SDL_Texture*> loadedTextures = {};
    std::vector<SDL_Rect> loadedRects = {};
    std::vector<SDL_Rect> renderRects = {};

    float windowAspect = 1.6;

    const float degToRad = 0.0174532925199;

    SDL_Renderer* renderer;
    SDL_Texture* rustTexture;
};

extern TextureManager *TM;
