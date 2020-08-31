//
// Created by Kirk Haller on 8/27/20.
//

#include "LagrangeSelector.h"


double XBiasSelector::evaluate_point_for_selection(Point &point_in) {
    if (!candidates->empty()) {
        return candidates->begin()->second->evaluate(point_in);
    }
    return LagrangeSelector::evaluate_point_for_selection(point_in);
}

unique_ptr<Polynomial> XBiasSelector::select_lagrange_for_point(Point &point_in) {
    assert(!candidates->empty());
    for (auto &candidate : *candidates) {
        double value = candidate.second->evaluate(point_in);
        if (fabs(value) > d_polynomial_value_tol) {
            Polynomial polynomial_out(*candidate.second);
            polynomial_out *= 1.0 / value;
            return make_unique<Polynomial>(polynomial_out);
        }
    }
    return LagrangeSelector::select_lagrange_for_point(point_in);
}

double LeastSelector::evaluate_point_for_selection(Point &point_in) {
    assert(!candidates->empty());
    int lowest_degree = candidates->begin()->first.get_degree();
    double sum = 0.0;
    for (auto &candidate : *candidates) {
        if (candidate.first.get_degree() == lowest_degree) {
            double value = candidate.second->evaluate(point_in);
            sum += value * value;
        }
    }
    return sum;
}

unique_ptr<Polynomial> LeastSelector::select_lagrange_for_point(Point &point_in) {
    assert(!candidates->empty());
    int lowest_degree = candidates->begin()->first.get_degree();
    int maximum_degree = candidates->rbegin()->first.get_degree();
    for (int d = lowest_degree; d <= maximum_degree; d++) {
        Polynomial polynomial_out;
        double sum = 0;
        for (auto &candidate : *candidates) {
            double value = candidate.second->evaluate(point_in);
            sum += value * value;
            Polynomial term(*candidate.second);
            term *= value;
            polynomial_out += term;
        }
        if (sum > d_polynomial_value_tol) {
            return make_unique<Polynomial>(polynomial_out);
        }
    }

    return LagrangeSelector::select_lagrange_for_point(point_in);
}

double HMSelector::evaluate_point_for_selection(Point &point_in) {
    return LagrangeSelector::evaluate_point_for_selection(point_in);
}

unique_ptr<Polynomial> HMSelector::select_lagrange_for_point(Point &point_in) {
    return LagrangeSelector::select_lagrange_for_point(point_in);
}