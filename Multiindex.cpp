//
// Created by Kirk Haller on 6/25/20.
//

#include <algorithm>
#include <functional>
#include "Multiindex.h"
#include <cassert>
#include <numeric>

Multi_index::Multi_index(const m_index_t &index_in) {
    m_index = m_index_t(index_in); // set the values of m_index based index_in

    // TODO: check that values are all non-negative
    for (int index = 0; index < dimension(); index++) {
        assert(get_value(index) >= 0 && "Indices must be non-negative");
    }

    // sum the values of m_index to calculate the degree of the monomial represented by m_index
    degree = std::accumulate(m_index.begin(), m_index.end(), 0);
}

Multi_index::Multi_index(int dimension, int index) {
    assert(dimension > index && index > 0 && "Bad unit vector request.");

    m_index = m_index_t(dimension, 0);
    m_index[index] = 1;
    degree = 1;
}

std::string Multi_index::description() const {
    assert(dimension() > 0 && "Mismatched Dimensions");

    std::string string_out = "( ";
    for (int index = 0; index < dimension() - 1; index++) {
        string_out += std::to_string(get_value(index));
    }

    string_out += std::to_string(get_value(dimension() - 1)) + " )";

    return string_out;
}

int Multi_index::get_value(const int index) const {
    assert(index >= 0 && index < dimension() && "Index is out of bounds");
    return m_index[index];
}

int Multi_index::get_degree() const {
    return degree;
}

Multi_index Multi_index::operator+(const Multi_index &rhs) const {
    assert(dimension() == rhs.dimension() && "Mismatched Multi_index dimensions.");
    m_index_t index_out(m_index);

    for (int dim = 0; dim < dimension(); dim++) {
        index_out[dim] += rhs.get_value(dim);
    }

    return Multi_index(index_out);
}

bool Multi_index::is_valid_for_subtraction(const Multi_index &rhs) const {
    if (dimension() != rhs.dimension()) {
        return false;
    }

    for (int dim = 0; dim < dimension(); dim++) {
        if (get_value(dim) < rhs.get_value(dim)) {
            return false;
        }
    }
    return true;
}

Multi_index Multi_index::operator-(const Multi_index &rhs) const {
    assert(dimension() == rhs.dimension() && "Mismatched Multi_index dimensions.");
    m_index_t index_out(m_index);

    for (int dim = 0; dim < dimension(); dim++) {
        index_out[dim] -= rhs.get_value(dim);
        assert(index_out[dim] >= 0 && "don't allow negative multi_indexes.");
    }

    return Multi_index(index_out);
}






