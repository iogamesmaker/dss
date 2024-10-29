#pragma once

struct RandomEngine
{
public:
    RandomEngine();
    ~RandomEngine();
    float random(float a, float b);
    int randomint(int a, int b);
    bool randombool();
private:
    std::random_device rd;
    std::mt19937 gen;
};

extern RandomEngine *RE;
