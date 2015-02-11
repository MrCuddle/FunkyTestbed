#include <iostream>
#include <SDL.h>
#include <vector>
#include <memory>
#include "GameRect.h"

using namespace std;

struct GameData
{
    vector<shared_ptr<GameRect>> actors;
    vector<shared_ptr<GameRect>> gameObjects;
    vector<shared_ptr<GameRect>> collidables;
    shared_ptr<GameRect> player;

    GameData(){}

    GameData(GameData&& rhs)
    {
        player = rhs.player;
        gameObjects = std::move(rhs.gameObjects);
        collidables = std::move(rhs.collidables);
        actors = std::move(rhs.actors);
    }
};

GameData initializeGame();

bool rectCollision(const shared_ptr<GameRect> r1, const shared_ptr<GameRect> r2);
void resolveCollision(shared_ptr<GameRect> r);
void updateLines(shared_ptr<GameRect> player, vector<shared_ptr<GameRect>> objects);

int main(int, char**){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 1368, 766, SDL_WINDOW_SHOWN);
    if (win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    std::string imagePath = "hehu.bmp";
    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    if (bmp == nullptr){
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_FreeSurface(bmp);
    if (tex == nullptr){
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    GameData gameData = initializeGame();

    bool quit = false;
    while (!quit){
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
            }
        }

        for (shared_ptr<GameRect> actor : gameData.actors)
        {
            for (shared_ptr<GameRect> collidable : gameData.collidables)
            {
                if (rectCollision(actor, collidable))
                    resolveCollision(actor);
            }
        }



        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        float movementX = 0, movementY = 0;
        if (keyState[SDL_SCANCODE_W])
            movementY -= gameData.player->moveSpeed;
        if (keyState[SDL_SCANCODE_S])
            movementY += gameData.player->moveSpeed;
        if (keyState[SDL_SCANCODE_A])
            movementX -= gameData.player->moveSpeed;
        if (keyState[SDL_SCANCODE_D])
            movementX += gameData.player->moveSpeed;
        gameData.player->Move(movementX, movementY);

        //int mouseX, mouseY;
        //SDL_GetMouseState(&mouseX, &mouseY);
        //gameData.player->rect.x = mouseX;
        //gameData.player->rect.y = mouseY;

        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(ren);
        for (shared_ptr<GameRect> r : gameData.gameObjects)
        {
            SDL_SetRenderDrawColor(ren, (*r).r, (*r).g, (*r).b, (*r).a);
            SDL_RenderFillRect(ren, &(*r).rect);
        }

        for (RayTarget r : gameData.player->rayTargets)
        {
            r.Render(ren);
        }

        SDL_SetRenderDrawColor(ren, 0xff, 0x00, 0x00, 0xff);
        //for (Line l : gameData.player->rayTargets)
        //{
        //    SDL_RenderDrawLine(ren, l.x1, l.y1, l.x2, l.y2);
        //}

        SDL_RenderPresent(ren);
    }
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

GameData initializeGame()
{
    GameData tempGameData;
    //PLAYER INIT
    shared_ptr<GameRect> player = make_shared<GameRect>(80, 80, 20, 20);

    player->b = 0x00;
    player->g = 0x00;
    player->moveSpeed = 1.1f;

    tempGameData.gameObjects.push_back(player);
    tempGameData.actors.push_back(player);
    tempGameData.player = player;

    //ENEMIES INIT

    shared_ptr<GameRect> r1 = make_shared<GameRect>(30, 30, 20, 20);
    
    r1->r = 0x00;
    r1->b = 0x00;

    tempGameData.gameObjects.push_back(r1);
    tempGameData.actors.push_back(r1);

    shared_ptr<GameRect> r2 = make_shared<GameRect>(60, 60, 20, 20);
    
    r2->g = 0x00;
    r2->r = 0x00;

    tempGameData.gameObjects.push_back(r2);
    tempGameData.actors.push_back(r2);

    shared_ptr<GameRect> obst1 = make_shared<GameRect>(120, 120, 400, 200);

    obst1->r = 0x06;
    obst1->g = 0xb4;
    obst1->b = 0xbd;

    tempGameData.gameObjects.push_back(obst1);
    tempGameData.collidables.push_back(obst1);

    for (int i = 0; i < tempGameData.gameObjects.size(); i++)
    {
        if (tempGameData.gameObjects[i] != player)
        {
            player->rayTargets.emplace_back(tempGameData.gameObjects[i], player->rect.x, player->rect.y);
        }
    }
    return tempGameData;
}

bool rectCollision(const shared_ptr<GameRect> r1, const shared_ptr<GameRect> r2)
{
    return SDL_HasIntersection(&r1->rect, &r2->rect);
}

void resolveCollision(shared_ptr<GameRect> r)
{
    float displacementX = r->prevX - r->currentX;
    float displacementY = r->prevY - r->currentY;
    r->Move(displacementX, displacementY);
}