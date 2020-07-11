//
// Created by Kirk Haller on 6/25/20.
//
#ifndef MULTIVARIATE_POLYNOMIALS_POINT_H
#define MULTIVARIATE_POLYNOMIALS_POINT_H

#include <math.h>
#include "Multiindex.h"
#include <string>
#include <vector>

#define d_point_tolerance 0.000000001
#define point_t std::vector<double>

class Point {

private:
    point_t value;

public:
    Point(const point_t &point_in);

    std::string description() const;

    inline int dimension() const { return value.size(); }

    double get_value(const int index) const;

    double power(const Multi_index &exponent) const;
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
