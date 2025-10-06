//
// Created by Kirk Haller on 9/26/20.
//

#include "ConvexHull2D.h"

ConvexHull2D::ConvexHull2D(const vector<Point> &input_points) {
    assert(input_points.empty() || input_points.begin()->dimension() == 2);

    working_list = input_points;

    compute_convex_hull();
}

bool ConvexHull2D::is_left(const Point &origin, const Point &end_in, const Point &test_in) {
    Point fixed = end_in - origin;
    Point test = test_in - origin;

    double value = cross_product(fixed, test);

    if (value > 0) {
        return true;
    } else if (value < 0) {
        return false;
    }

    return test.vector_length() >= fixed.vector_length();
}

bool ConvexHull2D::is_lower_right_of(const Point &origin, const Point &test) {
    double y_value = test.get_value(1) - origin.get_value(1);
    if (fabs(y_value) < convex_hull_tolerance) {
        return test.get_value(0) > origin.get_value(0);
    }

    return y_value < 0;
}

void ConvexHull2D::find_lower_right() {
    if (working_list.size() < 2) {
        return; // only one point, just return it.
    }

    unsigned long end_index = working_list.size() - 1;
    for (unsigned long index = 0; index < end_index; index++) {
        if (is_lower_right_of(working_list[end_index], working_list[index])) {
            swap(working_list[end_index], working_list[index]);
        }
    }
}

void ConvexHull2D::sort_by_angle() {
    if (working_list.empty()) {
        return;
    }

    Point origin = working_list.back();
    sort(working_list.begin(), working_list.end() - 1,
         [&](const Point &a, const Point &b) { return !is_left(origin, a, b); });
}

void ConvexHull2D::compute_convex_hull() {
    if (working_list.empty())
        return;

    if (working_list.size() == 1) {
        convex_hull.push_back(working_list.front());
        return;
    }

    find_lower_right();
    sort_by_angle();

    Point origin = pop_working_list();
    convex_hull.push_back(origin);
    Point last_end = pop_working_list();

    while (!working_list.empty()) {
        Point test = pop_working_list();
        while (!is_left(origin, last_end, test)) {
            last_end = pop_convex_hull();
            origin = convex_hull.back();
        }
        origin = last_end;
        last_end = test;
        convex_hull.push_back(origin);
    }
    convex_hull.push_back(last_end);
}

bool ConvexHull2D::point_in_convex_hull(const Point &point) const {
    if (convex_hull.empty())
        return false;

    if (convex_hull.size() == 1) {
        return convex_hull[0] == point;
    }

    if (convex_hull.size() == 2) {
        Point cv_vec = convex_hull[1] - convex_hull[0];
        Point test = point - convex_hull[0];
        if (fabs(cross_product(cv_vec, test)) < convex_hull_tolerance) {
            bool same_direction = (cv_vec.get_value(0) * test.get_value(0)
                                   + cv_vec.get_value(1) * test.get_value(1) > -convex_hull_tolerance);
            bool on_segment = test.vector_length() < cv_vec.vector_length();
            return same_direction && on_segment;
        }
    }

    for (int index = 0; index < convex_hull.size() - 1; index++) {
        if (!is_left(convex_hull[index], convex_hull[index + 1], point)) {
            return false;
        }
    }

    return is_left(convex_hull.back(), convex_hull.front(), point);
}



