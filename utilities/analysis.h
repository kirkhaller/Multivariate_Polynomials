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

struct analysis_data {
    Multi_index index;
    Polynomial error;
    double min = 0;
    double max = 0;
    double l1 = 0;
    double l2 = 0;
};

class Analysis {
private:
    vector<analysis_data> data;
    vector<Point> sample_locations;
    int dimension;
    double cell_size;

public:
    Analysis(const LinearInterpolationProblem &lip);

    void print_data();

private:
    void prune_2d(const LinearInterpolationProblem &lip);

    static void evaluate_points(analysis_data &poly_data, vector<Point> &samples);

    void evaluate_polynomials();
};


#endif //MULTIVARIATE_POLYNOMIALS_ANALYSIS_H
