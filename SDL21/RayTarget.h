#pragma once
#include <SDL.h>
#include "GameRect.h"
#include "Line.h"
#include <memory>

struct GameRect;
struct Line;

struct RayTarget
{
    float originX, originY;
    std::shared_ptr<GameRect> target;
    Line l1, l2, l3, l4;

    RayTarget(shared_ptr<GameRect> target, float x, float y)
    {
        this->target = target;
        UpdateRays(x, y);
    }

    void UpdateRays(float x, float y)
    {
        originX = x;
        originY = y;
        l1.x1 = l2.x1 = l3.x1 = l4.x1 = x;
        l1.y1 = l1.y1 = l1.y1 = l1.y1 = y;
        l1.x2 = target->rect.x;
        l1.y2 = target->rect.y;
        l2.x2 = target->rect.x + target->rect.w;
        l2.x2 = target->rect.y;
        l3.x2 = target->rect.x;
        l3.y2 = target->rect.y + target->rect.h;
        l4.x2 = target->rect.x + target->rect.w;
        l4.x2 = target->rect.y + target->rect.h;
    }

    void Render(SDL_Renderer* ren)
    {
        SDL_SetRenderDrawColor(ren, 0xff, 0x00, 0x00, 0x55);
        SDL_RenderDrawLine(ren, l1.x1, l1.y1, l1.x2, l1.y1);
        SDL_RenderDrawLine(ren, l2.x1, l2.y1, l2.x2, l2.y1);
        SDL_RenderDrawLine(ren, l3.x1, l3.y1, l3.x2, l3.y1);
        SDL_RenderDrawLine(ren, l4.x1, l4.y1, l4.x2, l4.y1);
    }
};