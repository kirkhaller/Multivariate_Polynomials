//
// Created by Kirk Haller on 8/27/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_LAGRANGE_H
#define MULTIVARIATE_POLYNOMIALS_LAGRANGE_H

#include "point.h"
#include "polynomial.h"

#define d_polynomial_value_tol 0.000000001

// A Lagrange Polynomials is a polynomial which is 1.0 at the point specified,
// and zero at all other interpolation points.

struct Lagrange {
    Point point;
    std::unique_ptr<Polynomial> polynomial_ptr;

    explicit Lagrange(Point point_in) : point(std::move(point_in)), polynomial_ptr(nullptr) {}

    [[nodiscard]] bool is_set() const { return (polynomial_ptr != nullptr); }

    [[nodiscard]] Polynomial *get_polynomial() const { return polynomial_ptr.get(); }

    double evaluate(Point &point_in) const {
        if (is_set()) {
            return polynomial_ptr->evaluate(point_in);
        }

        return 0.0;
    }

    // Lagrange assumes ownership of the polynomial.
    void set_polynomial(Polynomial *poly_in) {
        polynomial_ptr = (poly_in == nullptr ? nullptr : std::make_unique<Polynomial>(*poly_in));
#ifndef NDEBUG
        if (is_set()) {
            assert(fabs(1.0 - evaluate(point)) < d_polynomial_value_tol);
        }
#endif
    }

    [[nodiscard]] int get_degree() const {
        if (is_set()) {
            return polynomial_ptr->get_degree();
        }
        return 0;
    }

};


#endif //MULTIVARIATE_POLYNOMIALS_LAGRANGE_H
