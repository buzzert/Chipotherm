/*
 * Created on Mon Jan 28 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

// TODO: currently a lot of this behavior overlaps with button_actor. Ideally,
// button_actor would encapsulate a rounded_title_actor instead. 

#pragma once

#include <bubbles/bubbles.h>
#include <string>

using namespace Bubbles;

DEF_SHARED_PTR(RoundedTitleActor)

class RoundedTitleActor : public Actor 
{
public:
    RoundedTitleActor(Rect r, std::string title);

    void update() override;
    void render(cairo_t *cr, Rect in_rect) override;
    void layout_actors() override;

    void set_foreground_color(Color c);
    void set_label_contents(const std::string contents);

    bool get_filled() const { return _filled; };
    void set_filled(bool filled);

    // Flash animation to indicate changing state
    void flash();

    bool pulsing = false;
    
private:
    std::shared_ptr<LabelActor> _label;
    std::shared_ptr<LabelActor> _title_label;

    bool   _filled;
    Color  _foreground_color;

    double _pulse_begin = 0.0;
    bool   _pulse_direction = false;
    double _pulse_progress = 0.0;

    bool   _flashing = false;
    double _flash_progress = 0.0;
    double _flash_begin = 0.0;
};
