/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include <bubbles/bubbles.h>
#include <memory>
#include <cstdlib>
#include <cstring>

#include "primary_scene.h"

using namespace Bubbles;

int main(int argc, char **argv)
{
    // CHIP resolution is 480x272
    const int canvasWidth = 480;
    const int canvasHeight = 272;
    const float scale = 1.0; // for testing on hiDPI

    bool windowed = false;
    if (argc > 1 && strcmp("-w", argv[1]) == 0) {
        windowed = true;
    }

    PrimaryScene mainScene(Rect(0, 0, canvasWidth, canvasHeight), windowed, scale);
    mainScene.set_scale(scale);
    if (!windowed) {
        mainScene.set_hides_cursor(true);
    }

    // Start scene's runloop
    mainScene.run();
    return 0;
}
