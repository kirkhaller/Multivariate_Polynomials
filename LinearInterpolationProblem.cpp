//
// Created by Kirk Haller on 8/3/20.
//

#include "LinearInterpolationProblem.h"

LinearInterpolationProblem::LinearInterpolationProblem(const vector<Point> &points_in) {
    for (const auto &point: points_in) {
        Lagrange new_lagrange_point(point);
        lagranges.push_back(new_lagrange_point);
    }
}

LinearInterpolationProblem::~LinearInterpolationProblem() {
    for (auto lagrange: lagranges) {
        // This will safely delete the polynomial
        lagrange.set_polynomial(nullptr);
    }
}

int LinearInterpolationProblem::get_degree() const {
    int max_degree = 0;
    for (auto &lagrange: lagranges) {
        if (lagrange.get_degree() <= max_degree) continue;
        max_degree = lagrange.get_polynomial()->get_degree();
    }
    return max_degree;
}

