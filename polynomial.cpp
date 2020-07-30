//
// Created by Kirk Haller on 7/9/20.
//

#include "absl/strings/string_view.h"
#include "polynomial.h"

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

monomial_term Polynomial::leading_term() const {
    monomial_term term_out;

    for (auto term = coefficients.rbegin(); term != coefficients.rend(); term++) {
        if (abs(term->second) > d_polynomial_coefficient_tol) {
            term_out.coefficient = term->second;
            term_out.exponent = term->first;
            return term_out;
        }
    }
    term_out.coefficient = 0;
    term_out.exponent = Multi_index();
    return term_out;
}

absl::string_view Polynomial::describe() {
    std::string connector;
    description = "";

    for (auto term = coefficients.rbegin(); term != coefficients.rend(); term++) {
        description += connector + std::to_string(term->second) + " x^" + (term->first).description();
        connector = " + ";
    }

    return description;
}

