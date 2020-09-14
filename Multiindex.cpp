//
// Created by Kirk Haller on 6/25/20.
//

#include <algorithm>
#include <functional>
#include "Multiindex.h"
#include <numeric>
#include "utilities/utilities.h"


Multi_index::Multi_index(std::string &str) {
    if (!parse_multiindex_string(str, &m_index)) {
        throw std::invalid_argument("Invalid String: " + str);
    }

    degree = std::accumulate(m_index.begin(), m_index.end(), 0);
}

Multi_index::Multi_index(const m_index_t &index_in) {
    m_index = m_index_t(index_in); // set the values of m_index based index_in
    m_index.shrink_to_fit();

    // that values are all non-negative
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

    m_index = m_index_t(static_cast<unsigned long>(dimension), 0);
    m_index[index] = 1;
    degree = 1;
}

std::string Multi_index::description() const {

    std::string string_out = "(";
    for (int index = 0; index < dimension() - 1; index++) {
        string_out += std::to_string(get_value(index));
        string_out += ", ";
    }

    string_out += std::to_string(get_value(dimension() - 1)) + ")";

    return string_out;
}

int Multi_index::get_value(int index) const {
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

Multi_index Multi_index::least_common_multiple(Multi_index &other) const {
    Multi_index multiindex_out(other);
    for (int index = 0; index < dimension(); index++) {
        multiindex_out.m_index[index] = std::max(other.get_value(index), get_value(index));
    }
    return multiindex_out;
}

bool Multi_index::divides(Multi_index &other) const {
    for (int index = 0; index < dimension(); index++) {
        if (get_value(index) > other.get_value(index)) {
            return false;
        }
    }
    return true;
}


void compute_index_values_to_degree(unsigned long degree, unsigned long dimension, m_index_t &index,
                                    std::vector<m_index_t > &indices_out) {
    std::vector<m_index_t > new_indices;
    for (int count = 0; count < degree; count++) {
        m_index_t new_index(index);
        new_index.push_back(count);
        new_indices.push_back(new_index);
    }
    index.push_back(static_cast<int &&>(degree));
    new_indices.push_back(index);

    if (dimension > 1) {
        for (auto &working_index: new_indices) {
            compute_index_values_to_degree(degree - working_index.back(), dimension - 1,
                                           working_index, indices_out);
        }
    } else {
        indices_out.reserve(indices_out.size() + new_indices.size());
        indices_out.insert(indices_out.end(), new_indices.begin(), new_indices.end());
    }

}

std::vector<Multi_index> compute_exponents_to_degree(unsigned long degree, unsigned long dimension, bool homogeneous) {
    assert(dimension > 0);
    std::vector<Multi_index> indices;
    std::vector<m_index_t > index_vectors;
    m_index_t working_index;
    working_index.reserve(dimension);

    unsigned long working_dimension = (homogeneous ? dimension - 1 : dimension);
    compute_index_values_to_degree(degree, working_dimension, working_index, index_vectors);

    if (homogeneous) {
        for (auto &index : index_vectors) {
            int index_degree = std::accumulate(index.begin(), index.end(), 0);
            assert(index_degree <= degree);
            index.push_back(static_cast<int &&>(degree - index_degree));
        }
    }

    for (auto &index: index_vectors) {
        Multi_index new_index(index);
        indices.push_back(new_index);
    }

    return indices;
}
