//
// Created by Kirk Haller on 6/27/20.
//

#include "gtest/gtest.h"
#include "../point.h"

namespace {

    TEST(point_test, point_creation_test) {
        point_t initial_value{0.0, 1.0, 2.0};
        Point point = Point(initial_value);

        EXPECT_EQ(3, point.dimension());
        EXPECT_EQ(initial_value[0], point.get_value(0));
        EXPECT_EQ(initial_value[1], point.get_value(1));
        EXPECT_EQ(initial_value[2], point.get_value(2));
        EXPECT_DEATH(point.get_value(4), "Index for point out of range");
    }

}
