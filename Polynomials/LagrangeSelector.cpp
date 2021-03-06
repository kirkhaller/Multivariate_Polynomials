//
// Created by Kirk Haller on 8/27/20.
//

#include <iostream>
#include "LagrangeSelector.h"
#define d_selector_tolerance 0.000000001

unique_ptr<Polynomial> XBiasSelector::select_lagrange_for_point(Point &point_in, bool reuse) {
    assert(!candidates->empty());
    for (auto &candidate : *candidates) {
        double value = candidate.second->evaluate(point_in);
        if (fabs(value) > d_selector_tolerance) {
            Polynomial polynomial_out(*candidate.second);
            polynomial_out *= 1.0 / value;
            return make_unique<Polynomial>(polynomial_out);
        }
    }
    return LagrangeSelector::select_lagrange_for_point(point_in, reuse);
}

unique_ptr<Polynomial> LeastSelector::select_lagrange_for_point(Point &point_in, bool reuse) {
    assert(!candidates->empty());
    int lowest_degree = candidates->begin()->first.get_degree();
    int maximum_degree = candidates->rbegin()->first.get_degree();
    for (int d = lowest_degree; d <= maximum_degree; d++) {
        Polynomial polynomial_out;
        double sum = 0;
        for (auto &candidate : *candidates) {
            if (candidate.first.get_degree() == d) {
                double value = candidate.second->evaluate(point_in);
                sum += value * value;
                polynomial_out.add_multiply(value, *candidate.second);
            }
        }
        if (sum > d_selector_tolerance) {
            polynomial_out *= 1.0 / sum;
            return make_unique<Polynomial>(polynomial_out);
        }
    }

    return LagrangeSelector::select_lagrange_for_point(point_in, reuse);
}

unique_ptr<Polynomial> HMSelector::select_lagrange_for_point(Point &point_in, bool reuse) {
    if (!reuse || groebner == nullptr) {
        groebner = make_unique<GroebnerBasis>(*candidates);
    }

    leading_term_polynomial_map_t *working_list = (groebner->groebner_list.size() == 1 ?
                                                   candidates : &groebner->groebner_list);

    int lowest_degree = working_list->begin()->first.get_degree();
    int maximum_degree = working_list->rbegin()->first.get_degree();

    for (int d = lowest_degree; d <= maximum_degree; d++) {
        Polynomial polynomial_out;
        double sum = 0;
        for (auto &candidate : *working_list) {
            if (candidate.first.get_degree() == d) {
                double value = candidate.second->evaluate(point_in);
                sum += value * value;
                polynomial_out.add_multiply(value, *candidate.second);
            }
        }
        if (sum > d_selector_tolerance) {
            polynomial_out *= 1.0 / sum;
            if (!reuse) {
                groebner = nullptr;
            }
            return make_unique<Polynomial>(polynomial_out);
        }
    }

    if (!reuse) {
        groebner = nullptr;
    }
    return LagrangeSelector::select_lagrange_for_point(point_in, reuse);;
}
