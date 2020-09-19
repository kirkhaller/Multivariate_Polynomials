//
// Created by Kirk Haller on 9/8/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_GROEBNERBASIS_H
#define MULTIVARIATE_POLYNOMIALS_GROEBNERBASIS_H

#include "absl/container/btree_map.h"
#include "absl/container/btree_set.h"
#include "polynomial.h"
#include "LagrangeSelector.h"
#include <vector>


using namespace absl;
using namespace std;
//

class Criteria {
public:
    shared_ptr<Polynomial> first, second;
    Multi_index lcm, sum;

    Criteria(shared_ptr<Polynomial> one, shared_ptr<Polynomial> two) {
        first = one;
        Multi_index index_one = first->leading_term().exponent;
        second = two;
        Multi_index index_two = second->leading_term().exponent;
        lcm = index_one.least_common_multiple(index_two);
        sum = index_one + index_one;
    }

    [[nodiscard]] bool is_valid() const {
        return !(lcm == sum);
    }

    bool divides(Multi_index &dividend) const {
        return dividend.divides(lcm);
    }
};

inline bool operator<(const Criteria &lhs, const Criteria &rhs) {
    if (*lhs.first == *rhs.first) {
        return lhs.second < rhs.second;
    }
    return lhs.first < rhs.first;
}

inline bool operator==(const Criteria &lhs, const Criteria &rhs) {
    return ((*lhs.first == *rhs.first) && (*lhs.second == *rhs.second));
}

// Computes a reduce groebner basis for ideal generate by the polynomials input. (see, e.g, Cox, Little and Shea)
// This is not a general Groebner basis solver, but intended optimized for the case of interpolation.
// I.e., the ideal of polynomials that vanish on a finite point set.
#define groebner_list_t absl::btree_map<Multi_index, std::shared_ptr<Polynomial>>

class GroebnerBasis {
private:
    vector<shared_ptr<Polynomial>> input_list;
    btree_set<Criteria> leading_term_check;

public:
    groebner_list_t groebner_list;

public:
    explicit GroebnerBasis(btree_map<Multi_index, unique_ptr<Polynomial>> &errors);

    void add_new_polynomial(const shared_ptr<Polynomial> &poly);

    void solve();

    [[nodiscard]] Polynomial divided(const Polynomial &poly_in) const;

    [[nodiscard]] static Polynomial s_polynomial(const Criteria &criteria);

private:
    [[nodiscard]] Polynomial divided_unreduced(const Polynomial &poly_in) const;

    [[nodiscard]] bool check_criteria(const Criteria &criteria) const;

    void reduce();

};

inline bool operator<(const Polynomial &lhs, const Polynomial &rhs) {
    return lhs.leading_term().exponent < rhs.leading_term().exponent;
}

#endif //MULTIVARIATE_POLYNOMIALS_GROEBNERBASIS_H
