//
// Created by Kirk Haller on 6/25/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_MULTIINDEX_H
#define MULTIVARIATE_POLYNOMIALS_MULTIINDEX_H

#include <cassert>
#include <string>
#include <vector>

#define m_index_t std::vector<int>

class Multi_index {

private:
    m_index_t m_index;
    int degree;

public:
    Multi_index() : degree(-1), m_index({}) {};

    explicit Multi_index(std::string str);

    explicit Multi_index(const m_index_t &index_in);

    // Returns the unit vector (0,..,0,1,0,...0) with the 1 in the indicated index.
    Multi_index(int dimension, int index);

    [[nodiscard]] std::string description() const;

    [[nodiscard]] int dimension() const { return static_cast<int>(m_index.size()); };

    [[nodiscard]] int get_value(int index) const;

    [[nodiscard]] int get_degree() const;

    Multi_index operator+(const Multi_index &rhs) const;

    Multi_index operator-(const Multi_index &rhs) const;

    Multi_index &operator=(const Multi_index &rhs) = default;
};

// Comparison operators.
inline bool operator==(const Multi_index &lhs, const Multi_index &rhs) {
    if (lhs.dimension() != rhs.dimension())
        return false;

    for (int index=0; index < lhs.dimension(); index++) {
        if (lhs.get_value(index) != rhs.get_value(index))
            return false;
    }

    return true; // lhs and rhs must be equal
}

inline bool operator!=(const Multi_index &lhs, const Multi_index &rhs) {
    return !(lhs == rhs);
}

inline bool operator<(const Multi_index &lhs, const Multi_index &rhs) {
    if (lhs.dimension() != rhs.dimension()) {
        throw std::invalid_argument("Mismatched multi index Dimensions");
    }

    if (lhs.get_degree() != rhs.get_degree())
        return lhs.get_degree() < rhs.get_degree();

    for (int index = 0; index < lhs.dimension(); index++) {
        int lhs_value = lhs.get_value(index);
        int rhs_value = rhs.get_value(index);
        if (lhs_value != rhs_value) {
            return lhs_value < rhs_value;
        }
    }
    return false; // The operators must be equal so lsh < rhs is false
}

void compute_index_values_to_degree(int degree, int dimension, m_index_t &index, std::vector<m_index_t > &indices_out);

std::vector<Multi_index> compute_exponents_to_degree(int degree, int dimension, bool homogeneous);

#endif //MULTIVARIATE_POLYNOMIALS_MULTIINDEX_H
