#pragma once

class Stars
{
public:
     Stars();
    ~Stars();

    void renderStars(int offsetX, int offsetY);
    void updateStars();

    void generateNewStar(int offsetX, int offsetY, bool offscreen);

    void deleteStar(int index);
    void deleteAllStars();

    void regenerateStars();

    void printAllStars();

private:
    struct Star {
        int x;
        int y;
        float z;
        int type;
    };

    std::vector<Star> stars;
    std::vector<SDL_Texture*> starTexture;
    const float depth = 0.3;
    const int starAmount = 600;
    const int width = 340 * (1.5 + depth);
    const int height = 220 * (1.5 + depth);
};

extern Stars *SB;
