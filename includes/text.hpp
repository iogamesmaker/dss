#pragma once

class TextEngine
{
public:
    TextEngine();
    ~TextEngine();

    void loadFont(const char* path, int index, int fontW, int fontH, std::string fontLayout);
    void unloadFont(int index);
    void unloadAllFonts();

    void renderCharacter(const char* character, int x, int y, SDL_Color color, int index, bool shadow);
    void renderText(const std::string& text, int x, int y, SDL_Color color, int index);

private:
    SDL_Renderer* renderer;
    std::vector<SDL_Texture*> loadedFonts;
    std::vector<SDL_Rect> fontRects;
    std::vector<std::string> fontLayouts;
};

extern TextEngine *TE;
