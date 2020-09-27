//
// Created by Kirk Haller on 9/26/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_CONVEXHULL2D_H
#define MULTIVARIATE_POLYNOMIALS_CONVEXHULL2D_H

#include "point.h"
#include <vector>

using namespace std;
#define convex_hull_tolerance 0.000000001

class ConvexHull2D {
private:
    vector<Point> working_list;
    vector<Point> convex_hull;

public:
    ConvexHull2D(vector<Point> &input_points);

    bool point_in_convex_hull(const Point &point) const;

    int size() const { return convex_hull.size(); }


private:
    static double cross_product(const Point &fixed, const Point &test) {
        double value1 = fixed.get_value(0) * test.get_value(1);
        double value2 = fixed.get_value(1) * test.get_value(0);
        return value1 - value2;
    }

    static bool is_left(const Point &origin, const Point &end, const Point &test);

    static bool is_lower_right_of(const Point &origin, const Point &test);

    // Will permute the points in the working list.
    // The last element will be the point in the lower right corner
    void find_lower_right();

    void sort_by_angle();

    void compute_convex_hull();

    Point pop_working_list() {
        assert(!working_list.empty());
        Point out = working_list.back();
        working_list.pop_back();
        return out;
    }

    Point pop_convex_hull() {
        assert(!convex_hull.empty());
        Point out = convex_hull.back();
        convex_hull.pop_back();
        return out;
    }
};


#endif //MULTIVARIATE_POLYNOMIALS_CONVEXHULL2D_H
