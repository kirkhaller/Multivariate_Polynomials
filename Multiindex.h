//
// Created by Kirk Haller on 6/25/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_MULTIINDEX_H
#define MULTIVARIATE_POLYNOMIALS_MULTIINDEX_H

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

#endif //MULTIVARIATE_POLYNOMIALS_MULTIINDEX_H
