#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cerr << "Unable to initialize SDL: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("chipotherm", 0, 0, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Error creating window: " << SDL_GetError() << endl;        
        return 1;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    bool running = true;
    while (running) {
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0x00, 0x00));
        SDL_UpdateWindowSurface(window);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                default:
                    break;
            }
        }

        SDL_Delay(500);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

