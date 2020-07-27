//
// Created by Kirk Haller on 7/9/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_POLYNOMIAL_H
#define MULTIVARIATE_POLYNOMIALS_POLYNOMIAL_H

#include "absl/container/btree_map.h"
#include "absl/strings/string_view.h"
#include "Multiindex.h"
#include "point.h"


#define d_polynomial_coefficient_tol 0.00000000001
#define coefficient_t absl::btree_map<Multi_index, double>

class KBNSum {
private:
    double sum = 0.0;
    double error_sum = 0.0;

public:
    explicit KBNSum(double value_in);

    void add(double value_in);

    void add(const KBNSum &sum_in) {
        add(sum_in.sum);
        add(sum_in.error_sum);
    }

    [[nodiscard]] double value() const {
        return sum + error_sum;
    }

    [[nodiscard]] double error() const {
        return error_sum;
    }
};

class Polynomial {
private:
    coefficient_t coefficients;
    std::string description;

public:
    Polynomial() {
        coefficients = coefficient_t();
        description = "";
    }

    Polynomial(const Polynomial &poly_in) {
        coefficients = coefficient_t(poly_in.coefficients.begin(), poly_in.coefficients.end());
        description = poly_in.description;
    }

    int get_size() const {
        return coefficients.size();
    }

    int get_degree() const {
        if (coefficients.empty())
            return 0;

        //TODO: Make this the largest, non-zero monomial term's degree.
        return coefficients.rbegin()->first.get_degree();
    }

    int dimension() const {
        if (!coefficients.empty()) {
            return coefficients.begin()->first.dimension();
        }
        return 0;
    }

    bool is_zero() const;

    double evaluate(const Point &point) const;

    Polynomial derivative(int direction) const;

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

    // These functions are added for testing, and to make the implimentation of the PolyBracketProxy cleaner.
    double &set_coefficient(const Multi_index &exponent, double coefficient) {
        if (!coefficients.empty() && coefficients.begin()->first.dimension() != exponent.dimension())
            throw std::invalid_argument("Exponent's dimensions mismatch rest of polynomial.");
        return coefficients[exponent] = coefficient;
    }

    double get_coefficient(const Multi_index &exponent) const;

private:
    Polynomial multiply_by_monomial(Multi_index &index, double coefficient);
};


#endif //MULTIVARIATE_POLYNOMIALS_POLYNOMIAL_H
