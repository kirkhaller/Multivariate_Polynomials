//
// Created by Kirk Haller on 8/3/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_LINEARINTERPOLATIONPROBLEM_H
#define MULTIVARIATE_POLYNOMIALS_LINEARINTERPOLATIONPROBLEM_H

#include "absl/container/btree_map.h"
#include <cassert>
#include "point.h"
#include "polynomial.h"
#include <utility>
#include <vector>

#define d_polynomial_value_tol 0.00000000001
using namespace absl;
using namespace std;

// A Lagrange Polynomials is a polynomial which is 1.0 at the point specified,
// and zero at all other interpolation points.
struct Lagrange {
    Point point;
    Polynomial *polynomial_ptr;

    explicit Lagrange(Point point_in) : point(std::move(point_in)), polynomial_ptr(nullptr) {}

    ~Lagrange() { delete polynomial_ptr; }

    [[nodiscard]] Polynomial *get_polynomial() const { return polynomial_ptr; }

    // Lagrange assumes ownership of the polynomial.
    void set_polynomial(Polynomial *poly_in) {
        delete polynomial_ptr;
        polynomial_ptr = poly_in;
#ifndef NDEBUG
        if (polynomial_ptr != nullptr) {
            assert(fabs(1.0 - polynomial_ptr->evaluate(point)) < d_polynomial_value_tol);
        }
#endif

    }

    [[nodiscard]] int get_degree() const {
        if (polynomial_ptr != nullptr) {
            return polynomial_ptr->get_degree();
        }
        return 0;
    }

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
    vector<Lagrange> lagranges;
    btree_map<Multi_index, Polynomial *> errors;

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

        return lagranges.begin()->point.dimension();
    }

    [[nodiscard]] int get_degree() const;
};


#endif //MULTIVARIATE_POLYNOMIALS_LINEARINTERPOLATIONPROBLEM_H
