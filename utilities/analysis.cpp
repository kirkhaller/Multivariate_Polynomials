//
// Created by Kirk Haller on 9/28/20.
//

#include "analysis.h"
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include "../Geometry/ConvexHull2D.h"
#include "utilities.h"

#define samples_per_dim 40

Analysis::Analysis(const LinearInterpolationProblem &lip) {
// Set up sample locations
    interpolation_points.reserve(lip.lagranges.size());
    for (auto &lagrange: lip.lagranges) {
        interpolation_points.push_back(lagrange->point);
    }

    dimension = lip.dimension();
    point_t point_template;
    switch (sample_scheme) {
        case GRID:
            create_point_grid(dimension, samples_per_dim, point_template, &sample_locations);
            if (dimension == 2) {
                prune_2d();
            }
            break;
        case RADIAL:
            sample_point_lines();
            break;
    }


    data.reserve(lip.errors.size());
    for (auto &error: lip.errors) {
        analysis_data new_data;
        new_data.index = error.first;
        new_data.error = Polynomial(*error.second);
        new_data.error.clear_zero_terms();
        data.push_back(new_data);
    }

    evaluate_polynomials();
}

void Analysis::prune_2d() {
    assert(dimension == 2);

    ConvexHull2D cv(interpolation_points);

    auto it = remove_if(sample_locations.begin(), sample_locations.end(),
                        [&](const Point &point) { return !cv.point_in_convex_hull(point); });
    sample_locations.erase(it, sample_locations.end());
}


void Analysis::evaluate_points(analysis_data &poly_data, vector<Point> &samples) {
    if (samples.size() == 0) {
        return;
    }

    vector<double> values;
    values.reserve(samples.size());
    for (auto &point : samples) {
        double value = poly_data.error.evaluate(point);
        values.push_back(fabs(value));
    }
    sort(values.begin(), values.end());
    int mid = int(values.size() / 2);
    if (values.size() % 2 == 0) {
        poly_data.median = (values[mid - 1] + values[mid]) / 2;
    } else {
        poly_data.median = values[mid];
    }

    poly_data.mean = accumulate(values.begin(), values.end(),
                                double(0.0), [](double a, double b) { return a + b; }) / values.size();
    poly_data.standard_deviation = sqrt(accumulate(values.begin(), values.end(),
                                                   double(0.0), [&](double a, double b) {
                double diff = b - poly_data.median;
                return a + diff * diff;
            }));
}


void Analysis::evaluate_polynomials() {
    vector<thread> threads;
    threads.reserve(data.size());
    for (auto &poly_data : data) {
        threads.push_back(thread(evaluate_points, ref(poly_data), ref(sample_locations)));
    }
    for (auto &poly_thread : threads) {
        poly_thread.join();
    }
}

void Analysis::print_data() {
    cout << "\n Polynomial Error Analysis \n";
    double max_median = 0;
    double max_mean = 0;
    double max_stddev = 0;
    for (auto &poly_data : data) {
        max_median = max(max_median, poly_data.median);
        max_mean = max(max_mean, poly_data.mean);
        max_stddev = max(max_stddev, poly_data.standard_deviation);
        cout << "\nError: " << poly_data.error.get_description() << "\n";
        cout << poly_data.index.description() << " median: " << poly_data.median << " mean: " << poly_data.mean
             << " Standard Deviation: " << poly_data.standard_deviation << "\n";
    }
    cout << "\nMax Median: " << max_median
         << " Max Mean: " << max_mean
         << " Max Standard Deviation: " << max_stddev << "\n";
}

void Analysis::sample_point_lines() {
    int num_samples = 4;
    double denom_samples = 4.0 * double(num_samples);
    for (auto &point : interpolation_points) {
        for (auto &other_point : interpolation_points) {
            if (point != other_point) {
                for (int sample_number = 1; sample_number < num_samples; sample_number++) {
                    sample_locations.push_back(point.point_on_line(other_point,
                                                                   double(sample_number) / denom_samples));
                }
            }
        }
    }
}


