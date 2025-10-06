//
// Created by Kirk Haller on 7/9/20.
//

#include "absl/strings/string_view.h"
#include "polynomial.h"
#include "../utilities/utilities.h"

Polynomial::Polynomial(const std::string &string_in) {
    if (!parse_polynomial_string(string_in, &coefficients)) {
        throw std::invalid_argument("Invalid String: " + string_in);
    }
    describe();
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

Polynomial Polynomial::multiply_by_monomial(Multi_index &index, double coefficient) const {
    Polynomial poly_out;
    for (const auto &term : coefficients) {
        Multi_index shifted_multi_index = term.first + index;
        poly_out[shifted_multi_index] = coefficient * term.second;
    }
    return poly_out;
}

Polynomial Polynomial::derivative(int direction) const {
    if (get_degree() == 0)
        return {};

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

bool Polynomial::operator==(Polynomial &rhs) const {
    for (auto &term : coefficients) {
        if (fabs(rhs.get_coefficient(term.first) - term.second) > d_polynomial_coefficient_tol) {
            return false;
        }
    }

    for (auto &term : rhs.coefficients) {
        if (fabs(get_coefficient(term.first) - term.second) > d_polynomial_coefficient_tol) {
            return false;
        }
    }

    return true;
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
        if (fabs(term->second) > d_polynomial_coefficient_tol) {
            double value = term->second;
            description += connector + std::to_string(value) + " x^" + (term->first).description();
            if (connector.empty()) {
                connector = " + ";
            }
        }
    }

    return description;
}

void Polynomial::add_multiply(double scalar, const Polynomial &poly_in) {
    for (auto &term: poly_in.coefficients) {
        double current_value = get_coefficient(term.first);
        KBNSum sum(current_value);
        sum.add(scalar * term.second);
        set_coefficient(term.first, sum.value());
    }
}

void Polynomial::subtract_multiply(double scalar, const Polynomial &poly_in) {
    add_multiply(-scalar, poly_in);
}

void Polynomial::clear_zero_terms() {
    erase_if(coefficients, [](const auto &item) {
        auto const&[coefficient, value] = item;
        return fabs(value) < 0.1 * d_polynomial_coefficient_tol;
    });
}

Polynomial Polynomial::divided_by(const Polynomial &poly_in, Polynomial *quotient) const {
    Polynomial remainder = Polynomial(*this);
    if (this->is_zero()) {
        return remainder;
    }

    monomial_term leading_term_in = poly_in.leading_term();
    Multi_index working_term = leading_term().exponent;
    [[maybe_unused]] auto reverse_iterator = remainder.coefficients.find(working_term);
    while (!(working_term < leading_term_in.exponent)) {
        try {
            Multi_index dividing_term = working_term - leading_term_in.exponent;
            double value = reverse_iterator->second / leading_term_in.coefficient;
            if (quotient != nullptr) {
                (*quotient)[dividing_term] = value;
            }
            Polynomial working_poly = poly_in.multiply_by_monomial(dividing_term, value);
            remainder -= working_poly;
        }
        catch (std::range_error &e) {} // If the multi index subtraction is not valid, do nothing.

        // Need to find the term, since the subtraction above may invalidate the iterator
        // The Find operator returns an iterator, but can not compare to rend() so see if we are at the
        // beginning.
        if (working_term == remainder.coefficients.begin()->first) {
            return remainder;
        }
        reverse_iterator = --remainder.coefficients.find(working_term);
        working_term = reverse_iterator->first;
    }
    return remainder;
}

