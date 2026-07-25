//
// Created by Kirk Haller on 9/29/20.
//


#include <gtest/gtest.h>
#include <random>
#include "../Geometry/point.h"
#include "../Polynomials/LinearInterpolationProblem.h"
#include "../utilities/analysis.h"

namespace {
    class AnalysisTest : public ::testing::Test {
        int max_rows = 3;
        int max_columns = 3;

        void SetUp() override {
            points.reserve((max_rows + 1) * (max_columns + 1));
            for (int rows = 0; rows <= max_rows; rows++) {
                for (int columns = 0; columns <= max_columns; columns++) {
                    point_t point_vec = {2.0 * static_cast<double>(rows) / max_rows - 1.0,
                                         2.0 * static_cast<double>(columns) / max_columns - 1.0};
                    points.emplace_back(point_vec);
                }
            }
            extra_points.reserve(max_rows * max_columns);
            for (int rows = 0; rows < max_rows; rows++) {
                for (int columns = 0; columns < max_columns; columns++) {
                    point_t point_vec = {(2.0 * static_cast<double>(rows) + 0.5) / max_rows - 1.0,
                                         (2.0 * static_cast<double>(columns) + 0.5) / max_columns - 1.0};
                    extra_points.emplace_back(point_vec);
                }
            }

        }

        void TearDown() override {
            points.clear();
        }

    protected:
        vector<Point> points;
        vector<Point> extra_points;
    };


    TEST_F(AnalysisTest, TestFullGridXBias) {
        LinearInterpolationProblem test_lip(points);

        EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(x_bias));
        EXPECT_NO_FATAL_FAILURE(test_lip.solve());
        Analysis test_analysis(test_lip);
        test_analysis.print_data();
    }

    TEST_F(AnalysisTest, TestFullGridLeast) {
        LinearInterpolationProblem test_lip(points);

        EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(least));
        EXPECT_NO_FATAL_FAILURE(test_lip.solve());
        Analysis test_analysis(test_lip);
        test_analysis.print_data();
    }

    TEST_F(AnalysisTest, TestFullGridHM) {
        LinearInterpolationProblem test_lip(points);

        EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(hm));
        EXPECT_NO_FATAL_FAILURE(test_lip.solve());
        Analysis test_analysis(test_lip);
        test_analysis.print_data();
    }


    TEST_F(AnalysisTest, TestFullRandomAll) {
        int count = 20;
        int dimension = 2;
        std::random_device rd;
        std::mt19937 bitgen(rd());
        std::uniform_real_distribution<double> distrib(-1.0, 1.0);
        vector<Point> points;
        for (int loop = 0; loop < count; loop++) {
            point_t interpolation_point;
            for (int dim = 0; dim < dimension; dim++) {
                interpolation_point.push_back(distrib(bitgen));
            }
            points.emplace_back(interpolation_point);
            cout << "Point" << points.back().description() << "\n";
        }

        LinearInterpolationProblem test_lip(points);

        EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(x_bias));
        EXPECT_NO_FATAL_FAILURE(test_lip.solve());
        EXPECT_TRUE(test_lip.validate_results());
        Analysis test_analysis_xbias(test_lip);
        test_analysis_xbias.print_data();

        EXPECT_NO_FATAL_FAILURE(test_lip.reset());
        EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(least));
        EXPECT_NO_FATAL_FAILURE(test_lip.solve());
        EXPECT_TRUE(test_lip.validate_results());
        Analysis test_analysis_least(test_lip);
        test_analysis_least.print_data();

    }


    TEST_F(AnalysisTest, TestHMRandom) {
        int count = 15;
        std::random_device rd;
        std::mt19937 bitgen(rd());
        std::uniform_real_distribution<double> distrib(-1.0, 1.0);
        vector<Point> points;
        for (int loop = 0; loop < count; loop++) {
            double x = distrib(bitgen);
            double y = distrib(bitgen);
            points.push_back(Point({x, y}));
            cout << "Point" << loop << ": (" << x << ", " << y << " )\n";
        }

        LinearInterpolationProblem test_lip(points);

        EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(hm));
        EXPECT_NO_FATAL_FAILURE(test_lip.solve());
        EXPECT_TRUE(test_lip.validate_results());
        Analysis test_analysis_hm(test_lip);
        test_analysis_hm.print_data();
    }

}