#include "game.hpp"

TextEngine::TextEngine() {}

TextEngine::~TextEngine() {
    unloadAllFonts();
}

void TextEngine::loadFont(const char* path, int index, int fontW, int fontH, std::string fontLayout)
{
    if(index >= loadedFonts.size()) {
        loadedFonts.resize(index + 1);
        fontRects.resize(index + 1);
        fontLayouts.resize(index + 1);
    }

    loadedFonts[index] = TM->returnTexture(path);

    SDL_Rect temprect;

    SDL_QueryTexture(loadedFonts[index], NULL, NULL, &temprect.x, &temprect.y);

    temprect.w = fontW;
    temprect.h = fontH;

    fontRects[index] = temprect;
    fontLayouts[index] = fontLayout;
}

void TextEngine::renderCharacter(const char* character, int x, int y, SDL_Color color, int index, bool shadow)
{
    // Find the character's position in the font layout
    std::string fontLayout = fontLayouts[index];
    int charPos = fontLayout.find(character);

    int charsPerRow = loadedFonts[index] ? (fontRects[index].x / fontRects[index].w) : 0;

    SDL_Rect srcRect;
    if (charPos == std::string::npos) {
        srcRect.x = fontRects[index].x - fontRects[index].w;
        srcRect.y = fontRects[index].y - fontRects[index].h;
        std::cout << "Character " << fontLayout[charPos] << "missing.";
    } else {
        srcRect.x = charPos % charsPerRow * fontRects[index].w;
        srcRect.y = charPos / charsPerRow * fontRects[index].h;
    }
    srcRect.w = fontRects[index].w;
    srcRect.h = fontRects[index].h;


    SDL_Rect dstRect = TM->scaleToScreen(TM->customRectOffset(srcRect, x, y));

    if(shadow) {dstRect.w *= 1.10;}

    if(shadow) {
        SDL_SetTextureColorMod(loadedFonts[index], color.r * 0.8, color.g * 0.8, color.b * 0.8);
    } else {
        SDL_SetTextureColorMod(loadedFonts[index], color.r, color.g, color.b);
    }

    TM->renderCustom(loadedFonts[index], srcRect, dstRect);
}

void TextEngine::renderText(const std::string& text, int x, int y, SDL_Color color, int index)
{
    int offsetX = x;
    int offsetY = y;

        for (char character : text) {
            std::string str(1, character);
            renderCharacter(str.c_str(), offsetX - 1, offsetY,SDL_Color{static_cast<Uint8>(0.6f * color.r), static_cast<Uint8>(0.6f * color.g), static_cast<Uint8>(0.6f * color.b)}, index, true);

            renderCharacter(str.c_str(), offsetX, offsetY, color, index, false);

            offsetX += fontRects[index].w + 1;
            if (offsetX >= 160 - fontRects[index].w) {offsetY += fontRects[index].h; offsetX = x;}
        }

}


void TextEngine::unloadFont(int index)
{
    SDL_DestroyTexture(loadedFonts[index]);
    loadedFonts[index] = NULL;
    fontRects[index] = SDL_Rect{0, 0, 0, 0};
}

void TextEngine::unloadAllFonts()
{
    for(int i; i < loadedFonts.size(); i++)
    {
        unloadFont(i);
    }
}
