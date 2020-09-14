//
// Created by Kirk Haller on 9/13/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_TRANSFORM_H
#define MULTIVARIATE_POLYNOMIALS_TRANSFORM_H

#include "point.h"

// Computes a transform to take a box defined by the lower left and upper right bounds, and mapping it to
// the cube [-1,1]^d.
class Transform {
    std::shared_ptr<Point> scale;
    std::shared_ptr<Point> translation;

public:
    Transform() : scale(nullptr), translation(nullptr) {}

    Transform(Point &lower_left, Point &upper_right);

    explicit Transform(const std::vector<Point> &points_in);

    Transform(Transform &xform_in) = default;

    void apply(Point &point);

    void invert(Point &point);
};


#endif //MULTIVARIATE_POLYNOMIALS_TRANSFORM_H
