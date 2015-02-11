#pragma once
#include <vector>
#include <memory>
#include "RayTarget.h"


struct RayTarget;

using namespace std;

struct GameRect
{
    SDL_Rect rect;
    float currentX, currentY, prevX, prevY, moveSpeed;
    Uint8 r, g, b, a;

    vector<RayTarget> rayTargets;

    GameRect(int x, int y, int w, int h);

    void Move(float x, float y);

    void UpdateRayTargets();
};