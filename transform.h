//
// Created by Kirk Haller on 9/13/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_TRANSFORM_H
#define MULTIVARIATE_POLYNOMIALS_TRANSFORM_H

#include "point.h"

// Computes a transform to take a box defined by the lower left and upper right bounds, and mapping it to
// the cube [-1,1]^d.
class Transform {
    std::unique_ptr<Point> scale;
    std::unique_ptr<Point> translation;

public:
    Transform() : scale(nullptr), translation(nullptr) {}

    Transform(const Point &lower_left, const Point &upper_right) : scale(nullptr), translation(nullptr) {
        instantiate_from_extents(lower_left, upper_right);
    }

    void instantiate_from_extents(const Point &lower_left, const Point &upper_right);

    void instantiate_from_points(const std::vector<Point> &points_in);

    void apply(Point &point);

    void invert(Point &point);
};


#endif //MULTIVARIATE_POLYNOMIALS_TRANSFORM_H
