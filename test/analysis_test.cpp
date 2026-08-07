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
        points.reserve(count);
        cout << "\n" << count << " Points:\n";
        for (int loop = 0; loop < count; loop++) {
            point_t interpolation_point;
            interpolation_point.reserve(dimension);
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

        EXPECT_NO_FATAL_FAILURE(test_lip.reset());
        EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(hm));
        EXPECT_NO_FATAL_FAILURE(test_lip.solve());
        EXPECT_TRUE(test_lip.validate_results());
        Analysis test_analysis_hm(test_lip);
        test_analysis_hm.print_data();
    }


    TEST_F(AnalysisTest, TestHMRandom) {
        int count = 35;
        int dimension = 2;

        std::random_device rd;
        std::mt19937 bitgen(rd());
        std::uniform_real_distribution<double> distrib(-1.0, 1.0);
        vector<Point> points;
        points.reserve(count);
        cout << "\n" << count << " Points:\n";
        for (int loop = 0; loop < count; loop++) {
            point_t interpolation_point;
            interpolation_point.reserve(dimension);
            for (int dim = 0; dim < dimension; dim++) {
                interpolation_point.push_back(distrib(bitgen));
            }
            points.emplace_back(interpolation_point);
            cout << "Point" << points.back().description() << "\n";
        }

        if (count < 2)
        {
            return;
        }

        //check how close the points are
        Point point0 = points[0];
        Point point1 = points[1];
        double distance = 100;
        for (int i=0; i<count-1; i++)
        {
            for (int j=i+1; j<count; j++)
            {
                Point test_point = points[i] - points[j];
                double test_distance = test_point.vector_length();
                if (test_distance < distance)
                {
                    distance = test_distance;
                    point0 = points[i];
                    point1 = points[j];
                }
            }
        }
        cout << "\n" << " Minimum distance is " << distance << " between "
             << point0.description() << " and " << point1.description() << "\n";

        LinearInterpolationProblem test_lip(points);

        EXPECT_NO_FATAL_FAILURE(test_lip.set_selector_type(hm));
        EXPECT_NO_FATAL_FAILURE(test_lip.solve());
        EXPECT_TRUE(test_lip.validate_results());
        Analysis test_analysis_hm(test_lip);
        test_analysis_hm.print_data();
    }

}