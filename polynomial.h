//
// Created by Kirk Haller on 7/9/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_POLYNOMIAL_H
#define MULTIVARIATE_POLYNOMIALS_POLYNOMIAL_H

#include <map>
#include "Multiindex.h"
#include "point.h"

#define d_polynomial_coefficient_tol 0.00000000001

class KBNSum {
private:
    double sum = 0.0;
    double error_sum = 0.0;

public:
    explicit KBNSum(double value_in);

    KBNSum(const KBNSum &sum_in);

    void add(double value_in);

    void add(const KBNSum sum_in) {
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
    std::map<Multi_index, double> coefficients;

public:
    Polynomial() {
        coefficients = std::map<Multi_index, double>();
    }

    Polynomial(const Polynomial &poly_in) {
        coefficients = std::map<Multi_index, double>(poly_in.coefficients.begin(), poly_in.coefficients.end());
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
    double &operator[](Multi_index index);

    Polynomial operator+(const Polynomial &rhs) const;

private:
    Polynomial multiply_by_monomial(Multi_index index, double coeff);
};


#endif //MULTIVARIATE_POLYNOMIALS_POLYNOMIAL_H
