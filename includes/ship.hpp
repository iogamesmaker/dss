#pragma once

class Ships
{
public:
    Ships();
    ~Ships();

    void loadShip(const std::string path);

    void createShip();

    void updateMyShip();

    void renderShip(int index);

    void addShipVel(int index, float x, float y) {
        list[index].xVel += x;
        list[index].yVel += y;
    }
    glm::vec2 getCoordinates(int index) {
        return glm::vec2(list[index].x, list[index].y);
    }

private:
    enum {
        LEFT = 0,
        UP = 90,
        RIGHT = 180,
        DOWN = 270
    };
    struct Cannon
    {
        int x;
        int y;
        float dir;

        int maxAngle;
    };

    struct Thruster
    {
        int x;
        int y;
        int dir;
        int strength;
    };

    struct Ship
    {
        std::vector<Cannon> cannons;
        std::vector<Thruster> thrusters;

        std::string name;

        SDL_Texture* tex;
        int x;
        int y;

        int HP;
    };

    std::vector<Ship> ships;

    struct LoadedShip
    {
        int index; // ship type from ships vector

        float x, y;
        std::string name;
        int hp;
        int px, py;

        float xVel = 0, yVel = 0;

        std::vector<float> cannonDir;
        std::vector<float> thrusterDir;
    };

    std::vector<LoadedShip> list;

    SDL_Texture* cannonTexture;
    SDL_Texture* thrusterTexture;
};
extern Ships *SH;
