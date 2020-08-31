//
// Created by Kirk Haller on 8/3/20.
//

#include <cassert>
#include <memory>
#include "LinearInterpolationProblem.h"

LinearInterpolationProblem::LinearInterpolationProblem(const vector<Point> &points_in) {

    for (const auto &point: points_in) {
        lagranges.push_back(make_unique<Lagrange>(point));
    }
}

LinearInterpolationProblem::~LinearInterpolationProblem() {
    for (auto &lagrange: lagranges) {
        // This will safely delete the polynomial
        lagrange->set_polynomial(nullptr);
    }

    for (auto &error: errors) {
        delete error.second;
    }
}

int LinearInterpolationProblem::get_degree() const {
    int max_degree = 0;
    for (auto &lagrange: lagranges) {
        if (lagrange->get_degree() <= max_degree) continue;
        max_degree = lagrange->get_degree();
    }
    return max_degree;
}

void LinearInterpolationProblem::update_errors_for_lagrange(const Lagrange &new_lagrange) {
    std::vector<Multi_index> indices_to_remove;
    for (auto &error: errors) {
        double value = error.second->evaluate(new_lagrange.point);
        Polynomial loop_polynomial = *new_lagrange.polynomial_ptr;
        loop_polynomial *= value;
        *(error.second) -= loop_polynomial;
        if (error.second->is_zero()) {
            indices_to_remove.push_back(error.first);
        }
    }

    for (auto &index: indices_to_remove) {
        errors.erase(index);
    }

#ifndef NDEBUG
    for (auto &error: errors) {
        double value = error.second->evaluate(new_lagrange.point);
        assert(fabs(value) < d_polynomial_value_tol);
    }
#endif
}

void LinearInterpolationProblem::set_selector_type(LagrangeSelector_e type_in) {
    selector_type = type_in;

    switch (selector_type) {
        case x_bias:
            selector = std::make_unique<XBiasSelector>(&errors);
            break;
        case least:
            selector = std::make_unique<LeastSelector>(&errors);
            break;
        case hm:
            selector = std::make_unique<HMSelector>(&errors);
            break;
    }
}


vector<Polynomial *> LinearInterpolationProblem::get_errors_for_degree(int degree) {
    vector<Polynomial *> vector_out;

    int maximum_degree = (errors.empty() ? -1 : errors.rbegin()->first.get_degree());

    if (degree > maximum_degree) {
        // Add error terms to minimum degree
        int dimension = int(lagranges.begin()->get()->point.dimension());
        for (int d = maximum_degree + 1; d <= degree; d++) {
            auto collection_at_degree = compute_exponents_to_degree(d, dimension, true);
            for (auto &exponent: collection_at_degree) {
                Polynomial error;
                error[exponent] = 1.0;
                for (auto &lagrange: lagranges) {
                    if (lagrange->is_set()) {
                        // error = error - error(point)*lagrange, which means new error vanishes at point.
                        double value = error.evaluate(lagrange->point);
                        Polynomial correction = (*(lagrange->get_polynomial()));
                        correction *= value;
                        error -= correction;
                    }
                }
                errors[exponent] = new Polynomial(error);
            }
        }
    }

    for (auto &error: errors) {
        if (error.first.get_degree() == degree) {
            vector_out.push_back(error.second);
        }
    }

    return vector_out;
}



