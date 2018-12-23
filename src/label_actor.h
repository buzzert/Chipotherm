/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once 

#include "texture_actor.h"
#include "color.h"

#include <string>
#include <memory>

class LabelActor : public TextureActor
{
public:
    LabelActor(Rect rect, std::string contents);
    ~LabelActor() override;

    void set_font_prop(std::string &font_prop);
    const std::string& get_font_prop() const;

    void set_contents(std::string &contents);
    const std::string& get_contents() const;

    void set_foreground_color(Color &c);
    const Color& get_foreground_color() const;

    void update(SDL_Renderer *renderer) override;
private:
    std::string _font_prop;
    std::string _contents;
    Color       _foreground_color;
    bool        _needs_texture_update; 
    void update_texture(SDL_Renderer *renderer);
};
