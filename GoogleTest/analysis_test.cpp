//
// Created by Kirk Haller on 9/29/20.
//


#include "absl/random/random.h"
#include "gtest/gtest.h"
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
        int count = 70;
        int max_int = INT32_MAX;
        absl::BitGen bitgen;
        vector<Point> points;
        for (int loop = 0; loop < count; loop++) {
            double x = double(absl::Uniform(bitgen, -max_int, max_int)) / double(max_int);
            double y = double(absl::Uniform(bitgen, -max_int, max_int)) / double(max_int);
            double z = double(absl::Uniform(bitgen, -max_int, max_int)) / double(max_int);
            points.push_back(Point({x, y, z}));
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
        int max_int = INT32_MAX;
        absl::BitGen bitgen;
        vector<Point> points;
        for (int loop = 0; loop < count; loop++) {
            double x = double(absl::Uniform(bitgen, -max_int, max_int)) / double(max_int);
            double y = double(absl::Uniform(bitgen, -max_int, max_int)) / double(max_int);
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
