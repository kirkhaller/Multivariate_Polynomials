//
// Created by Kirk Haller on 8/11/20.
//


#include "gtest/gtest.h"
#include "../Geometry/point.h"
#include "../Polynomials/LinearInterpolationProblem.h"

namespace {
    class LinearInterpolationProblemTest : public ::testing::Test {
        int max_rows = 5;
        int max_columns = 5;

        void SetUp() override {
            points.reserve((max_rows + 1) * (max_columns + 1));
            for (int rows = 0; rows <= max_rows; rows++) {
                for (int columns = 0; columns <= max_columns; columns++) {
                    point_t point_vec = {2.0 * double(rows) / max_rows - 1.0,
                                         2.0 * double(columns) / max_columns - 1.0};
                    points.emplace_back(point_vec);
                }
            }
            extra_points.reserve(max_rows * max_columns);
            for (int rows = 0; rows < max_rows; rows++) {
                for (int columns = 0; columns < max_columns; columns++) {
                    point_t point_vec = {(2.0 * double(rows) + 0.5) / max_rows - 1.0,
                                         (2.0 * double(columns) + 0.5) / max_columns - 1.0};
                    extra_points.emplace_back(point_vec);
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
        vector<Point> extra_points;
    };
}

TEST_F(LinearInterpolationProblemTest, TestEdgeCases) {
    vector<Point> no_points;
    LinearInterpolationProblem test_lip(no_points);
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 0);
    EXPECT_EQ(test_lip.dimension(), -1);
    EXPECT_TRUE(test_lip.validate_results());
}

TEST_F(LinearInterpolationProblemTest, TestOnePoint) {
    vector<Point> one_point;
    one_point.push_back(*Point_Lookup(0, 0));

    LinearInterpolationProblem test_lip(one_point);
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(x_bias));
    EXPECT_EQ(test_lip.get_degree(), 0);
    EXPECT_EQ(test_lip.dimension(), 2);
    EXPECT_TRUE(test_lip.validate_results());

    EXPECT_NO_FATAL_FAILURE(test_lip.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(least));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 0);
    EXPECT_TRUE(test_lip.validate_results());

    EXPECT_NO_FATAL_FAILURE(test_lip.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(hm));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 0);
    EXPECT_TRUE(test_lip.validate_results());
}

TEST_F(LinearInterpolationProblemTest, TestTwoPoints) {
    vector<Point> two_point;
    two_point.push_back(*Point_Lookup(0, 0));
    two_point.push_back(*Point_Lookup(1, 1));

    LinearInterpolationProblem test_lip(two_point);
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(x_bias));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 1);
    EXPECT_TRUE(test_lip.validate_results());

    EXPECT_NO_FATAL_FAILURE(test_lip.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(least));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 1);
    EXPECT_TRUE(test_lip.validate_results());

    EXPECT_NO_FATAL_FAILURE(test_lip.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(hm));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 1);
    EXPECT_TRUE(test_lip.validate_results());
}

TEST_F(LinearInterpolationProblemTest, TestTensorProduct) {
    vector<Point> x_points;
    x_points.push_back(*Point_Lookup(0, 0));
    x_points.push_back(*Point_Lookup(1, 0));
    x_points.push_back(*Point_Lookup(2, 0));

    LinearInterpolationProblem test_lip(x_points);
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(x_bias));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 2);
    EXPECT_TRUE(test_lip.validate_results());

    EXPECT_NO_FATAL_FAILURE(test_lip.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(least));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 2);
    EXPECT_TRUE(test_lip.validate_results());

    EXPECT_NO_FATAL_FAILURE(test_lip.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(hm));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_EQ(test_lip.get_degree(), 2);
    EXPECT_TRUE(test_lip.validate_results());

    vector<Point> y_points;
    y_points.push_back(*Point_Lookup(0, 0));
    y_points.push_back(*Point_Lookup(0, 1));
    y_points.push_back(*Point_Lookup(0, 2));

    LinearInterpolationProblem test_lip_y(y_points);
    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(x_bias));
    EXPECT_NO_FATAL_FAILURE(test_lip_y.solve());
    EXPECT_EQ(test_lip_y.get_degree(), 2);
    EXPECT_TRUE(test_lip_y.validate_results());

    EXPECT_NO_FATAL_FAILURE(test_lip_y.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip_y.set_selector_type(least));
    EXPECT_NO_FATAL_FAILURE(test_lip_y.solve());
    EXPECT_EQ(test_lip_y.get_degree(), 2);
    EXPECT_TRUE(test_lip_y.validate_results());


    EXPECT_NO_FATAL_FAILURE(test_lip_y.reset());
    EXPECT_NO_FATAL_FAILURE(test_lip_y.set_selector_type(hm));
    EXPECT_NO_FATAL_FAILURE(test_lip_y.solve());
    EXPECT_EQ(test_lip_y.get_degree(), 2);
    EXPECT_TRUE(test_lip_y.validate_results());
}

TEST_F(LinearInterpolationProblemTest, TestFullGridXBias) {
    LinearInterpolationProblem test_lip(points);

    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(x_bias));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_TRUE(test_lip.validate_results());
}

TEST_F(LinearInterpolationProblemTest, TestFullGridLeast) {
    LinearInterpolationProblem test_lip(points);

    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(least));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_TRUE(test_lip.validate_results());
}

TEST_F(LinearInterpolationProblemTest, TestFullGridHM) {
    LinearInterpolationProblem test_lip(points);

    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(hm));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_TRUE(test_lip.validate_results());
}

TEST_F(LinearInterpolationProblemTest, TestAddPointXBias) {
    LinearInterpolationProblem test_lip(points);

    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(x_bias));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_TRUE(test_lip.validate_results());

    EXPECT_FALSE(test_lip.add_point(*points.begin()));
    EXPECT_TRUE(test_lip.add_point(*extra_points.begin()));
}


TEST_F(LinearInterpolationProblemTest, TestAddPointUndoXBias) {
    LinearInterpolationProblem test_lip(points);

    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(x_bias));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_TRUE(test_lip.validate_results());

    for (auto &point : extra_points) {
        EXPECT_TRUE(test_lip.add_point(point, true));
        EXPECT_TRUE(test_lip.validate_results());
        EXPECT_TRUE(test_lip.undo());
    }
}


TEST_F(LinearInterpolationProblemTest, TestAddPointUndoHM) {
    LinearInterpolationProblem test_lip(points);

    EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(hm));
    EXPECT_NO_FATAL_FAILURE(test_lip.solve());
    EXPECT_TRUE(test_lip.validate_results());

    for (auto &point : extra_points) {
        EXPECT_TRUE(test_lip.add_point(point, true));
        EXPECT_TRUE(test_lip.validate_results());
        EXPECT_TRUE(test_lip.undo());
    }
}