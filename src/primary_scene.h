/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/bubbles.h>
#include <sigc++/sigc++.h>

#include "io_control.h"
#include "button_actor.h"
#include "rounded_title_actor.h"

using namespace Bubbles;

class PrimaryScene : public MainScene
{
public:
    PrimaryScene(Rect canvasRect, bool windowed, double scale = 1.0);

private:
    ActorGridPtr initialize_status_grid();
    ActorGridPtr initialize_statistics_grid();
    ActorGridPtr initialize_controls_grid();

    RoundedTitleActorPtr _current_temp_indicator;
    RoundedTitleActorPtr _target_temp_indicator;

    ButtonActorPtr _heat_button;

    void update_ui_state();

    IOControl _controller;

};
