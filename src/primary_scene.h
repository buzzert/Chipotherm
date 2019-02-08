/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/bubbles.h>
#include <chrono>

#include "button_actor.h"
#include "graph_actor.h"
#include "monitor.h"
#include "remote.h"
#include "rounded_title_actor.h"
#include "qube_actor.h"

using namespace Bubbles;

class PrimaryScene : public MainScene
{
public:
    PrimaryScene(Rect canvasRect, bool windowed, double scale = 1.0);

    void update() override;

private:
    ActorGridPtr initialize_status_grid();
    ActorGridPtr initialize_statistics_grid();
    ActorGridPtr initialize_controls_grid();

    LabelActorPtr _online_label;

    RoundedTitleActorPtr _current_temp_indicator;
    RoundedTitleActorPtr _target_temp_indicator;

    GraphActorPtr _graph;
    QubeActorPtr  _qube;

    ButtonActorPtr _heat_button;

    Remote _remote;

    void update_ui_state();

    using Clock = std::chrono::system_clock;
    std::chrono::time_point<Clock> _last_sample_time;
    std::chrono::time_point<Clock> _last_graph_update_time;
    Monitor _monitor;
};

