//
// Created by Kirk Haller on 8/27/20.
//

#include <iostream>
#include "LagrangeSelector.h"
#define d_selector_tolerance 0.000000001

unique_ptr<Polynomial> XBiasSelector::select_lagrange_for_point(Point &point_in) {
    assert(!candidates->empty());
    for (auto &candidate : *candidates) {
        double value = candidate.second->evaluate(point_in);
        if (fabs(value) > d_selector_tolerance) {
            Polynomial polynomial_out(*candidate.second);
            polynomial_out *= 1.0 / value;
            return make_unique<Polynomial>(polynomial_out);
        }
    }
    return LagrangeSelector::select_lagrange_for_point(point_in);
}

unique_ptr<Polynomial> LeastSelector::select_lagrange_for_point(Point &point_in) {
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

    return LagrangeSelector::select_lagrange_for_point(point_in);
}

unique_ptr<Polynomial> HMSelector::select_lagrange_for_point(Point &point_in) {
    GroebnerBasis groebner(*candidates);

    Polynomial polynomial_out;
    double sum = 0;
    for (auto &poly_set : groebner.groebner_list) {
        //cout << "Groebner: " << poly_set.second->get_description() << "\n";
        double value = poly_set.second->evaluate(point_in);
        sum += value * value;
        polynomial_out.add_multiply(value, *poly_set.second);
    }
    polynomial_out *= 1.0 / sum;
    return make_unique<Polynomial>(polynomial_out);
}
