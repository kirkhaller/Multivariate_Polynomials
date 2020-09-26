//
// Created by Kirk Haller on 8/3/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_LINEARINTERPOLATIONPROBLEM_H
#define MULTIVARIATE_POLYNOMIALS_LINEARINTERPOLATIONPROBLEM_H

#include "absl/container/btree_map.h"
#include <cassert>
#include "GroebnerBasis.h"
#include "Lagrange.h"
#include "LagrangeSelector.h"
#include "../Geometry/point.h"
#include "polynomial.h"
#include "../Geometry/transform.h"
#include <utility>
#include <vector>

using namespace absl;
using namespace std;

enum LagrangeSelector_e {
    x_bias, least, hm
};

#define error_map_t btree_map<Multi_index, unique_ptr<Polynomial>>

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
    error_map_t errors;
    int max_error_degree;

    // To address tolerance concerns, will keep data within [-1,1]^d cube.
    Transform xform;

    // Data for backup
    vector<unique_ptr<Lagrange>> backup_lagranges;
    error_map_t backup_errors; // These could be reconstructed if need be.


public:
    // Will set up the interpolation problem, assumptions:
    //  - the points are unique (not checked, may fail).
    //  - the points are of the same dimension (not check, will fail).
    // default solve mode is x-bias.
    explicit LinearInterpolationProblem(const vector<Point> &points_in);

    ~LinearInterpolationProblem();

    // inquiry functions
    //
    [[nodiscard]] int dimension() const {
        if (lagranges.empty())
            return -1;

        return lagranges.begin()->get()->point.dimension();
    }

    // returns that highest degree of the lagrange polynomials.
    [[nodiscard]] int get_degree() const;

    void set_selector_type(LagrangeSelector_e type_in);

    // These are the main routines
    void reset();

    void solve();

    // returns false if the point is not added.
    // If with_undo is true, the operation can be undone
    bool add_point(const Point &new_point, bool with_undo = false);

    // Returns if the undo was successful.
    bool undo();

    //Validation Methods
    [[nodiscard]] bool validate_results() const;

private:
    // Internal construction mechanism.

    void update_lagranges_for_new_lagrange(const Lagrange &new_lagrange);

    // degree 0 will mean return lowest degree available.
    void add_errors_to_degree(int degree);

    void update_errors_for_lagrange(const Lagrange &new_lagrange);

    void backup_data_for_undo();

};


#endif //MULTIVARIATE_POLYNOMIALS_LINEARINTERPOLATIONPROBLEM_H
