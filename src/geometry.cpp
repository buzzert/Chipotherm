/*
 * Created on Sat Dec 22 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "geometry.h"

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

