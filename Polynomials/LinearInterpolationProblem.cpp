//
// Created by Kirk Haller on 8/3/20.
//

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include "LinearInterpolationProblem.h"

LinearInterpolationProblem::LinearInterpolationProblem(const vector<Point> &points_in) : max_error_degree(-1) {

    // Initialize backup data
    backup_errors = {};

    //setup lagrange data, capturing points.
    //will also translate and scale into cube [-1.1]^d
    for (const auto &point: points_in) {
        lagranges.push_back(make_unique<Lagrange>(point));
    }

    xform.instantiate_from_points(points_in);

    for (auto &lagrange: lagranges) {
        xform.apply(lagrange->point);
    }


    // add the errors
    if (!lagranges.empty()) {
        add_errors_to_degree(0);
    }

    // set up the selectors.
    set_selector_type(selector_type);

}

LinearInterpolationProblem::~LinearInterpolationProblem() {
    for (auto &lagrange: lagranges) {
        // This will safely delete the polynomial
        lagrange->set_polynomial(nullptr);
    }

    errors.clear();
}

int LinearInterpolationProblem::get_degree() const {
    int max_degree = 0;
    for (auto &lagrange: lagranges) {
        if (lagrange->get_degree() <= max_degree) continue;
        max_degree = lagrange->get_degree();
    }
    return max_degree;
}


void LinearInterpolationProblem::update_lagranges_for_new_lagrange(const Lagrange &new_lagrange) {
    Point new_point(new_lagrange.point);
    for (auto &lagrange : lagranges) {
        if (lagrange->is_set() && (lagrange->get_polynomial() != new_lagrange.get_polynomial())) {
            double value = lagrange->evaluate(new_point);
            lagrange->polynomial_ptr->subtract_multiply(value, *new_lagrange.get_polynomial());
        }
    }
}

void LinearInterpolationProblem::update_errors_for_lagrange(const Lagrange &new_lagrange) {
    std::vector<Multi_index> terms_to_erase;
    for (auto &error : errors) {
        double value = error.second->evaluate(new_lagrange.point);
        error.second->subtract_multiply(value, *(new_lagrange.polynomial_ptr));
        if (error.second->is_zero()) {
            terms_to_erase.push_back(error.first);
        }
    }

    for (auto &term : terms_to_erase) {
        errors.erase(term);
    }

#ifndef NDEBUG
    /*
    for (auto &error: errors) {
        double value = error.second->evaluate(new_lagrange.point);
        if (fabs(value) > d_polynomial_value_tol) {
            cout << "\nError for " << error.first.description() << " is " << value << ".\n";
        }
    }
     */
#endif
}

void LinearInterpolationProblem::set_selector_type(LagrangeSelector_e type_in) {

    selector_type = type_in;

    switch (selector_type) {
        case x_bias:
            selector = std::make_unique<XBiasSelector>(&errors);
            cout << "\nUsing X-Bias Selector. \n";
            break;
        case least:
            selector = std::make_unique<LeastSelector>(&errors);
            cout << "\nUsing Least Selector. \n";
            break;
        case hm:
            selector = std::make_unique<HMSelector>(&errors);
            cout << "\nUsing HM Selector. \n";
            break;
    }
}

void LinearInterpolationProblem::add_errors_to_degree(int degree) {

    if (degree > max_error_degree) {
        // Add error terms to minimum degree
        int dimension = int(lagranges.begin()->get()->point.dimension());
        for (int d = max_error_degree + 1; d <= degree; d++) {
            auto collection_at_degree = compute_exponents_to_degree(d, dimension, true);
            for (auto &exponent: collection_at_degree) {
                Polynomial error;
                error[exponent] = 1.0;
                for (auto &lagrange: lagranges) {
                    if (lagrange->is_set()) {
                        // error = error - error(point)*lagrange, which means new error vanishes at point.
                        double value = error.evaluate(lagrange->point);
                        error.subtract_multiply(value, *(lagrange->get_polynomial()));
                    }
                }
                if (!error.is_zero()) {
                    errors[exponent] = make_unique<Polynomial>(error);
                }
            }
        }
    }

    max_error_degree = degree;
}

void LinearInterpolationProblem::reset() {
    for (auto &lagrange: lagranges) {
        lagrange->set_polynomial(nullptr);
    }

    errors.clear();
    max_error_degree = -1;
    add_errors_to_degree(0);
}

void LinearInterpolationProblem::solve() {

    for (auto &lagrange : lagranges) {
        if (!lagrange->is_set()) {
            lagrange->polynomial_ptr = selector->select_lagrange_for_point(lagrange->point, false);
            assert(lagrange->is_set());
#ifndef NDEBUG
            // cout << "  Point: " << lagrange->point.description();
            // cout << "  New Lagrange: " << lagrange->get_polynomial()->describe() << '\n';
#endif
            update_lagranges_for_new_lagrange(*lagrange);
            update_errors_for_lagrange(*lagrange);
            if (errors.empty() || errors.rbegin()->first.get_degree() < lagrange->get_degree() + 1) {
                add_errors_to_degree(lagrange->get_degree() + 1);
            }
        }
    }

#ifndef NDEBUG
    cout << "\nLagranges: \n";
    for (auto &lagrange: lagranges) {
        cout << "Point: " << lagrange->point.description() << ": ";
        cout << lagrange->get_polynomial()->describe() << '\n';
    }

    cout << "\nErrors: \n";
    for (auto &error: errors) {
        cout << error.first.description() << ":  ";
        cout << error.second->describe() << '\n';
    }
#endif
}


bool LinearInterpolationProblem::add_point(const Point &new_point, bool with_undo) {
    bool is_unique_point = true;
    Point xformed_point(new_point);
    xform.apply(xformed_point);

    for (auto &lagrange : lagranges) {
        if (xformed_point == lagrange->point) {
            is_unique_point = false;
        }
    }
    if (is_unique_point) {
        if (with_undo) {
            backup_data_for_undo();
        }
        lagranges.push_back(make_unique<Lagrange>(xformed_point));
        auto lagrange = lagranges.rbegin()->get();
        lagrange->polynomial_ptr = selector->select_lagrange_for_point(lagrange->point, with_undo);
        assert(lagrange->is_set());

        update_lagranges_for_new_lagrange(*lagrange);
        update_errors_for_lagrange(*lagrange);
        if (errors.empty() || errors.rbegin()->first.get_degree() < lagrange->get_degree() + 1) {
            add_errors_to_degree(lagrange->get_degree() + 1);
        }
    }

    return is_unique_point;
}

bool LinearInterpolationProblem::validate_results() const {
    double scaled_tolerance = lagranges.size() * d_polynomial_value_tol;
    double max_error = 0;
    double max_lagrange_error = 0;
    bool return_value = true;
    for (auto &lagrange_points: lagranges) {
        for (auto &error: errors) {
            double error_value = fabs(error.second->evaluate(lagrange_points->point));
            max_error = (error_value > max_error ? error_value : max_error);
            if (error_value > scaled_tolerance) {
                return_value = false;
            }
        }

        for (auto &lagrange: lagranges) {
            double value = lagrange->evaluate(lagrange_points->point);
            if (lagrange->point == lagrange_points->point) {
                value = fabs(1 - value);
            } else {
                value = fabs(value);
            }
            max_lagrange_error = (value > max_lagrange_error ? value : max_lagrange_error);
            if (value > scaled_tolerance) {
                return_value = false;
            }
        }
    }


#ifndef NDEBUG
    cout << "Count:" << lagranges.size() << " Max Lagrange Error: " << max_lagrange_error << "; Max Error: "
         << max_error << '\n';
#endif
    return return_value;
}

void LinearInterpolationProblem::backup_data_for_undo() {
    backup_lagranges.clear();
    for (auto &lagrange : lagranges) {
        backup_lagranges.push_back(make_unique<Lagrange>(lagrange->point));
        Lagrange *backup_lagrange = backup_lagranges.rbegin()->get();
        if (lagrange->is_set()) {
            // Want to make sure we are doing a copy and not a move.
            Polynomial backup_poly(*lagrange->get_polynomial());
            backup_lagrange->set_polynomial(&backup_poly);
        }
    }

    backup_errors.clear();
    for (auto &error : errors) {
        Polynomial error_poly(*error.second);
        backup_errors[error.first] = make_unique<Polynomial>(error_poly);
    }
}

bool LinearInterpolationProblem::undo() {
    if (backup_lagranges.empty() || backup_errors.empty()) {
        return false;
    }
    lagranges = std::move(backup_lagranges);
    backup_lagranges.clear();
    errors = std::move(backup_errors);
    backup_errors.clear();
    return true;
}




