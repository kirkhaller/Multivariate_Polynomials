//
// Created by Kirk Haller on 6/27/20.
//

#include "gtest/gtest.h"
#include "../point.h"

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

}
