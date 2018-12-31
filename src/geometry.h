/*
 * Created on Sat Dec 22 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include <string>
#include <vector>
#include <math.h>

// for SDL_Rect
#include <SDL2/SDL.h>

struct Rect
{
    float x;
    float y;

    int width;
    int height;

    Rect() : x(0), y(0), width(0), height(0) {};
    Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {};
    SDL_Rect to_sdl_rect() {
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = width;
        rect.h = height;

        return rect;
    };

    std::string to_string() const {
        return "Rect(" + std::to_string(x) + ", " + std::to_string(y) 
                       + ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
    };
};

struct Vec2 {
    double x;
    double y;
};

struct Vec3 {
    double x;
    double y;
    double z;

    double& operator[] (const int index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw;
        }
    }
};

// For lack of a matrix type...
typedef std::vector<std::vector<double>> matrix_t;

Vec3 rotate_x(Vec3 p, double angle);
Vec3 rotate_y(Vec3 p, double angle);
Vec3 rotate_z(Vec3 p, double angle);

Vec3 matrix_vector_product(matrix_t matrix, Vec3 p);
