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
    dimension = lip.dimension();
    point_t point_template;
    create_point_grid(dimension, samples_per_dim, point_template, &sample_locations);
    if (dimension == 2) {
        prune_2d(lip);
    }
    cell_size = pow((2.0 / double(samples_per_dim)), dimension);

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

void Analysis::prune_2d(const LinearInterpolationProblem &lip) {
    assert(dimension == 2);
    vector<Point> interpolation_points;
    for (auto &lagrange : lip.lagranges) {
        interpolation_points.push_back(Point(lagrange->point));
    }

    ConvexHull2D cv(interpolation_points);

    auto it = remove_if(sample_locations.begin(), sample_locations.end(),
                        [&](const Point &point) { return !cv.point_in_convex_hull(point); });
    sample_locations.erase(it, sample_locations.end());
}


void Analysis::evaluate_points(analysis_data &poly_data, vector<Point> &samples) {
    vector<double> values;
    values.reserve(samples.size());
    for (auto &point : samples) {
        double value = poly_data.error.evaluate(point);
        values.push_back(value);
    }
    auto p = max_element(values.begin(), values.end());
    poly_data.max = *p;
    p = min_element(values.begin(), values.end());
    poly_data.min = *p;

    poly_data.l1 = accumulate(values.begin(), values.end(),
                              double(0.0), [](double a, double b) { return a + fabs(b); });
    poly_data.l2 = accumulate(values.begin(), values.end(),
                              double(0.0), [](double a, double b) { return a + b * b; });
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
    for (auto &poly_data : data) {
        poly_data.l1 *= cell_size;
        poly_data.l2 = sqrt(cell_size * poly_data.l2);
    }
}

void Analysis::print_data() {
    cout << "\n Polynomial Error Analysis \n";
    for (auto &poly_data : data) {
        cout << "\nError: " << poly_data.error.get_description() << "\n";
        cout << poly_data.index.description() << " max: " << poly_data.max << " min: " << poly_data.min
             << " L1: " << poly_data.l1 << " L2: " << poly_data.l2 << "\n";
    }
}


