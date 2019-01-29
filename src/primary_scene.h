/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/bubbles.h>
#include <sigc++/sigc++.h>

using namespace Bubbles;

class PrimaryScene : public MainScene
{
public:
    PrimaryScene(Rect canvasRect, bool windowed, double scale = 1.0);

private:
    ActorGridPtr initialize_status_grid();
    ActorGridPtr initialize_statistics_grid();
    ActorGridPtr initialize_controls_grid();

};
