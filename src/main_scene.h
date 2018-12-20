#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "actor.h"

#include <vector>
#include <map>
#include <chrono>
#include <memory>

typedef std::chrono::high_resolution_clock::time_point update_time;

class MainScene
{
public:
    MainScene(Rect canvasRect, bool windowed);
    ~MainScene();

    void add_actor(std::unique_ptr<Actor>&& actor);

    void update();
    void render();

private:
    SDL_Window   *_window;
    SDL_Renderer *_renderer;

    std::vector<std::unique_ptr<Actor>> _actors;

    Rect _canvasRect;
};

#endif
