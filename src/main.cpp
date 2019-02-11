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

    // Options
    float scale = 1.0; // for testing on hiDPI
    bool windowed = false;
    std::string server_url = "";

    int opt_idx = 1;
    if (argc > opt_idx && strcmp("-w", argv[opt_idx]) == 0) {
        windowed = true;
        scale = 2.0;
        opt_idx++;
    }

    if (argc > opt_idx && strlen(argv[opt_idx]) > 0) {
        server_url = std::string(argv[opt_idx]);
        opt_idx++;
    }

    PrimaryScene mainScene(Rect(0, 0, canvasWidth, canvasHeight), windowed, scale, server_url);
    if (!windowed) {
        mainScene.set_hides_cursor(true);
    }

    // Start scene's runloop
    mainScene.run();
    return 0;
}
