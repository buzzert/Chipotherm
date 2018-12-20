#include "main_scene.h"

#include "SDL2/SDL.h"
#include "texture_actor.h"

#include <iostream>
#include <sstream>
#include <random>

using namespace std;

typedef shared_ptr<Actor> actor_ptr;

MainScene::MainScene(Rect canvasRect, bool windowed)
    : _canvasRect(canvasRect)
{
    int flags = windowed ? SDL_WINDOW_SHOWN : SDL_WINDOW_FULLSCREEN_DESKTOP;

    _window = SDL_CreateWindow("Chipotherm", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, canvasRect.width, canvasRect.height, flags);
    if (_window) {
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    }
}

MainScene::~MainScene()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}


void MainScene::update()
{
    for (auto &a : _actors) {
        a->update(_renderer);
    }
}

void MainScene::add_actor(std::unique_ptr<Actor> &&a)
{
    _actors.push_back(std::move(a));
}

void MainScene::render()
{
    SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(_renderer);

    for (std::unique_ptr<Actor> &a : _actors) {
        a->render(_renderer);
    }

    SDL_RenderPresent(_renderer);
}
