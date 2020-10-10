//
// Created by Kirk Haller on 9/28/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_ANALYSIS_H
#define MULTIVARIATE_POLYNOMIALS_ANALYSIS_H

#include "../Geometry/point.h"
#include "../Polynomials/LinearInterpolationProblem.h"
#include "../Polynomials/Multiindex.h"
#include "../Polynomials/polynomial.h"
#include <vector>

using namespace std;

enum sample_scheme_e {
    GRID, RADIAL
};

struct analysis_data {
    Multi_index index;
    Polynomial error;
    double median = 0;
    double mean = 0;
    double standard_deviation = 0;
};

class Analysis {
private:
    vector<analysis_data> data;
    vector<Point> interpolation_points;
    vector<Point> sample_locations;
    int dimension;
    sample_scheme_e sample_scheme = RADIAL;

public:
    Analysis(const LinearInterpolationProblem &lip);

    void print_data();

private:
    void prune_2d();

    void sample_point_lines();

    static void evaluate_points(analysis_data &poly_data, vector<Point> &samples);

    void evaluate_polynomials();
};


#endif //MULTIVARIATE_POLYNOMIALS_ANALYSIS_H
