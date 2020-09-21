//
// Created by Kirk Haller on 9/8/20.
//

#include "GroebnerBasis.h"

using namespace absl;
using namespace std;

bool turn_on_debugging = false;

GroebnerBasis::GroebnerBasis(btree_map<Multi_index, unique_ptr<Polynomial>> &errors) {
    groebner_list = {};

    for (auto &error : errors) {
        shared_ptr<Polynomial> new_poly = make_shared<Polynomial>(*error.second);
        add_new_polynomial(new_poly);
    }

    solve();
    reduce();
}

void GroebnerBasis::add_new_polynomial(const shared_ptr<Polynomial> &poly) {
    poly->clear_zero_terms();
    monomial_term leading_term = poly->leading_term();
    *poly *= 1.0 / leading_term.coefficient;
    if (turn_on_debugging) {
        poly->describe();
    }

    for (auto &g : input_list) {
        Criteria criteria(poly, g);
        if (criteria.is_valid()) {
            leading_term_check.emplace(criteria);
        }
    }
    input_list.push_back(poly);
}

Polynomial GroebnerBasis::divided_unreduced(const Polynomial &poly_in) const {
    Polynomial remainder(poly_in);

    for (auto &g : input_list) {
        remainder = remainder.divided_by(*g);
    }

    if (turn_on_debugging) {
        remainder.describe();
    }
    return remainder;
}

Polynomial GroebnerBasis::divided(const Polynomial &poly_in) const {
    Polynomial remainder(poly_in);

    for (auto g = groebner_list.rbegin(); g != groebner_list.rend(); g++) {
        remainder = remainder.divided_by(*g->second);
    }

    if (turn_on_debugging) {
        remainder.describe();
    }
    return remainder;
}

void GroebnerBasis::solve() {
    while (!leading_term_check.empty()) {
        auto pop = leading_term_check.extract(leading_term_check.begin());
        Criteria working_criteria = pop.value();

        if (!check_criteria(working_criteria)) {
            Polynomial s = s_polynomial(working_criteria);
            s = divided_unreduced(s);
            if (!s.is_zero()) {
                add_new_polynomial(make_shared<Polynomial>(s));
            }
        }
    }
}

bool GroebnerBasis::check_criteria(const Criteria &criteria) const {

    for (auto &g: input_list) {
        if (g != criteria.first && g != criteria.second) {
            Multi_index term = g->leading_term().exponent;
            Criteria test_first(g, criteria.first);
            Criteria test_second(g, criteria.second);
            if (!leading_term_check.contains(test_first) &&
                !leading_term_check.contains(test_second) &&
                term.divides(criteria.lcm)) {
                return true;
            }
        }
    }

    return false;
}

Polynomial GroebnerBasis::s_polynomial(const Criteria &criteria) {
    Polynomial first(*criteria.first);
    Polynomial second(*criteria.second);
    monomial_term first_term = first.leading_term();
    monomial_term second_term = second.leading_term();
    Multi_index first_adjustment = criteria.lcm - first_term.exponent;
    Multi_index second_adjustment = criteria.lcm - second_term.exponent;

    first = first.multiply_by_monomial(first_adjustment, 1.0);
    second = second.multiply_by_monomial(second_adjustment, 1.0);

    return first - second;
}

void GroebnerBasis::reduce() {
    sort(input_list.begin(), input_list.end(), polynomial_compare);
    for (auto &poly : input_list) {
        *poly = divided(*poly);
        if (!poly->is_zero()) {
            monomial_term lt = poly->leading_term();
            poly->describe();
            assert(groebner_list.count(lt.exponent) == 0);
            groebner_list[lt.exponent] = make_unique<Polynomial>(*poly);
        }
    }
}

bool GroebnerBasis::polynomial_compare(shared_ptr<Polynomial> &poly1, shared_ptr<Polynomial> &poly2) {
    Multi_index index1 = poly1->leading_term().exponent;
    Multi_index index2 = poly2->leading_term().exponent;
    return index1 < index2;
}
