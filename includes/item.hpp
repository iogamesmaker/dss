#pragma once

class ItemManager
{
public:
    ItemManager();
    ~ItemManager();
    void loadItems(const std::string& path);

    int getMoney(int index) {return money[index];}
    bool increaseMoney(int index, int amount) {if(money[index] - amount >= 0) {money[index] -= amount; return true;} else {return false;}}
private:
    struct Item {
        int id = -1;
        std::string name = "item";
        SDL_Texture* tex = nullptr;
        int stack = 1;
    };

    std::vector<Item> items;
    std::vector<int> money;
};

extern ItemManager *IM;
