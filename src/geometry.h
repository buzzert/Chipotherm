/*
 * Created on Sat Dec 22 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include <vector>
#include <math.h>

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
