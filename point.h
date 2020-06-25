//
// Created by Kirk Haller on 6/25/20.
//
#ifndef MULTIVARIATE_POLYNOMIALS_POINT_H
#define MULTIVARIATE_POLYNOMIALS_POINT_H

#include <math.h>
#include "Multiindex.h"
#include <string>

#define d_point_tolerance 0.000000001

class Point {
    Point();

    std::string description() const;

public:
    int dimension() const { return -1; }

    int get_value(const int index) const;

    double power(Multi_index &exponent) const;
};

inline bool operator==(const Point &lhs, const Point &rhs) {
    if (lhs.dimension() != rhs.dimension())
        return false;

    for (int index = 0; index < lhs.dimension(); index++) {
        if (fabs(lhs.get_value(index) - rhs.get_value(index)) > d_point_tolerance)
            return false;
    }

    return true;
}

inline bool operator!=(const Point &lhs, const Point &rhs) {
    return !(lhs == rhs);
}

#endif //MULTIVARIATE_POLYNOMIALS_POINT_H
