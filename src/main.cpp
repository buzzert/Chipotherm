#include <SDL2/SDL.h>

#include <libgen.h>
#include <memory>

#include "main_scene.h"
#include "label_actor.h"

int main(int argc, char **argv)
{
    int canvasWidth = 640;
    int canvasHeight = 480;
    bool windowed = false;

    if (argc > 1 && strcmp("-w", argv[1]) == 0) {
        windowed = true;
    }

    Rect canvasRect(0, 0, canvasWidth, canvasHeight);

    MainScene mainScene(canvasRect, windowed);

    auto label = std::unique_ptr<LabelActor>(new LabelActor(Rect(0, 0, 600, 200), "Hello!!"));
    mainScene.add_actor(std::move(label));

    bool running = true;
    const int kTicksPerFrame = 1000 / 60;

    while (running) {
        Uint32 startTime = SDL_GetTicks();

        SDL_Event e;
        while ( SDL_PollEvent( &e ) != 0 ) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        mainScene.update();
        mainScene.render();

        Uint32 ticks = SDL_GetTicks() - startTime;
        if (ticks < kTicksPerFrame) {
            SDL_Delay(kTicksPerFrame - ticks);
        }
    }

    return 0;
}
