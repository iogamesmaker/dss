#include "game.hpp"

Ships::Ships()
{
    loadShip("ships/default");
    cannonTexture = TM->returnTexture("ships/cannon.png");
    thrusterTexture = TM->returnTexture("ships/thruster.png");
}
Ships::~Ships(){}

void Ships::loadShip(const std::string path)
{
    Ship tempShip;
    tempShip.tex = nullptr;

    std::string file;
    std::ifstream inputFile(path);

    if (!inputFile.is_open())
    {
        std::cout << "file " << path << " didn't wanna be opened";
        return;
    }

    enum {
        PATH,
        CANNON,
        THRUSTER,
        HP
    };

    std::string line;

    int reader;

    while (getline(inputFile, line)) {

        std::string casedLine = line;

        std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c){ return std::toupper(c); });


        if (line[0] == '#' || line == "")
        {
            continue;
        }
        else if (line == "CANNON") {
            reader = CANNON;
        }
        else if (line == "THRUSTER")
        {
            reader = THRUSTER;
        }
        else if (line.substr(0, 3) == "HP=") {
            tempShip.HP = std::stoi(line.substr(3));
        }
        else if (line.substr(0, 4) == "TEX=")
        {
            if(tempShip.tex == nullptr)
            {
                tempShip.tex = TM->returnTexture(casedLine.substr(4).c_str());
            }
        }
        else if (line.substr(0, 5) == "NAME=")
        {
            tempShip.name = casedLine.substr(5);
            std::cout << "Loaded " << casedLine.substr(5) << "." << std::endl;
        }
        else {
            std::istringstream ss(line);
            if (reader == CANNON) {
                Cannon cannon;
                char comma;
                ss >> cannon.x >> comma >> cannon.y >> comma >> cannon.dir >> comma >> cannon.maxAngle;

                cannon.y = -cannon.y;

                tempShip.cannons.push_back(cannon);
            }
            else if (reader == THRUSTER) {
                Thruster thruster;
                char comma;
                ss >> thruster.x >> comma >> thruster.y >> comma >> thruster.dir >> comma >> thruster.strength;

                tempShip.thrusters.push_back(thruster);
            }
        }
    }

    ships.push_back(tempShip);
}

void Ships::createShip()
{
    LoadedShip ship;
    ship.x = 0;
    ship.y = 0;
    ship.index = 0;
    ship.name = "Wolfheze";
    ship.hp = 100;
    ship.px = 100;
    ship.py = 100;
    ship.xVel = 0;
    ship.yVel = 0;

    ship.cannonDir.push_back(0);
    ship.thrusterDir.push_back(0);

    list.push_back(ship);
}

void Ships::updateMyShip()
{
    list[0].x += list[0].xVel;
    list[0].y += list[0].yVel;

    list[0].xVel *= 0.993;
    list[0].yVel *= 0.993;

    Ship& ship = ships[list[0].index];
    float scaleFactor = (game->aspectRatio() > 1.6)
        ? static_cast<float>(game->windowHeight()) / 200.0f
        : static_cast<float>(game->windowWidth()) / 320.0f;

    for(int i = 0; i < ship.cannons.size(); i++)
    {
        // 1366x768:
        // scaleFactor = 3.84
        // 1366 / 3.84 = 355 - 320 = 35.729166667
        // 768 / 3.84 = 200 - 200 = 0
        //
        // Border is 35.72916667 game pixels on the sides
        // Border is 137.2 screen pixels
        // Each side half of that.


        glm::vec2 mouse = glm::vec2(
            game->mX(),
            game->mY()
        );

        glm::vec2 cannon(
            roundf(ship.cannons[i].x * game->zoomVal() * scaleFactor),
            roundf(ship.cannons[i].y * game->zoomVal() * scaleFactor)
        );

        float angle;
        if(game->aspectRatio() < 1.6) // borders on left right
        {
            // std::cout <<
            angle = 270 - glm::degrees(
                atan2(
                    cannon.x - (mouse.x - game->windowWidth() / 2) * (1.6 / game->aspectRatio()),
                    cannon.y - (mouse.y - game->windowHeight() / 2)
                )
            );
        } else {
            angle = 270 - glm::degrees(
                atan2(
                    (cannon.x - (mouse.x - game->windowWidth()  / 2)),
                    (cannon.y - (mouse.y - game->windowHeight() / 2) * (1.6 / game->aspectRatio()))
                )
            );
        }


        if(angle >= 360) angle -= 360;
        if(angle <= 0) angle += 360;

        // if (std::abs(ship.cannons[i].dir - angle) < ship.cannons[i].maxAngle || (std::abs(-ship.cannons[i].dir - angle) < ship.cannons[i].maxAngle && abs(ship.cannons[i].dir) + ship.cannons[i].maxAngle / 2 > 180)) {
            list[0].cannonDir[i] = angle;
        // }
    }
}


void Ships::renderShip(int index)
{
    float scaleFactor = (game->aspectRatio() > 1.6)
        ? static_cast<float>(game->windowHeight()) / 200.0f
        : static_cast<float>(game->windowWidth()) / 320.0f;

    TM->renderScaledTexture(ships[list[index].index].tex, game->zoomVal(), 0, 0);

    for (int i = 0; i < ships[list[index].index].cannons.size(); i++) {
        float cannonX = ships[list[index].index].cannons[i].x;
        float cannonY = ships[list[index].index].cannons[i].y;

        TM->renderRotatedScaledTexture(
            cannonTexture,
            list[index].cannonDir[i],
            game->zoomVal(),
            cannonX + 2,
            cannonY,
            SDL_Point{0, 0}
        );
    }

    for(int i = 0; i < ships[list[index].index].thrusters.size(); i++)
    {
        int angle = glm::degrees(atan2(-list[index].xVel, list[index].yVel));

        if(list[index].thrusterDir[i] < angle) {
            list[index].thrusterDir[i] += 0.02 * (list[index].thrusterDir[i] + angle);
        }
        if(list[index].thrusterDir[i] > angle) {
            list[index].thrusterDir[i] += 0.02 * (list[index].thrusterDir[i] - angle);
        }

        if(list[index].thrusterDir[i] >= 360) list[index].thrusterDir[i] = 0;
        if(list[index].thrusterDir[i] <= 0) list[index].thrusterDir[i] = 360;

        TM->renderRotatedScaledTexture(thrusterTexture,
        list[index].thrusterDir[i],
        game->zoomVal(), ships[list[index].index].thrusters[i].x, ships[list[index].index].thrusters[i].y, SDL_Point{1, 1});
    }
}
