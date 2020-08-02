//
// Created by Kirk Haller on 7/9/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_POLYNOMIAL_H
#define MULTIVARIATE_POLYNOMIALS_POLYNOMIAL_H

#include <string>
#include "absl/container/btree_map.h"
#include "absl/strings/string_view.h"
#include "utilities/kbnsum.h"
#include "Multiindex.h"
#include "point.h"


#define d_polynomial_coefficient_tol 0.00000000001
#define coefficient_t absl::btree_map<Multi_index, double>

struct monomial_term {
    Multi_index exponent;
    double coefficient;
};

class Polynomial {
private:
    coefficient_t coefficients;
    std::string description;

public:
    Polynomial() {
        coefficients = coefficient_t();
    }

    Polynomial(const Polynomial &poly_in) {
        coefficients = coefficient_t(poly_in.coefficients.begin(), poly_in.coefficients.end());
        description = poly_in.description;
    }

    explicit Polynomial(const coefficient_t &map_in) {
        coefficients = map_in;
        describe();
    }

    Polynomial(const std::string &string_in);

    [[nodiscard]] long get_size() const {
        return coefficients.size();
    }

    std::string get_description() {
        if (description.empty()) {
            describe();
        }
        return description;
    }

    [[nodiscard]] monomial_term leading_term() const;

    [[nodiscard]] int get_degree() const {
        if (coefficients.empty())
            return 0;

        //TODO: Make this the largest, non-zero monomial term's degree.
        return leading_term().exponent.get_degree();
    }

    [[nodiscard]] int dimension() const {
        if (!coefficients.empty()) {
            return coefficients.begin()->first.dimension();
        }
        return 0;
    }

    [[nodiscard]] bool is_zero() const;

    [[nodiscard]] double evaluate(const Point &point) const;

    [[nodiscard]] Polynomial derivative(int direction) const;

    absl::string_view describe();

    // operator assignments
    Polynomial operator+(const Polynomial &rh) const;

    Polynomial operator-(const Polynomial &rh) const;

    Polynomial &operator+=(const Polynomial &rhs);

    Polynomial &operator-=(const Polynomial &rhs);

    Polynomial &operator*=(double scale);

    // A structure to allow get/set using the bracket operator.
    // Thanks to: andreagriffini.com
    struct PolynomialBracketProxy {
        Polynomial &poly;
        Multi_index &index;

        PolynomialBracketProxy(Polynomial &poly_in, Multi_index &index_in) : poly(poly_in), index(index_in) {}

        //This is the get operator
        operator double() {
            return poly.get_coefficient(index);
        }

        //This is the set operator
        double &operator=(const double value) {
            return poly.set_coefficient(index, value);
        }

        // operator functions
        double &operator+=(const double value) {
            KBNSum sum(value);
            double poly_coefficient = poly.get_coefficient(index);
            sum.add(poly_coefficient);
            return poly.set_coefficient(index, sum.value());
        }

        double &operator-=(const double value) {
            KBNSum sum(-value);
            double poly_coefficient = poly.get_coefficient(index);
            sum.add(poly_coefficient);
            return poly.set_coefficient(index, sum.value());
        }

        double &operator*=(const double value) {
            double poly_coefficient = poly.get_coefficient(index);
            return poly.set_coefficient(index, poly_coefficient * value);
        }

        bool operator==(const double rhs_value) const {
            double poly_coefficient = poly.get_coefficient(index);
            return poly_coefficient == rhs_value;
        }

        bool operator!=(const double rhs_value) const {
            double poly_coefficient = poly.get_coefficient(index);
            return poly_coefficient != rhs_value;
        }

        bool operator==(const PolynomialBracketProxy rhs) const {
            double poly_coefficient = poly.get_coefficient(index);
            double rhs_coefficient = rhs.poly.get_coefficient(rhs.index);
            return poly_coefficient == rhs_coefficient;
        }

        bool operator!=(const PolynomialBracketProxy rhs) const {
            double poly_coefficient = poly.get_coefficient(index);
            double rhs_coefficient = rhs.poly.get_coefficient(rhs.index);
            return poly_coefficient != rhs_coefficient;
        }


    };

    PolynomialBracketProxy operator[](Multi_index &index) {
        return PolynomialBracketProxy(*this, index);
    }

    // These functions are added for testing, and to make the implementation of the PolyBracketProxy cleaner.
    double &set_coefficient(const Multi_index &exponent, double coefficient) {
        if (!coefficients.empty() && coefficients.begin()->first.dimension() != exponent.dimension())
            throw std::invalid_argument("Exponent's dimensions mismatch rest of polynomial.");
        coefficients[exponent] = coefficient;
#ifndef NDEBUG
        describe();
#endif
        return coefficients[exponent];
    }

    double get_coefficient(const Multi_index &exponent) const;

private:
    Polynomial multiply_by_monomial(Multi_index &index, double coefficient);
};


#endif //MULTIVARIATE_POLYNOMIALS_POLYNOMIAL_H
