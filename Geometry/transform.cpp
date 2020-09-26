//
// Created by Kirk Haller on 9/13/20.
//

#include "transform.h"


void Transform::instantiate_from_extents(const Point &lower_left, const Point &upper_right) {
    assert(lower_left.dimension() == upper_right.dimension());
    Point scale_test(lower_left), translation_test(upper_right);
    bool set_scale = false;
    bool set_translation = false;

    for (int index = 0; index < upper_right.dimension(); index++) {
        double upper = upper_right.get_value(index);
        double lower = lower_left.get_value(index);
        double average = (upper + lower) / 2.0;
        double differance = (upper - lower) / 2.0;
        assert(differance > 0);
        double scale_value = std::max(1.0, differance);
        if (!set_translation && fabs(differance) > d_point_tolerance) {
            set_translation = true;
        }
        if (!set_scale && scale_value != 1.0) {
            set_scale = true;
        }
        translation_test.set_value(index, average);
        scale_test.set_value(index, scale_value);
    }

    if (set_translation) {
        translation = std::make_unique<Point>(translation_test);
    }
    if (set_scale) {
        scale = std::make_unique<Point>(scale_test);
    }
}


void Transform::instantiate_from_points(const std::vector<Point> &points_in) {
    if (points_in.empty()) {
        return;
    }

    point_t lower, upper;

    for (int index = 0; index < points_in.begin()->dimension(); index++) {
        lower.push_back(-1);
        upper.push_back(1);
    }

    for (const auto &point : points_in) {
        for (int index = 0; index < point.dimension(); index++) {
            double value = point.get_value(index);
            if (value < lower[index]) {
                lower[index] = value;
            } else if (value > upper[index]) {
                upper[index] = value;
            }
        }
    }

    Point lower_left(lower), upper_right(upper);
    instantiate_from_extents(lower_left, upper_right);
}

void Transform::apply(Point &point) {
    if (translation != nullptr) {
        point -= *translation;
    }

    if (scale != nullptr) {
        for (int index = 0; index < scale->dimension(); index++) {
            double point_value = point.get_value(index);
            double new_value = point_value / scale->get_value(index);
            point.set_value(index, new_value);
        }
    }
}

void Transform::invert(Point &point) {
    if (scale != nullptr) {
        for (int index = 0; index < scale->dimension(); index++) {
            double point_value = point.get_value(index);
            double new_value = point_value * scale->get_value(index);
            point.set_value(index, new_value);
        }
    }
    if (translation != nullptr) {
        point += *translation;
    }
}

