#include "game.hpp"

 // all is instanced except for the inventory


GUIManager::GUIManager() {
    popuptexture = TM->returnTexture("assets/sprite/menu/popup.png");
}

GUIManager::~GUIManager() {
    SDL_DestroyTexture(popuptexture);
    unloadAllElements();
}

void GUIManager::renderInventory(int invSlots) {
    SDL_Rect rect;

    int columns = static_cast<int>(std::ceil(std::sqrt(invSlots))); // what the dogshit
    int rows = (invSlots + columns - 1) / columns;

    rect.x = -154; // background rect
    rect.y = -95;
    rect.w = 25 * columns + 11;
    rect.h = 25 * rows + 26;

    renderCustomPopup(rect, 0);

    // TE->renderText( // text
    //     "Inventory",
    //     -146, -89,
    //     SDL_Color{99, 155, 255},
    //     1
    // );

    // TE->renderText( // money display, named dogshit
    //     "dogshit: " + std::to_string(IM->getMoney(0)),
    //     -146, -80,
    //     SDL_Color{99, 155, 255},
    //     0
    // );

    rect.x = -147; // first slot
    rect.y = -72;
    rect.w = 23;
    rect.h = 23;

    for(int i = 0; i < invSlots; i++) { // render slots and shit
        renderCustomPopup(rect, 0 + (isHovered(TM->scaleToScreen(rect))));
        rect.x += 25;

        if ((i + 1) % columns == 0) { // move to the next column
            rect.x = -147;
            rect.y += 25;
        }
    }
}


void GUIManager::addText(const std::string& string, int x, int y, int font, SDL_Color color, int index)
{
    if(index >= elements.size()) {
        elements.resize(index + 1);
    }
    Text newText;
    newText.text = string;
    newText.x = x;
    newText.y = y;
    newText.font = font;
    newText.color = color;

    elements[index].index = text.size();
    text.push_back(newText);
    elements[index].type = TEXT;
}

void GUIManager::addPopup(SDL_Rect popup, int index) {
    if(index >= elements.size()) {
        elements.resize(index + 1);
    }
    elements[index].index = popups.size();
    popups.push_back(popup);
    elements[index].type = POPUP;
}

void GUIManager::unloadElement(int index)
{
    if(elements[index].type == POPUP)
    {
        if(elements[index].index == popups.size()) {
            popups.pop_back();
        }
        else if(elements[index].index < popups.size())
        {
            popups.erase(popups.begin() + elements[index].index);
        }
    }
    else if(elements[index].type == BUTTON)
    {
        if (elements[index].index < buttons.size()) {
            SDL_DestroyTexture(buttons[elements[index].index].a);
            SDL_DestroyTexture(buttons[elements[index].index].b);

            if(elements[index].index == buttons.size())
            {
                buttons.pop_back();
            } else {
                buttons.erase(buttons.begin() + elements[index].index);
            }
        }
    }
    else if(elements[index].type == TEXT)
    {
        if(elements[index].index == text.size()) {
            text.pop_back();
        }
        else if(elements[index].index < text.size())
        {
            text.erase(text.begin() + elements[index].index);
        }
    }
    if(index == elements.size()) {
        elements.pop_back();
    }
    else if(index < elements.size())
    {
        elements.erase(elements.begin() + index);
    }
}

void GUIManager::unloadAllElements()
{
    for(int i; i < elements.size(); i++)
    {
        unloadElement(i);
    }
    buttons.clear();
    popups.clear();
    text.clear();
    elements.clear();
}

void GUIManager::renderElements()
{
    for(int index = 0; index < elements.size(); index++)
    {
        if(elements[index].type == BUTTON)
        {
            SDL_Rect rect = TM->scaleToScreen(buttons[elements[index].index].rect);

            SDL_Rect dstRect = TM->customRectOffset(rect, rect.x, rect.y);
            if(!isHovered(rect))
            {
                TM->renderTexture(buttons[elements[index].index].a, rect.x, rect.y);
            }
            else
            {
                TM->renderTexture(buttons[elements[index].index].b, rect.x, rect.y);
                if(game->lmb() && !game->plmb() && buttons[elements[index].index].func != nullptr) {
                    buttons[elements[index].index].func();
                }
            }
        }

        else if(elements[index].type == POPUP)
        {
            renderCustomPopup(popups[elements[index].index], 0);
        }

        else if(elements[index].type == TEXT)
        {
            TE->renderText(
                text[elements[index].index].text,
                text[elements[index].index].x,
                text[elements[index].index].y,
                text[elements[index].index].color,
                text[elements[index].index].font
            );
        }
    }
}

void GUIManager::renderCustomPopup(SDL_Rect popup, int type) // Next step: fix popup placement, the inventory is NOT supposed to be at -98, 0, should be more at top left corner. Should fix the scaled popup rust as well.
{
    float scaleFactor = (static_cast<float>(game->windowWidth()) / static_cast<float>(game->windowHeight()) > 1.6f)
                        ? static_cast<float>(game->windowHeight()) / 200.0f
                        : static_cast<float>(game->windowWidth ()) / 320.0f;
    // if(popup.x <= -160 + popup.w / 2) {popup.x = -160 + popup.w / 2;}
    // if(popup.y <= -100 + popup.h / 2) {popup.y = -100 + popup.h / 2;}
    // if(popup.x >=  160 - popup.w / 2) {popup.x =  160 - popup.w / 2;}
    // if(popup.y >=  100 - popup.h / 2) {popup.y =  100 - popup.h / 2;}

    popup.x += popup.w / 2;
    popup.y += popup.h / 2;

    SDL_Rect oldPopup = popup;

    popup = TM->scaleToScreen(popup);

    if(oldPopup.x < 0)   {oldPopup.x = abs(oldPopup.x);} // temporary fix, works kind of
    if(oldPopup.y < 0)   {oldPopup.y = abs(oldPopup.y);}
    if(oldPopup.x > 320) {oldPopup.x -= oldPopup.x - 320;}
    if(oldPopup.y > 200) {oldPopup.y -= oldPopup.y - 200;}

    popup = TM->customRectOffset(popup, popup.x, popup.y);

    // std::cout << popup.x << ", " << popup.y << ", " << oldPopup.w << ", " << oldPopup.h << std::endl;

    TM->renderCustom(TM->returnRustTexture(), oldPopup, popup);

    SDL_Rect tempRect = SDL_Rect{static_cast<int>(popup.x + 2 * scaleFactor), static_cast<int>(popup.y + 2 * scaleFactor), popup.w, popup.h};

    popup = {tempRect.x, tempRect.y, static_cast<int>(tempRect.w - 4 * scaleFactor), static_cast<int>(tempRect.h - 4 * scaleFactor)};

    TM->renderCustom(popuptexture, SDL_Rect{2, 0, 1, 1}, popup);
}

bool GUIManager::isHovered(SDL_Rect rect)
{
    rect = TM->customRectOffset(rect, rect.x, rect.y);

    if(game->mX() >= rect.x && game->mX() <= rect.x + rect.w && game->mY() >= rect.y && game->mY() <= rect.y + rect.h) {
        return true;
    } else {
        return false;
    }
}

void GUIManager::addButton(SDL_Texture* button, SDL_Texture* selected, int x, int y, std::function<void()> function, int index)
{
    if(index >= elements.size()) {
        elements.resize(index + 1);
    }

    Button tempButton;
    tempButton.a = button;
    tempButton.b = selected;
    int tw, th;
    SDL_QueryTexture(button, nullptr, nullptr, &tw, &th);
    tempButton.rect = SDL_Rect{x, y, tw, th};
    tempButton.func = function;

    elements[index].index = buttons.size();
    buttons.push_back(tempButton);
    elements[index].type = BUTTON;
}
