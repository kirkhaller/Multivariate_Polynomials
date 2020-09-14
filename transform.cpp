//
// Created by Kirk Haller on 9/13/20.
//

#include "transform.h"


Transform::Transform(Point &lower_left, Point &upper_right) {
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
        translation = std::make_shared<Point>(translation_test);
    }
    if (set_scale) {
        scale = std::make_shared<Point>(scale_test);
    }
}


Transform::Transform(const std::vector<Point> &points_in) {
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

    bool set_scale = false;
    bool set_translation = false;
    point_t translation_test, scale_test;
    for (int index = 0; index < upper.size(); index++) {
        double upper_value = upper[index];
        double lower_value = lower[index];
        double average = (upper_value + lower_value) / 2.0;
        double differance = (upper_value - lower_value) / 2.0;
        assert(differance > 0);
        double scale_value = std::max(1.0, differance);
        if (!set_translation && fabs(differance) > d_point_tolerance) {
            set_translation = true;
        }
        if (!set_scale && scale_value != 1.0) {
            set_scale = true;
        }
        translation_test.push_back(average);
        scale_test.push_back(scale_value);
    }

    if (set_translation) {
        translation = std::make_shared<Point>(translation_test);
    }
    if (set_scale) {
        scale = std::make_shared<Point>(scale_test);
    }
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

