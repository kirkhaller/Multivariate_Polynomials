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
        if (get_value(index) < 0) {
            throw std::invalid_argument("Multi Indices must be non-negative.");
        }
    }

    // sum the values of m_index to calculate the degree of the monomial represented by m_index
    degree = std::accumulate(m_index.begin(), m_index.end(), 0);
}

Multi_index::Multi_index(int dimension, int index) {
    if (dimension < index || index < 0) {
        throw std::out_of_range("Index out of bounds.");
    }

    m_index = m_index_t(dimension, 0);
    m_index[index] = 1;
    degree = 1;
}

std::string Multi_index::description() const {

    std::string string_out = "( ";
    for (int index = 0; index < dimension() - 1; index++) {
        string_out += std::to_string(get_value(index));
    }

    string_out += std::to_string(get_value(dimension() - 1)) + " )";

    return string_out;
}

int Multi_index::get_value(const int index) const {
    if (dimension() < index || index < 0) {
        throw std::out_of_range("Index out of bounds.");
    }
    return m_index[index];
}

int Multi_index::get_degree() const {
    return degree;
}

Multi_index Multi_index::operator+(const Multi_index &rhs) const {
    if (dimension() != rhs.dimension()) {
        throw std::invalid_argument("Mismatched Multi_index dimensions.");
    }
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
    if (dimension() != rhs.dimension()) {
        throw std::invalid_argument("Mismatched Multi_index dimensions.");
    }
    m_index_t index_out(m_index);

    for (int dim = 0; dim < dimension(); dim++) {
        index_out[dim] -= rhs.get_value(dim);
        if (index_out[dim] < 0)
            throw std::range_error("don't allow negative multi_indexes.");
    }

    return Multi_index(index_out);
}






