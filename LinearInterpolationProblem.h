//
// Created by Kirk Haller on 8/3/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_LINEARINTERPOLATIONPROBLEM_H
#define MULTIVARIATE_POLYNOMIALS_LINEARINTERPOLATIONPROBLEM_H

#include "absl/container/btree_map.h"
#include <cassert>
#include "Lagrange.h"
#include "LagrangeSelector.h"
#include "point.h"
#include "polynomial.h"
#include <utility>
#include <vector>

using namespace absl;
using namespace std;

enum LagrangeSelector_e {
    x_bias, least, hm
};

/*
 * A linear interpolation problem in general takes a set of points with data, and a function space and returns a
 * unique element of the function space that matches the data at the points. Here, the function space is a subset
 * of the space of polynomials. Rather than matching data for a specific function, it returns the lagrange polynomials.
 * Lagrange polynomials are 1 at a specified point, and zero at all others.
 */

class LinearInterpolationProblem {
private:
    // Point processing order matters
    vector<unique_ptr<Lagrange>> lagranges;

    LagrangeSelector_e selector_type = x_bias;
    unique_ptr<LagrangeSelector> selector;
    btree_map<Multi_index, unique_ptr<Polynomial>> errors;
    int max_error_degree;

    // A mechanism for caching evaluation data, and too investigate performance.
    std::vector<Point> evaluation_grid;

    //These point to data owned above
    btree_map<Polynomial *, btree_map<Point *, double>> evaluation_data;


public:
    explicit LinearInterpolationProblem(const vector<Point> &points_in);

    ~LinearInterpolationProblem();

    // inquiry functions
    [[nodiscard]] int dimension() const {
        if (lagranges.empty())
            return -1;

        return lagranges.begin()->get()->point.dimension();
    }

    [[nodiscard]] int get_degree() const;

    void set_selector_type(LagrangeSelector_e type_in);

    // Lagrange Construction Methods

    void update_lagranges_for_new_lagrange(const Lagrange &new_lagrange);

    // degree 0 will mean return lowest degree available.
    void add_errors_to_degree(int degree);

    void update_errors_for_lagrange(const Lagrange &new_lagrange);

    // These are the main routines
    void reset();

    void solve();

    //Validation Methods
    [[nodiscard]] bool valid_results() const;

};


#endif //MULTIVARIATE_POLYNOMIALS_LINEARINTERPOLATIONPROBLEM_H
