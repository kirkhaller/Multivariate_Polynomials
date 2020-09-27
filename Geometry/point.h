//
// Created by Kirk Haller on 6/25/20.
//
#ifndef MULTIVARIATE_POLYNOMIALS_POINT_H
#define MULTIVARIATE_POLYNOMIALS_POINT_H

#include <cmath>
#include "../Polynomials/Multiindex.h"
#include <string>
#include <vector>

#define d_point_tolerance 0.000000001
#define point_t std::vector<double>

class Point {

private:
    point_t value;

public:
    explicit Point(const point_t &point_in);

    [[nodiscard]] std::string description() const;

    [[nodiscard]] inline unsigned long dimension() const { return value.size(); }

    [[nodiscard]] double get_value(int index) const;

    void set_value(int index, double value_in) {
        assert(index < dimension());
        value[index] = value_in;
    }

    [[nodiscard]] double power(const Multi_index &exponent) const;

    double vector_length() const;

    Point &operator-=(const Point &rhs) {
        assert(dimension() == rhs.dimension());

        for (int index = 0; index < value.size(); index++) {
            value[index] -= rhs.get_value(index);
        }

        return *this;
    }

    Point &operator+=(const Point &rhs) {
        assert(dimension() == rhs.dimension());

        for (int index = 0; index < value.size(); index++) {
            value[index] += rhs.get_value(index);
        }

        return *this;
    }

};

inline Point operator-(const Point &lhs, const Point &rhs) {
    Point out = lhs;
    out -= rhs;
    return out;
}

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
