#include "game.hpp"

RandomEngine::RandomEngine()
:gen(rd())
{}

RandomEngine::~RandomEngine() {}

float RandomEngine::random(float a, float b)
{
    std::uniform_real_distribution<> dis(a, b);
    return dis(gen);
}

int RandomEngine::randomint(int a, int b)
{
    std::uniform_real_distribution<> dis(a, b);
    return dis(gen);
}

bool RandomEngine::randombool()
{
    std::uniform_real_distribution<> dis(0, 1);
    return dis(gen) == 1;
}
