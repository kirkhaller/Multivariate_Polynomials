//
// Created by Kirk Haller on 6/25/20.
//

#include "point.h"

Point::Point(const point_t &point_in) {
    value = point_t(point_in);
    value.shrink_to_fit();
}

std::string Point::description() const {
    assert(dimension() > 0 && "Dimension of a Point needs to be > 0");
    std::string string_out = "( ";
    for (int index = 0; index < dimension()-1; index++) {
        string_out += std::to_string(get_value(index));
        string_out += ", ";
    }
    string_out += std::to_string(get_value(dimension()-1));
    string_out += " )";
    return string_out;
}

double Point::get_value(int index) const {
    assert(index >= 0 && index < dimension() && "Index for point out of range");
    return value[index];
}

double Point::power(const Multi_index &exponent) const {
    assert(dimension() == exponent.dimension() && "Mismatched Dimensions");
    double value_out = 1;
    for (int index = 0; index < dimension(); index++) {
        value_out *= pow(get_value(index), exponent.get_value(index));
    }
    return value_out;
}


