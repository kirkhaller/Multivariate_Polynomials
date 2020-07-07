//
// Created by Kirk Haller on 6/25/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_MULTIINDEX_H
#define MULTIVARIATE_POLYNOMIALS_MULTIINDEX_H

#include <cassert>
#include <string>

class Multi_index {
    Multi_index();

    std::string description() const;

public:
    int dimension() const { return -1; };

    int get_value(const int index) const;
};

inline bool operator==(const Multi_index &lhs, const Multi_index &rhs) {
    // Need to implement
    return false;
}

inline bool operator!=(const Multi_index &lhs, const Multi_index &rhs) {
    return !(lhs == rhs);
}

inline bool operator<(const Multi_index &lhs, const Multi_index &rhs) {
    assert(lhs.dimension() == rhs.dimension() && "Mismatched Dimensions.");
    for (int index = 0; index < lhs.dimension(); index++) {
        int lhs_value = lhs.get_value(index);
        int rhs_value = rhs.get_value(index);
        if (lhs_value != rhs_value) {
            return lhs_value < rhs_value;
        }
    }
    return false; // The operators must be equal
}

#endif //MULTIVARIATE_POLYNOMIALS_MULTIINDEX_H
