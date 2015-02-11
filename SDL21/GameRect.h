#pragma once
#include <vector>
#include <memory>
#include "RayTarget.h"

using namespace std;

struct GameRect
{
    SDL_Rect rect;
    float currentX, currentY, prevX, prevY, moveSpeed;
    Uint8 r, g, b, a;

    vector<RayTarget> rayTargets;

    GameRect(int x, int y, int w, int h)
    {
        rect.x = x;
        currentX = x;
        rect.y = y;
        currentY = y;
        rect.w = w;
        rect.h = h;
        r = g = b = a = 0xff;
    }

    void Move(float x, float y){
        prevX = currentX;
        prevY = currentY;
        currentX += x;
        currentY += y;
        rect.x = currentX;
        rect.y = currentY;
    }

    void UpdateRayTargets()
    {
        for (RayTarget r : rayTargets)
            r.UpdateRays(rect.x + rect.w / 2, rect.y + rect.h / 2);
    }
};