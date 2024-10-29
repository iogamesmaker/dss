#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <math.h>
#include <cmath>
#include <random>
#include <algorithm>

#include <string>
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <cctype>
#include <thread>
#include <atomic>

#ifdef WEB
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_render.h>

#ifdef WEB
#include <emscripten.h>
#endif

#include "texture.hpp"
#include "sound.hpp"
#include "stars.hpp"
#include "text.hpp"
#include "random.hpp"
#include "gui.hpp"
#include "ship.hpp"
#include "item.hpp"

class Game
{
public:
    Game();
    ~Game();

    void init(const char *title, int width, int height);
    void handleEvents();
    void update();
    void render();
    void clean();
    // --
    void switchToNewMenu();
    void switchToMainMenu();
    void switchToGame();
    // --

    void stop() {isRunning = false;}

    // get functions

    bool running() {return isRunning;}
    int windowWidth() {return wWidth;}
    int windowHeight() {return wHeight;}
    int mX() {return mouseX;}
    int mY() {return mouseY;}

    float scaleFactor() {return((windowAspect > 1.6) ? static_cast<float>(wHeight) / 200.0f : static_cast<float>(wWidth) / 320.0f);}

    int scaledmX() {
        return (mouseX - wWidth  / 2) * scaleFactor();
    }
    int scaledmY() {
        return (mouseY - wHeight / 2) * scaleFactor();
    }
    bool lmb() {return mousePressed;}
    bool rmb() {return rightMousePressed;}
    bool plmb() {return prevMousePressed;}
    bool prmb() {return prevRightMousePressed;}
    bool debug() {return debugBool;}

    float returnDeltaTime() {return deltaTime;}

    float aspectRatio() {return windowAspect;}

    int State() {return state;}

    float zoomVal() {return zoom;}

    bool keyPressed(SDL_Keycode key) {return keyStates[key];}
    void releaseKey(SDL_Keycode key) {keyStates[key] = false;}

    SDL_Renderer* returnRenderer() {return renderer;}


private:
    float mouseX = 0, mouseY = 0;
    int wHeight = 200, wWidth = 320;
    float windowAspect = 1.6;

    bool isRunning = true;
    bool mousePressed = false;
    bool rightMousePressed = false;
    bool prevMousePressed = false;
    bool prevRightMousePressed = false;
    bool debugBool = false;

    int state = 0;

    float zoom = 1;
    float smoothScroll = 0;

    std::atomic<bool> eventThreadRunning;
    std::thread eventThread;

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Texture* cursor;

    std::unordered_map<int, bool> keyStates; // All key states - handy input system

    float deltaTime = 0.0f;
    Uint64 lastFrameTime = 0;
    Uint64 currentFrameTime = 0;
};

extern Game *game;
