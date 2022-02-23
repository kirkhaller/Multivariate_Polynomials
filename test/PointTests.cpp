//
// Created by Kirk Haller on 6/27/20.
//

#include <gtest/gtest.h>
#include "../Geometry/point.h"
#include "../Geometry/transform.h"

namespace {

    TEST(point_test, point_creation_test) {
        point_t initial_value{0.0, 1.0, 2.0};
        point_t initial_value2{0.0, 1.0, 3.0};
        Point point = Point(initial_value);
        Point point2 = Point(initial_value);
        Point point3 = Point(initial_value2);

        EXPECT_EQ(3, point.dimension());
        EXPECT_EQ(initial_value[0], point.get_value(0));
        EXPECT_EQ(initial_value[1], point.get_value(1));
        EXPECT_EQ(initial_value[2], point.get_value(2));
        EXPECT_DEATH(int value = point.get_value(4), "Index for point out of range");

        EXPECT_TRUE(point == point2);
        EXPECT_FALSE(point == point3);
        EXPECT_TRUE(point != point3);
        EXPECT_FALSE(point != point2);
    }

    TEST(point_test, point_evaluation) {
        Point zero({0, 0});
        Point x({1, 0});
        Point y({0, 1});
        Point half_x({0.5, 0});
        Point half_y({0, 0.5});
        Point x_plus_y({1, 1});

        Multi_index index_00(m_index_t{0, 0});
        Multi_index index_10(m_index_t{1, 0});
        Multi_index index_01(m_index_t{0, 1});
        Multi_index index_11(m_index_t{1, 1});

        EXPECT_EQ(zero.power(index_00), 1);
        EXPECT_EQ(x.power(index_00), 1);
        EXPECT_EQ(y.power(index_00), 1);

        EXPECT_EQ(zero.power(index_10), 0);
        EXPECT_EQ(x.power(index_10), 1);
        EXPECT_EQ(y.power(index_10), 0);
        EXPECT_EQ(half_x.power(index_10), 0.5);
        EXPECT_EQ(half_y.power(index_10), 0);
        EXPECT_EQ(x_plus_y.power(index_10), 1);

        EXPECT_EQ(zero.power(index_01), 0);
        EXPECT_EQ(x.power(index_01), 0);
        EXPECT_EQ(y.power(index_01), 1);
        EXPECT_EQ(half_x.power(index_01), 0);
        EXPECT_EQ(half_y.power(index_01), 0.5);
        EXPECT_EQ(x_plus_y.power(index_01), 1);

        EXPECT_EQ(zero.power(index_11), 0);
        EXPECT_EQ(x.power(index_11), 0);
        EXPECT_EQ(y.power(index_11), 0);
        EXPECT_EQ(half_x.power(index_11), 0);
        EXPECT_EQ(half_y.power(index_11), 0);
        EXPECT_EQ(x_plus_y.power(index_11), 1);

    }

    TEST(transform_test, simple_test) {
        Point upper_right({1, 1, 1});
        Point lower_left({-1, -1, -1});
        Point center({0, 0, 0});
        Point copy_upper_right(upper_right);
        Point copy_lower_left(lower_left);
        Point copy_center(center);

        Transform trans(lower_left, upper_right);
        trans.apply(copy_upper_right);
        trans.apply(copy_lower_left);
        trans.apply(copy_center);
        EXPECT_EQ(upper_right, copy_upper_right);
        EXPECT_EQ(lower_left, copy_lower_left);
        EXPECT_EQ(center, copy_center);

        trans.invert(copy_upper_right);
        trans.invert(copy_lower_left);
        trans.invert(copy_center);
        EXPECT_EQ(upper_right, copy_upper_right);
        EXPECT_EQ(lower_left, copy_lower_left);
        EXPECT_EQ(center, copy_center);

        EXPECT_DEATH(Transform trans_bad(upper_right, lower_left), "");
        EXPECT_DEATH(Transform trans_bad2(upper_right, upper_right), "");
    }

    TEST(transform_test, scale_test) {
        Point upper_right({2, 2, 2});
        Point lower_left({-2, -2, -2});
        Point center({0, 0, 0});
        Point original_upper_right(upper_right);
        Point original_lower_left(lower_left);
        Point original_center(center);
        Point cube_upper_right({1, 1, 1});
        Point cube_lower_left({-1, -1, -1});
        Point cube_center(center);

        Transform trans(lower_left, upper_right);
        trans.apply(lower_left);
        trans.apply(upper_right);
        trans.apply(center);
        EXPECT_EQ(upper_right, cube_upper_right);
        EXPECT_EQ(lower_left, cube_lower_left);
        EXPECT_EQ(center, cube_center);

        trans.invert(upper_right);
        trans.invert(lower_left);
        trans.invert(center);
        EXPECT_EQ(upper_right, original_upper_right);
        EXPECT_EQ(lower_left, original_lower_left);
        EXPECT_EQ(center, original_center);
    }

    TEST(transform_test, translate_test) {
        Point upper_right({2, 2, 2});
        Point lower_left({0, 0, 0});
        Point center({1, 1, 1});
        Point original_upper_right(upper_right);
        Point original_lower_left(lower_left);
        Point original_center(center);
        Point cube_upper_right({1, 1, 1});
        Point cube_lower_left({-1, -1, -1});
        Point cube_center({0, 0, 0});

        Transform trans(lower_left, upper_right);
        trans.apply(lower_left);
        trans.apply(upper_right);
        trans.apply(center);
        EXPECT_EQ(upper_right, cube_upper_right);
        EXPECT_EQ(lower_left, cube_lower_left);
        EXPECT_EQ(center, cube_center);

        trans.invert(upper_right);
        trans.invert(lower_left);
        trans.invert(center);
        EXPECT_EQ(upper_right, original_upper_right);
        EXPECT_EQ(lower_left, original_lower_left);
        EXPECT_EQ(center, original_center);
    }

    TEST(transform_test, scale_and_translate_test) {
        Point upper_right({2, 3, 4});
        Point lower_left({0, 0, 0});
        Point center({1, 1.5, 2});
        Point original_upper_right(upper_right);
        Point original_lower_left(lower_left);
        Point original_center(center);
        Point cube_upper_right({1, 1, 1});
        Point cube_lower_left({-1, -1, -1});
        Point cube_center({0, 0, 0});

        Transform trans(lower_left, upper_right);
        trans.apply(lower_left);
        trans.apply(upper_right);
        trans.apply(center);
        EXPECT_EQ(upper_right, cube_upper_right);
        EXPECT_EQ(lower_left, cube_lower_left);
        EXPECT_EQ(center, cube_center);

        trans.invert(upper_right);
        trans.invert(lower_left);
        trans.invert(center);
        EXPECT_EQ(upper_right, original_upper_right);
        EXPECT_EQ(lower_left, original_lower_left);
        EXPECT_EQ(center, original_center);
    }

}
