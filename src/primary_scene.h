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
#include "sounds.h"

using namespace Bubbles;

class PrimaryScene : public MainScene
{
public:
    // `server_url` refers to the command and control server
    PrimaryScene(Rect canvasRect, bool windowed, double scale, std::string server_url);

    void update() override;

protected:
    void pointer_event(int x, int y, bool pressed) override;

private:
    ActorGridPtr initialize_status_grid();
    ActorGridPtr initialize_statistics_grid();
    ActorGridPtr initialize_controls_grid();
    void user_set_monitoring_enabled(bool enabled);

    LabelActorPtr _online_label;
    LabelActorPtr _status_label;

    RoundedTitleActorPtr _current_temp_indicator;
    RoundedTitleActorPtr _target_temp_indicator;

    GraphActorPtr _graph;
    QubeActorPtr  _qube;

    ButtonActorPtr _heat_button;
    ButtonActorPtr _lithium_mode_button;

    Remote _remote;

    void update_ui_state();

    using Clock = std::chrono::system_clock;
    std::chrono::time_point<Clock> _last_sample_time;
    std::chrono::time_point<Clock> _last_graph_update_time;
    std::chrono::time_point<Clock> _last_status_blink_time;
    Monitor _monitor;

    SoundEngine _sounds;

    bool _lithium_enabled = false;
};

