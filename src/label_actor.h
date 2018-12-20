#ifndef TEXTURE_ACTOR_H
#define TEXTURE_ACTOR_H

#include "actor.h"
#include "color.h"

#include <string>
#include <memory>

class LabelActor : public Actor
{
public:
    LabelActor(Rect rect, std::string contents);
    ~LabelActor() override;

    void set_contents(std::string &contents);
    const std::string& get_contents() const;

    void set_foreground_color(Color &c);
    const Color& get_foreground_color() const;

    void update(SDL_Renderer *renderer) override;
    void render(SDL_Renderer *renderer) override;
private:
    std::string _contents;
    Color       _foreground_color;

    bool _needs_texture_update;
    std::shared_ptr<SDL_Texture> _texture;
    void update_texture(SDL_Renderer *renderer);
};

#endif
