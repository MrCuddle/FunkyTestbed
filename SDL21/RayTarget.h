#pragma once
#include <SDL.h>
#include <memory>
#include "Line.h"
#include "GameRect.h"

struct GameRect;
struct Line;

struct RayTarget
{
    float originX, originY;
    std::shared_ptr<GameRect> target;
    Line l1, l2, l3, l4;

    RayTarget(std::shared_ptr<GameRect> target, float x, float y);

    void UpdateRays(float x, float y);

    void Render(SDL_Renderer* ren);
};