//
// Created by Kirk Haller on 9/26/20.
//


#include "gtest/gtest.h"
#include "../Geometry/point.h"
#include "../Geometry/ConvexHull2D.h"
#include <vector>

TEST(ConvexHull2D, EmptyTest) {
    std::vector<Point> point_set;
    ConvexHull2D cv(point_set);
    EXPECT_TRUE(cv.size() == 0);
}

TEST(ConvexHull2D, OnePointTest) {
    Point one_point({0, 0});
    Point other_point({1, 0});
    std::vector<Point> point_set = {one_point};
    ConvexHull2D cv(point_set);
    EXPECT_TRUE(cv.size() == 1);
    EXPECT_TRUE(cv.point_in_convex_hull(one_point));
    EXPECT_FALSE(cv.point_in_convex_hull(other_point));
}

TEST(ConvexHull2D, ThreePointTest) {
    Point one_point({0, 0});
    Point other_point({1, 0});
    Point third_point({1, 1});
    Point center_point({0.5, 0.25});
    Point not_on_point({0, 1});
    std::vector<Point> point_set = {one_point, other_point, third_point};
    ConvexHull2D cv(point_set);
    EXPECT_TRUE(cv.size() == 3);
    EXPECT_TRUE(cv.point_in_convex_hull(center_point));
    EXPECT_FALSE(cv.point_in_convex_hull(not_on_point));
}
