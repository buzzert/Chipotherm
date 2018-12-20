#include <cstdint>
#include <SDL2/SDL.h>

struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
        : red(red), green(green), blue(blue), alpha(alpha) {};
    
    Uint32 to_combined_color(SDL_PixelFormat *pf) {
        return SDL_MapRGBA(pf, red, green, blue, alpha);
    }

    SDL_Color to_sdl_color() {
        return { red, green, blue, alpha };
    }
};

