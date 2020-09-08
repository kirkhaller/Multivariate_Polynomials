//
// Created by Kirk Haller on 8/11/20.
//


#include "gtest/gtest.h"
#include "../point.h"
#include "../LinearInterpolationProblem.h"
#include <vector>

namespace {
    class LinearInterpolationProblemTest : public ::testing::Test {
        int max_rows = 4;
        int max_columns = 4;

        void SetUp() override {
            points.reserve(max_rows * max_columns);
            for (int rows = 0; rows <= max_rows; rows++) {
                for (int columns = 0; columns <= max_columns; columns++) {
                    point_t point_vec = {2.0 * double(rows) / max_rows - 1.0,
                                         2.0 * double(columns) / max_columns - 1.0};
                    points.emplace_back(point_vec);
                }
            }
        }

        void TearDown() override {
            points.clear();
        }

    protected:
        Point *Point_Lookup(int row, int column) {
            return &points[row * (max_rows + 1) + column];
        }

        vector<Point> points;
    };
}

TEST_F(LinearInterpolationProblemTest, TestEdgeCases) {
    vector<Point> no_points;
    LinearInterpolationProblem test_lip(no_points);
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 0);
    EXPECT_EQ(test_lip.dimension(), -1);
    EXPECT_TRUE(test_lip.valid_results());
}

TEST_F(LinearInterpolationProblemTest, TestOnePoint) {
    vector<Point> one_point;
    one_point.emplace_back(*Point_Lookup(0, 0));

    LinearInterpolationProblem test_lip(one_point);
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 0);
    EXPECT_EQ(test_lip.dimension(), 2);
    EXPECT_TRUE(test_lip.valid_results());

    EXPECT_NO_FATAL_FAILURE(test_lip.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(least));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 0);
    EXPECT_TRUE(test_lip.valid_results());
}

TEST_F(LinearInterpolationProblemTest, TestTwoPoints) {
    vector<Point> two_point;
    two_point.emplace_back(*Point_Lookup(0, 0));
    two_point.emplace_back(*Point_Lookup(1, 1));

    LinearInterpolationProblem test_lip(two_point);
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 1);
    EXPECT_TRUE(test_lip.valid_results());

    EXPECT_NO_FATAL_FAILURE(test_lip.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(least));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 1);
    EXPECT_TRUE(test_lip.valid_results());
}

TEST_F(LinearInterpolationProblemTest, TestTensorProduct) {
    vector<Point> x_points;
    x_points.emplace_back(*Point_Lookup(0, 0));
    x_points.emplace_back(*Point_Lookup(1, 0));
    x_points.emplace_back(*Point_Lookup(2, 0));

    LinearInterpolationProblem test_lip(x_points);
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 2);
    EXPECT_TRUE(test_lip.valid_results());

    EXPECT_NO_FATAL_FAILURE(test_lip.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(least));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 2);
    EXPECT_TRUE(test_lip.valid_results());

    vector<Point> y_points;
    y_points.emplace_back(*Point_Lookup(0, 0));
    y_points.emplace_back(*Point_Lookup(0, 1));
    y_points.emplace_back(*Point_Lookup(0, 2));

    LinearInterpolationProblem test_lip_y(y_points);
    EXPECT_NO_FATAL_FAILURE(test_lip_y.solve());
    EXPECT_EQ(test_lip_y.get_degree(), 2);
    EXPECT_TRUE(test_lip_y.valid_results());

    EXPECT_NO_FATAL_FAILURE(test_lip_y.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip_y.set_selector_type(least));
    EXPECT_NO_FATAL_FAILURE(test_lip_y.solve());
    EXPECT_EQ(test_lip_y.get_degree(), 2);
    EXPECT_TRUE(test_lip_y.valid_results());
}

TEST_F(LinearInterpolationProblemTest, TestFullGrid) {
    LinearInterpolationProblem test_lip(points);
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_TRUE(test_lip.valid_results());

    EXPECT_NO_FATAL_FAILURE(test_lip.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(least));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_TRUE(test_lip.valid_results());
}
