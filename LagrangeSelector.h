//
// Created by Kirk Haller on 8/27/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_LAGRANGESELECTOR_H
#define MULTIVARIATE_POLYNOMIALS_LAGRANGESELECTOR_H

#include "absl/container/btree_map.h"
#include "GroebnerBasis.h"
#include "Lagrange.h"
#include "Multiindex.h"
#include "point.h"
#include "polynomial.h"

// LagrangeSelector would be a protocol in a smalltalk type language (java, objective-c, swift),
// But we will use subclasses to get a similar design pattern

using namespace absl;
using namespace std;

#define leading_term_polynomial_map_t absl::btree_map<Multi_index, unique_ptr<Polynomial>>

class LagrangeSelector {

protected:
    leading_term_polynomial_map_t *candidates;

public:
    explicit LagrangeSelector(leading_term_polynomial_map_t *candidates_in) : candidates(candidates_in) {}

    // Calling function owns the polynomial.
    virtual unique_ptr<Polynomial> select_lagrange_for_point(Point &point_in) {
        assert(true); // Getting here is an error.
        return make_unique<Polynomial>();
    }

};

// XBias Selector uses an existing monomial ordering to select the first (lowest) non-zero monomial as constructor
// for the Lagrange.
class XBiasSelector : public LagrangeSelector {

public:
    explicit XBiasSelector(leading_term_polynomial_map_t *candidates_in) : LagrangeSelector(candidates_in) {}

    unique_ptr<Polynomial> select_lagrange_for_point(Point &point_in) override;
};

// Least Selector using de Boor and Ron's Least Polynomial construction to select the Lagrange.
// In the Least, the "least" terms of the exp^{new point}, which ends up just being the
// \sum_\alpha e_\alpha({new point}) e_\alpha, which e_\alpha is the error for x^\alpha
class LeastSelector : public LagrangeSelector {

public:
    explicit LeastSelector(leading_term_polynomial_map_t *candidates_in) : LagrangeSelector(candidates_in) {}

    unique_ptr<Polynomial> select_lagrange_for_point(Point &point_in) override;
};

// The HM (Haller Mann) selector, follow the least, but uses the groebner basis for the ideal I({points}).
class HMSelector : public LagrangeSelector {

public:
    explicit HMSelector(leading_term_polynomial_map_t *candidates_in) : LagrangeSelector(candidates_in) {}

    unique_ptr<Polynomial> select_lagrange_for_point(Point &point_in) override;
};

#endif //MULTIVARIATE_POLYNOMIALS_LAGRANGESELECTOR_H
