#include "game.hpp"

Stars::Stars()
{
    // load all star textures into my cool vector array
    starTexture.push_back(TM->returnTexture("assets/sprite/stars/star1.png"));
    starTexture.push_back(TM->returnTexture("assets/sprite/stars/star2.png"));
    starTexture.push_back(TM->returnTexture("assets/sprite/stars/star3.png"));
    starTexture.push_back(TM->returnTexture("assets/sprite/stars/star4.png"));
    starTexture.push_back(TM->returnTexture("assets/sprite/stars/star5.png"));
    starTexture.push_back(TM->returnTexture("assets/sprite/stars/star6.png"));
    starTexture.push_back(TM->returnTexture("assets/sprite/stars/star7.png"));
    starTexture.push_back(TM->returnTexture("assets/sprite/stars/star8.png"));
    starTexture.push_back(TM->returnTexture("assets/sprite/stars/star9.png"));
    starTexture.push_back(TM->returnTexture("assets/sprite/stars/star10.png"));
    starTexture.push_back(TM->returnTexture("assets/sprite/stars/star11.png"));

    regenerateStars(); // generate all stars
}

Stars::~Stars() {
    deleteAllStars(); // delete all stars, yucky
}

void Stars::generateNewStar(int offsetX, int offsetY, bool offscreen)
{
    int starX;
    int starY;

    float starZ = RE->random(0.002, depth);

    do { // reaptedly brute force generate a star until it's in bounds
        starX = RE->randomint(-width, width) + offsetX;
        starY = RE->randomint(-height, height) + offsetY;
    } while (offscreen && abs(starX - offsetX * starZ) <= 160 && abs(starY - offsetY * starZ) <= 100);

    // advanced C++ shit B) ^

    int starType = RE->randomint(0, 11); // random star type, only affects texture at the moment.
    stars.push_back({starX, starY, starZ, starType}); // push the star to the vector
}

void Stars::renderStars(int offsetX, int offsetY)
{
    for (int i = stars.size() - 1; i != -1; i--) {
        int starX = stars[i].x + offsetX * stars[i].z;
        int starY = stars[i].y + offsetY * stars[i].z;

        if(game->State() == 0) {
            if (abs(starX) <= 160 && abs(starY) <= 100) {
                TM->renderTexture(starTexture[stars[i].type], starX, starY);
            }
        } else if ((abs(starX) * game->zoomVal() <= 160 && abs(starY) * game->zoomVal() <= 100)) {
            TM->renderScaledTexture(starTexture[stars[i].type], game->zoomVal(), starX, starY);
        }

        if (starX >= width || starX <= -width || starY >= height || starY <= -height) {
            generateNewStar(-offsetX * stars[i].z, -offsetY * stars[i].z, true);
            stars.erase(stars.begin() + i);
        }
    }
}

void Stars::regenerateStars()
{
    stars.clear();

    for(int i = 0; i < starAmount; i++) {
        generateNewStar(0, 0, false);
    }
}

void Stars::deleteAllStars()
{
    stars.clear();
}
