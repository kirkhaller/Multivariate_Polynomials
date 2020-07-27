//
// Created by Kirk Haller on 7/9/20.
//

#include <iostream>
#include "polynomial.h"

KBNSum::KBNSum(double value_in) : sum(value_in), error_sum(0.0) {}

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
    return !std::any_of(coefficients.begin(), coefficients.end(),
                        [](const auto &term) { return abs(term.second) > d_polynomial_coefficient_tol; });

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

Polynomial Polynomial::multiply_by_monomial(Multi_index &index, double coefficient) {
    Polynomial poly_out;
    for (const auto &term : coefficients) {
        Multi_index shifted_multi_index = term.first + index;
        poly_out[shifted_multi_index] = coefficient * term.second;
    }
    return poly_out;
}

Polynomial Polynomial::derivative(int direction) const {
    if (get_degree() == 0)
        return Polynomial();

    Polynomial derivative;
    Multi_index dir(coefficients.end()->first.dimension(), direction);
    for (const auto &term: coefficients) {
        try {
            Multi_index shifted_multi_index = term.first - dir;
            derivative[shifted_multi_index] = term.second * term.first.get_value(direction);
        }
        catch (std::range_error &e) {} // If the multi index subtraction is not valid, do nothing.
    }
    return derivative;
}

double Polynomial::get_coefficient(const Multi_index &exponent) const {
    auto found = coefficients.find(exponent);
    if (found != coefficients.end()) {
        // Added to make debugging easier.
        double value = found->second;
        return value;
    }
    // Added to make debugging easier.
    return 0;
}

Polynomial Polynomial::operator+(const Polynomial &rhs) const {
    Polynomial poly_out(*this);

    for (const auto &term: rhs.coefficients) {
        Multi_index exponent(term.first);
        poly_out[exponent] += term.second;
    }

    return poly_out;
}

Polynomial Polynomial::operator-(const Polynomial &rhs) const {
    Polynomial poly_out(*this);

    for (const auto &term: rhs.coefficients) {
        Multi_index exponent(term.first);
        poly_out[exponent] -= term.second;
    }

    return poly_out;
}

Polynomial &Polynomial::operator+=(const Polynomial &rhs) {
    for (const auto &term: rhs.coefficients) {
        KBNSum sum(term.second);
        double existing_value = get_coefficient(term.first);
        sum.add(existing_value);
        set_coefficient(term.first, sum.value());
    }

    return *this;
}

Polynomial &Polynomial::operator-=(const Polynomial &rhs) {
    for (const auto &term: rhs.coefficients) {
        double existing_value = get_coefficient(term.first);
        KBNSum sum(existing_value);
        sum.add(-term.second);
        set_coefficient(term.first, sum.value());
    }

    return *this;
}

Polynomial &Polynomial::operator*=(double value) {
    for (const auto &term: coefficients) {
        set_coefficient(term.first, value * term.second);
    }

    return *this;
}

