//
// Created by Kirk Haller on 7/9/20.
//

#include <iostream>
#include "polynomial.h"

KBNSum::KBNSum(double value_in) : sum(value_in), error_sum(0.0) {}

KBNSum::KBNSum(const KBNSum &sum_in) {
    sum = sum_in.sum;
    error_sum = sum_in.error_sum;
}

void KBNSum::add(double value_in) {
    double new_sum = sum + value_in;
    if (abs(sum) > abs(value_in)) {
        error_sum += ((sum - new_sum) + value_in);
    } else {
        error_sum += ((value_in - new_sum) + sum);
    }
    sum = new_sum;
}

bool Polynomial::is_zero() const {
    if (coefficients.empty()) {
        return true;
    }

    for (const auto &element : coefficients) {
        if (abs(element.second) > d_polynomial_coefficient_tol) {
            return false;
        }
    }

    return true;
}

double Polynomial::evaluate(const Point &point) const {
    KBNSum value_out(0.0);

    for (const auto &term : coefficients) {
        // coefficient * x^(exponent)
        double term_value = term.second * point.power(const_cast<Multi_index &>(term.first));
        value_out.add(term_value);
    }

    return value_out.value();
}

Polynomial Polynomial::multiply_by_monomial(Multi_index index, double coeff) {
    Polynomial poly_out;
    for (const auto &term : coefficients) {
        poly_out[term.first + index] = coeff * term.second;
    }
    return poly_out;
}

Polynomial Polynomial::derivative(int direction) const {
    if (get_degree() == 0)
        return Polynomial();

    Polynomial derivative;
    Multi_index dir(coefficients.end()->first.dimension(), direction);
    for (const auto &term: coefficients) {
        if (term.first.is_valid_for_subtraction(dir)) {
            derivative[term.first - dir] = term.second * term.first.get_value(direction);
        }
    }
    return derivative;
}

double &Polynomial::operator[](Multi_index index) {
    assert((coefficients.empty() || dimension() == index.dimension()) && "Mismatched dimension");
    return coefficients[index];
}

Polynomial Polynomial::operator+(const Polynomial &rhs) const {
    Polynomial poly_out(*this);

    for (const auto &term: rhs.coefficients) {
        poly_out[term.first] += term.second;
    }

    return poly_out;
}
