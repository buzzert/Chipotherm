#include "qube_actor.h"

#include "color.h"
#include <SDL2/SDL.h>
#include <iostream>

// For lack of a matrix type...
typedef std::vector<std::vector<double>> matrix_t;

Vec3 matrix_vector_product(matrix_t matrix, Vec3 p)
{
    Vec3 result;

    for (unsigned int i = 0; i < 3; i++) {
        std::vector<double> row = matrix[i];
        result[i] = 0.0;

        for (unsigned int j = 0; j < 3; j++) {
            double col = row[j];
            result[i] = result[i] + row[j] * p[j];
        }
    }

    return result;
}

Vec3 rotate_x(Vec3 p, double angle)
{
    double c = cos(angle);
    double s = sin(angle);

    matrix_t matrix = {
        { 1, 0,  0},
        { 0, c, -s},
        { 0, s,  c}
    };

    return matrix_vector_product(matrix, p);
}

Vec3 rotate_y(Vec3 p, double angle)
{
    double c = cos(angle);
    double s = sin(angle);

    matrix_t matrix = {
        { c, 0, s},
        { 0, 1, 0},
        {-s, 0, c}
    };

  return matrix_vector_product(matrix, p);
}

Vec3 rotate_z(Vec3 p, double angle)
{
    double c = cos(angle);
    double s = sin(angle);

    matrix_t matrix = {
        { c, -s, 0},
        { s,  c, 0},
        { 0,  0, 1}
    };

  return matrix_vector_product(matrix, p);
}

QubeActor::QubeActor(Rect rect)
    : Actor(rect)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, rect.width, rect.height, 32, DEFAULT_RGBA_MASK);    
    _surface = std::shared_ptr<SDL_Surface>(surface, SDL_FreeSurface);

    cairo_surface_t *cairosurf = cairo_image_surface_create_for_data(
        (unsigned char *)surface->pixels,
        CAIRO_FORMAT_RGB24,
        surface->w,
        surface->h,
        surface->pitch
    );

    cairo_t *cr = cairo_create(cairosurf);
    cairo_surface_destroy(cairosurf);
    _cairo_ctx = std::shared_ptr<cairo_t>(cr, cairo_destroy);
}

void QubeActor::update(SDL_Renderer *renderer)
{
    _time_offset += 0.01;;
}

void QubeActor::render(SDL_Renderer *renderer)
{
    cairo_t *cr = _cairo_ctx.get();

    // Clear
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    double radius = rect.height / 8;
    draw_tetrahedron(rect.width / 4, rect.height / 2, (radius *.1) + (radius *.9), _time_offset);

    // TODO: three classes doing this same thing now
    SDL_Rect dst_rect = rect.to_sdl_rect();

    // slow
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, _surface.get());
    SDL_SetTextureAlphaMod(texture, (alpha * 255));
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
    SDL_RenderCopy(renderer, texture, NULL, &dst_rect);
}

void QubeActor::draw_line(double x1, double y1, double x2, double y2, double size)
{
    cairo_t *cr = _cairo_ctx.get();
    cairo_set_line_width(cr, size);
    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    cairo_stroke(cr);
}

void QubeActor::draw_point(double x, double y, double size)
{
    cairo_t *cr = _cairo_ctx.get();
    cairo_arc(cr, x, y, size, 0, 2 * M_PI);
    cairo_fill(cr);
}

void QubeActor::draw_mesh(
    double x, double y, 
    std::vector<Vec3> vertices, 
    std::vector<std::pair<unsigned int, unsigned int>> edges,
    double scale,
    Color color,
    double time_offset)
{
    double point_size = 5.0;
    double line_size = 2.0;

    // Calculate points, but don't draw yet (needs to draw on top of lines)
    for (Vec3 &point : vertices) {
        point = rotate_x(point, time_offset / 2);
        point = rotate_y(point, time_offset / 3);
        point = rotate_z(point, time_offset / 5);
    }

    // Draw lines connecting the points
    cairo_set_source_rgb(_cairo_ctx.get(), 0.0, 0.0, 0.7);
    for (std::pair<unsigned int, unsigned int> edge : edges) {
        double x1 = vertices[edge.first].x * scale;
        double y1 = vertices[edge.first].y * scale;

        double x2 = vertices[edge.second].x * scale;
        double y2 = vertices[edge.second].y * scale;

        draw_line(x + x1, y + y1, x + x2, y + y2, line_size);
    }

    // Now draw points
    cairo_set_source_rgb(_cairo_ctx.get(), 0.0, 0.0, 1.0);
    for (const Vec3 &point : vertices) {
        draw_point(x + point.x * scale, y + point.y * scale, point_size);
    }
}

void QubeActor::draw_tetrahedron(double cx, double cy, double radius, double time_offset)
{
    double q = 0.57735026919;

    std::vector<Vec3> vertices = {
        { q, q, q},
        { q,-q,-q},
        {-q, q,-q},
        {-q,-q, q}
    };

    std::vector<std::pair<unsigned int, unsigned int>> edges = {
        { 0, 1 },
        { 0, 2 },
        { 0, 3 },
        { 1, 2 },
        { 2, 3 },
        { 3, 1 }
    };

    draw_mesh(cx, cy, vertices, edges, radius, Color(0, 0, 0xFF, 0xFF), time_offset);
}
