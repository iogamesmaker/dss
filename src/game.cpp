#include "game.hpp"

Game::Game(){}

Game::~Game()
{
}

void Game::init(const char* title, int width, int height)
{
    std::cout << "I like forest shitgnomes" << std::endl;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {std::cout << "sdl init exploded: " << SDL_GetError() << std::endl;}

#ifdef WEB
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(!window) {std::cout << "making window didnt work: " << SDL_GetError() << std::endl;}

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(!renderer) {std::cout << "renderer didn't initialise: " << SDL_GetError() << std::endl;}


    SDL_SetWindowMinimumSize(window, 320, 200);
#ifdef WEB
    int width, height;
    emscripten_get_screen_size(&width, &height);

    SDL_SetWindowMaximumSize(window, width, height);
    SDL_SetWindowSize(window, width, height);
#else
    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {std::cout << "display mode exploded: " << SDL_GetError() << std::endl;}

    SDL_SetWindowMaximumSize(window, displayMode.w, displayMode.h);
    SDL_SetWindowSize(window, displayMode.w, displayMode.h);
#endif

    SDL_ShowCursor(SDL_DISABLE);

    RE = new RandomEngine();
    TM = new TextureManager(renderer);
    SM = new SoundManager();
    SB = new Stars();
    TE = new TextEngine();
    GUI= new GUIManager();
    SH = new Ships();
    IM = new ItemManager();

    eventThreadRunning = true;
    eventThread = std::thread(&Game::handleEvents, this);

    state = 0;

    // TE->loadFont("assets/fonts/small.png", 0, 3, 5, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!?@#$()[]<>={}+*%/\\&-_'\":;,.`~^ ");
    // TE->loadFont("assets/fonts/default.png", 1, 6, 8, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!?@#$()[]<>={}+*%/\\&-_'\":;,.`~^ ");
    cursor = TM->returnTexture("assets/sprite/cursor.png");

    TM->unloadAllTextures();


    TM->loadTexture("assets/sprite/menu/splash.png", 0);

    TM->loadTexture("assets/sprite/menu/starship.png", 1);
    TM->loadTexture("assets/sprite/menu/starships.png", 2);

    TM->updateRects();

    SM->loadSound("assets/sound/button.wav", 0);
    SM->loadSound("assets/sound/start.wav", 1);
    SM->loadMusic("assets/sound/music/menu.mp3", 0);

    SM->playMusic(0, true);
    SM->playSound(1);

    lastFrameTime = SDL_GetPerformanceCounter();

    switchToMainMenu();
}

void Game::handleEvents()
{
    while (eventThreadRunning) {
        currentFrameTime = SDL_GetPerformanceCounter();
        deltaTime = static_cast<float>(currentFrameTime - lastFrameTime) / SDL_GetPerformanceFrequency();
        lastFrameTime = currentFrameTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }

            if (event.type == SDL_KEYDOWN) {
                keyStates[event.key.keysym.sym] = true;
            }

            if (event.type == SDL_KEYUP) {
                keyStates[event.key.keysym.sym] = false;
            }

            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    wWidth = event.window.data1;
                    wHeight = event.window.data2;
                    windowAspect = static_cast<float>(wWidth) / wHeight;
                    TM->updateRects();
                }
            }

            if (event.type == SDL_MOUSEMOTION) {
                mouseX = event.motion.x;
                mouseY = event.motion.y;
            }

            if (event.type == SDL_MOUSEWHEEL) {
                smoothScroll += event.wheel.y * 200 * deltaTime;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    prevMousePressed = mousePressed;
                    mousePressed = true;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    prevRightMousePressed = rightMousePressed;
                    rightMousePressed = true;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    prevMousePressed = mousePressed;
                    mousePressed = false;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    prevRightMousePressed = rightMousePressed;
                    rightMousePressed = false;
                }
            }
        }

        game->update();

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Small sleep to avoid busy waiting
    }
}

void Game::update()
{
    if(state != 0) {
        if(keyPressed(SDLK_w)) {
            SH->addShipVel(0, 0, 0.5 * deltaTime);
        }
        if(keyPressed(SDLK_a)) {
            SH->addShipVel(0, 0.5 * deltaTime, 0);
        }
        if(keyPressed(SDLK_s)) {
            SH->addShipVel(0, 0, -0.5 * deltaTime);
        }
        if(keyPressed(SDLK_d)) {
            SH->addShipVel(0, -0.5 * deltaTime, 0);
        }
    }

    if(state == 1) {

        smoothScroll *= 0.995;
        zoom += smoothScroll * deltaTime;
        if(zoom < 0.5) {
            smoothScroll += 15 * deltaTime;
        } else if(zoom > 4) {
            smoothScroll -= 20 * deltaTime;
        }

        SH->updateMyShip();
    }
}

void Game::switchToMainMenu()
{
    GUI->unloadAllElements();

    GUI->addButton(
        TM->returnTexture("assets/sprite/menu/button/new.png"),
        TM->returnTexture("assets/sprite/menu/button/new-selected.png"),
        0, -20, std::bind(&Game::switchToNewMenu, this), 0
    );
    GUI->addButton(
        TM->returnTexture("assets/sprite/menu/button/load.png"),
        TM->returnTexture("assets/sprite/menu/button/load-selected.png"),
        0, 6, nullptr, 1
    );
    GUI->addButton(
        TM->returnTexture("assets/sprite/menu/button/quit.png"),
        TM->returnTexture("assets/sprite/menu/button/quit-selected.png"),
        0, 32, std::bind(&Game::stop, this), 2
    );
    // GUI->addText(
    //     "holy shit you can't do anyitnhg 10/10!!",
    //     -160,
    //     92,
    //     0,
    //     SDL_Color{128, 128, 128},
    //     3
    // );
}

void Game::switchToNewMenu()
{
    SM->unloadSound(1);
    GUI->unloadAllElements();
    SM->playSound(0);

    GUI->addPopup({-150, -92, 300, 184}, 0);
    GUI->addButton(
        TM->returnTexture("assets/sprite/menu/button/x.png"),
        TM->returnTexture("assets/sprite/menu/button/x-selected.png"),
        140, -83, std::bind(&Game::switchToMainMenu, game), 2
    );
    // GUI->addText(
    //     "Create a ship",
    //     -143,
    //     -86,
    //     1,
    //     SDL_Color{99, 155, 255},
    //     3
    // );
    GUI->addButton(
        TM->returnTexture("assets/sprite/menu/button/launch.png"),
        TM->returnTexture("assets/sprite/menu/button/launch-selected.png"),
        103, 76, std::bind(&Game::switchToGame, game), 4
    );
}

void Game::switchToGame() {
    SB->regenerateStars();
    SM->playSound(0); // make new sound

    GUI->unloadAllElements();
    state = 1;
    TM->unloadAllTextures();
    SH->createShip();
}

void Game::render()
{
    if(renderer != nullptr) {
        SDL_RenderClear(renderer);
    }
    float ticks = SDL_GetTicks64();
    int starOffset = ticks / 10;

    if(state == 0)
    {
        SB->renderStars((((mouseX - wWidth / 2) / wWidth) * 160) + starOffset, (((mouseY - wHeight / 2) / wHeight) * 100) + starOffset);
        TM->renderLoadedTexture(1, std::clamp(-700 + static_cast<int>(ticks) / 3, -700, 0), -40);

        TM->renderLoadedTexture(0, 0, 0);

        if(SDL_GetTicks() >= 2250) {
            SDL_Rect rect = TM->rectOffset(2, 0, -40);

            rect.h = std::min((static_cast<float>(ticks - 2250) / 10), static_cast<float>(rect.h));

            TM->renderCustom(TM->texture(2), TM->rect(2), rect);
        }
    }
    else if(state == 1)
    {
        SB->renderStars(SH->getCoordinates(0).x, SH->getCoordinates(0).y);
        SH->renderShip(0);
        SH->updateMyShip();
        GUI->renderInventory(10);
    }

    GUI->renderElements();

    SDL_Rect rect = TM->scaleToScreen(TM->textureToRect(cursor));
    rect.x = (mouseX);
    rect.y = (mouseY);

    TM->renderCustom(cursor, {0, 0, 8, 8}, rect);

    if(renderer != nullptr) {
        SDL_RenderPresent(renderer);
    }
}

void Game::clean()
{
    eventThreadRunning = false;
    if (eventThread.joinable()) {
        eventThread.join();
    }
    isRunning = false;
    delete TM;
    delete SM;
    delete SB;
    delete RE;
    delete TE;
    delete GUI;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "a\n";
}
