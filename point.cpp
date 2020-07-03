//
// Created by Kirk Haller on 6/25/20.
//

#include "point.h"
#include <cassert>

Point::Point(const point_t &point_in) {
    value = point_t(point_in);
}

std::string Point::description() const {
    assert(dimension() > 0);
    std::string string_out = "( ";
    for (int index = 0; index < dimension(); index++)
        string_out += std::to_string(get_value(index));
    string_out += " )";
    return string_out;
}

double Point::get_value(const int index) const {
    assert(index >= 0 && index < dimension()); // Mismatched Dimensions
    return value[index];
}

double Point::power(Multi_index &exponent) const {
    assert(dimension() == exponent.dimension());
    double value_out = 0;
    for (int index = 0; index < dimension(); index++) {
        value_out += pow(get_value(index), exponent.get_value(index));
    }
    return value_out;
}


