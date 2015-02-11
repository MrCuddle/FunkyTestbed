#include "GameRect.h"




void GameRect::UpdateRayTargets()
{
    for (RayTarget &r : rayTargets)
        r.UpdateRays(rect.x + rect.w / 2, rect.y + rect.h / 2);
}

void GameRect::Move(float x, float y)
{
    prevX = currentX;
    prevY = currentY;
    currentX += x;
    currentY += y;
    rect.x = currentX;
    rect.y = currentY;
}

GameRect::GameRect(int x, int y, int w, int h)
{
    rect.x = x;
    currentX = x;
    rect.y = y;
    currentY = y;
    rect.w = w;
    rect.h = h;
    r = g = b = a = 0xff;
}
