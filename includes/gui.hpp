#pragma once

class GUIManager
{
public:
    GUIManager();
    ~GUIManager();

    void addPopup(SDL_Rect popup, int index);
    void renderCustomPopup(SDL_Rect popup, int type);

    void addText(const std::string& string, int x, int y, int font, SDL_Color color, int index);

    void renderInventory(int invSlots);

    bool isHovered(SDL_Rect rect);

    void addButton(SDL_Texture* button, SDL_Texture* selected, int x, int y, std::function<void()> function, int index);

    void renderElements();

    void unloadElement(int index);
    void unloadAllElements();
private:
    SDL_Renderer* renderer;
    SDL_Texture* popuptexture;

    std::vector<SDL_Rect> popups;

    struct Button {
        SDL_Texture* a;
        SDL_Texture* b;
        SDL_Rect rect;
        std::function<void()> func;
    };
    std::vector<Button> buttons;

    struct Text {
        std::string text;
        int x;
        int y;
        SDL_Color color;
        int font;
    };

    std::vector<Text> text;

    enum Type {
        BUTTON,
        POPUP,
        TEXT
    };
    struct Element {
        int index;
        int type;
    };
    std::vector<Element> elements;
};

extern GUIManager *GUI;
