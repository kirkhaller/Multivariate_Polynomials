//
// Created by Kirk Haller on 9/8/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_GROEBNERBASIS_H
#define MULTIVARIATE_POLYNOMIALS_GROEBNERBASIS_H

#include "absl/container/btree_map.h"
#include "absl/container/btree_set.h"
#include "polynomial.h"
#include <vector>


using namespace absl;
using namespace std;
//

class Criteria {
public:
    int first, second;
    Multi_index lcm, sum;

    Criteria(int a, Multi_index &index_a, int b, Multi_index &index_b) {
        first = min(a, b);
        second = max(a, b);
        lcm = index_a.least_common_multiple(index_b);
        sum = index_a + index_b;
    }

    [[nodiscard]] bool is_valid() const {
        return !(lcm == sum);
    }

    bool divides(Multi_index &dividend) const {
        return dividend.divides(lcm);
    }
};

inline bool operator<(const Criteria &lhs, const Criteria &rhs) {
    if (lhs.first == rhs.first) {
        return lhs.second < rhs.second;
    }
    return lhs.first < rhs.first;
}

inline bool operator==(const Criteria &lhs, const Criteria &rhs) {
    return ((lhs.first == rhs.first) && (lhs.second == rhs.second));
}

// Computes a reduce groebner basis for ideal generate by the polynomials input. (see, e.g, Cox, Little and Shea)
// This is not a general Groebner basis solver, but optimized for the case of interpolation.
// I.e., the ideal of polynomials that vanish on a finite point set.
#define leading_term_polynomial_map_t absl::btree_map<Multi_index, unique_ptr<Polynomial>>

class GroebnerBasis {
private:
    vector<shared_ptr<Polynomial>> input_list;
    btree_set<Criteria> leading_term_check;

public:
    leading_term_polynomial_map_t groebner_list;

public:
    explicit GroebnerBasis(btree_map<Multi_index, unique_ptr<Polynomial>> &errors);

    void add_new_polynomial(const shared_ptr<Polynomial> &poly);

    void solve();

    [[nodiscard]] Polynomial divided(const Polynomial &poly_in) const;

    [[nodiscard]] Polynomial s_polynomial(const Criteria &criteria);

private:
    [[nodiscard]] Polynomial divided_unreduced(const Polynomial &poly_in) const;

    [[nodiscard]] bool check_criteria(const Criteria &criteria) const;

    void reduce();

    static bool polynomial_compare(shared_ptr<Polynomial> &poly1, shared_ptr<Polynomial> &poly2);
};

#endif //MULTIVARIATE_POLYNOMIALS_GROEBNERBASIS_H
