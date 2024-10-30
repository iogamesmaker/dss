#include "game.hpp"

TextureManager::TextureManager(SDL_Renderer* mainRenderer)
{
    renderer = mainRenderer;

    rustTexture = returnTexture("assets/sprite/rust.png");
}
TextureManager::~TextureManager(){unloadAllTextures();}

void TextureManager::updateRects() {
    windowAspect = static_cast<float>(game->windowWidth()) / game->windowHeight();

    for (int i = 0; i < loadedRects.size(); i++) {
        renderRects[i] = loadedRects[i];

        float scaleFactor = (windowAspect > 1.6f)
                            ? static_cast<float>(game->windowHeight()) / 200.0f
                            : static_cast<float>(game->windowWidth ()) / 320.0f;

        renderRects[i].w = loadedRects[i].w * scaleFactor;
        renderRects[i].h = loadedRects[i].h * scaleFactor;
    }
}

SDL_Rect TextureManager::customRectOffset(SDL_Rect rect, float offsetX, float offsetY) {
    SDL_Rect temprect = rect;

    if (windowAspect > 1.6f) {
        temprect.x = (game->windowWidth() - temprect.w) / 2;
        temprect.y = (game->windowHeight() - temprect.h) / 2;
        temprect.x += (offsetX / 320.0f) * (game->windowHeight() * 1.6f);
        temprect.y += (offsetY / 200.0f) * game->windowHeight();
    } else {
        temprect.x = (game->windowWidth() - temprect.w) / 2;
        temprect.y = (game->windowHeight() - temprect.h) / 2;
        temprect.x += (offsetX / 320.0f) * game->windowWidth();
        temprect.y += (offsetY / 200.0f) * (game->windowWidth() / 1.6f);
    }
    return temprect;
}

SDL_Rect TextureManager::rectOffset(int index, float offsetX, float offsetY) {
    SDL_Rect temprect = renderRects[index];

    if (windowAspect > 1.6f) {
        temprect.x = (game->windowWidth() - temprect.w) / 2;
        temprect.y = (game->windowHeight() - temprect.h) / 2;
        temprect.x += (offsetX / 320.0f) * (game->windowHeight() * 1.6f);
        temprect.y += (offsetY / 200.0f) * game->windowHeight();
    } else {
        temprect.x = (game->windowWidth() - temprect.w) / 2;
        temprect.y = (game->windowHeight() - temprect.h) / 2;
        temprect.x += (offsetX / 320.0f) * game->windowWidth();
        temprect.y += (offsetY / 200.0f) * (game->windowWidth() / 1.6f);
    }
    return temprect;
}

void TextureManager::loadTexture(const char* path, int index)
{
    SDL_Surface* tempSurface = IMG_Load(path);

    if(tempSurface == NULL) {std::cout << "texture didn't load ): (try running the executable from the install folder): " << SDL_GetError() << std::endl;}
    SDL_Texture* temptexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_Rect temprect = {0, 0, 0, 0};
    if(temptexture) {
        if (index >= loadedTextures.size()) {
            loadedTextures.resize(index + 1);
            loadedRects.resize(index + 1);
            renderRects.resize(index + 1);
        }

        loadedTextures[index] = temptexture;
        renderRects[index] = temprect;

        int tw, th;
        SDL_QueryTexture(temptexture, NULL, NULL, &tw, &th);
        temprect.w = tw;
        temprect.h = th;
        loadedRects[index] = temprect;
    }   else {std::cout << "texture loading said kaboom: " << SDL_GetError() << std::endl;}
}

SDL_Texture* TextureManager::returnTexture(const char* path)
{
    SDL_Surface* tempSurface = IMG_Load(path);

    if(tempSurface == NULL) {std::cout << "texture didn't load/return ): (try running the executable from the install folder): " << SDL_GetError() << std::endl;}
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    if(!texture) {std::cout << "texture loading said kaboom: " << SDL_GetError() << std::endl;}

    return texture;
}

void TextureManager::renderLoadedTexture(int index, float offsetX, float offsetY) {
    if(index > loadedTextures.size()) return;
    SDL_Rect rect = rectOffset(index, offsetX, offsetY);
    SDL_RenderCopy(renderer, loadedTextures[index], NULL, &rect);
}

void TextureManager::renderTexture(SDL_Texture* texture, float offsetX, float offsetY) {
    int tw, th;
    SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
    SDL_Rect rect = customRectOffset(scaleToScreen({0, 0, tw, th}), offsetX, offsetY);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void TextureManager::renderCustom(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst) {
    SDL_RenderCopy(renderer, texture, &src, &dst);
}

void TextureManager::renderScaledTexture(SDL_Texture* texture, float scale, float offsetX, float offsetY)
{
    SDL_Rect rect = textureToRect(texture);

    renderCustom(texture, rect, customRectOffset(resizeRect(scaleToScreen(rect), scale), offsetX * scale, offsetY * scale));
}

void TextureManager::renderRotatedScaledTexture(SDL_Texture* texture, float angle, float scale, float offsetX, float offsetY, SDL_Point center) {
    SDL_Rect srcRect = textureToRect(texture);
    SDL_Rect dstRect = customRectOffset(resizeRect(scaleToScreen(srcRect), scale), offsetX * scale, offsetY * scale);

    float scaleFactor = (windowAspect > 1.6f)
                    ? static_cast<float>(game->windowHeight()) / 200.0f
                    : static_cast<float>(game->windowWidth ()) / 320.0f;

    center.x *= scale;
    center.y *= scale;
    center.x *= scaleFactor;
    center.y *= scaleFactor;

    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, angle, &center, SDL_FLIP_NONE);
}

SDL_Rect TextureManager::scaleToScreen(SDL_Rect rect)
{
    float scaleFactor = (windowAspect > 1.6)
                        ? static_cast<float>(game->windowHeight()) / 200.0f
                        : static_cast<float>(game->windowWidth ()) / 320.0f;

    rect.w = rect.w * scaleFactor;
    rect.h = rect.h * scaleFactor;
    return rect;
}

SDL_Texture* TextureManager::texture(int index)
{
    if(index > loadedTextures.size()) {std::cout << "damn that texture dont exist bro: (loadedTextures[ " << index << "])\n";}
    return loadedTextures[index];
}

SDL_Rect TextureManager::rect(int index)
{
    if(index > loadedRects.size()) {std::cout << "loadedRects[" << index << "] doesn't exist.";}
    return loadedRects[index];
}

SDL_Rect TextureManager::textureToRect(SDL_Texture* texture)
{
    int tw, th;
    SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
    return SDL_Rect{0, 0, tw, th};
}

SDL_Rect TextureManager::resizeRect(SDL_Rect rect, float scale)
{
    rect.w *= scale;
    rect.h *= scale;

    return rect;
}

void TextureManager::unloadAllTextures() // might go and explode
{
    for(int i; i < loadedTextures.size(); i++)
    {
        unloadTexture(i);
    }
}

void TextureManager::unloadTexture(int index)
{
    if(index == loadedTextures.size())
    {
        SDL_DestroyTexture(loadedTextures[index]);
        loadedTextures.pop_back();
        loadedRects.pop_back();
        renderRects.pop_back();
    }
    else if (index < loadedTextures.size()) {
        SDL_DestroyTexture(loadedTextures[index]);
        loadedTextures[index] = NULL;
        loadedRects[index] = SDL_Rect{0, 0, 0, 0};
        renderRects[index] = SDL_Rect{0, 0, 0, 0};
    }
}

