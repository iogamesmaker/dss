#include "game.hpp"

ItemManager::ItemManager() {
    loadItems("sprite/items/items");
    money.push_back(0);
}

ItemManager::~ItemManager() {
    for (Item& item : items) {
        if (item.tex != nullptr) {
            SDL_DestroyTexture(item.tex);
        }
    }
    items.clear();
}

void ItemManager::loadItems(const std::string& path) {
    Item tempItem;
    tempItem.tex = nullptr;

    std::ifstream inputFile(path); // 50% chatGPT
    if (!inputFile.is_open()) {
        std::cout << "File " << path << " could not be opened." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.empty() || line[0] == '#') {
            continue; // thank you chatGPT for this function
        }

        std::istringstream lineStream(line);
        std::string name, texturePath;
        int id, stack;
        char comma;

        if (std::getline(lineStream, name, ',') && lineStream >> id >> comma && std::getline(lineStream, texturePath, ',') && lineStream >> stack) { // magic

            name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end()); // magic
            texturePath.erase(std::remove_if(texturePath.begin(), texturePath.end(), ::isspace), texturePath.end());

            tempItem.id = id;
            tempItem.name = name;
            tempItem.stack = stack;

            tempItem.tex = TM->returnTexture(texturePath.c_str());

            if (tempItem.tex == nullptr) {
                std::cout << "Failed to load texture: " << texturePath << std::endl;
                continue;
            }
            items.push_back(tempItem);
        }
        else {
            std::cout << "line \"" << line << "\" exploded." << std::endl;
        }
    }
    inputFile.close();
}
