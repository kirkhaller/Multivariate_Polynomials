//
// Created by Kirk Haller on 7/27/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_KBNSUM_H
#define MULTIVARIATE_POLYNOMIALS_KBNSUM_H

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


#endif //MULTIVARIATE_POLYNOMIALS_KBNSUM_H
