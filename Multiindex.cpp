//
// Created by Kirk Haller on 6/25/20.
//

#include "Multiindex.h"
#include <cassert>
#include <math.h>
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

std::string Multi_index::description() const {
    assert(dimension() > 0 && "Mismatched Dimensions");

    std::string string_out = "( ";
    for (int index = 0; index < dimension() - 1; index++){
        string_out += std::to_string(get_value(index));
    }

    string_out += std::to_string(get_value(dimension()-1)) + " )";

    return string_out;
}

int Multi_index::get_value(const int index) const {
    assert(index >= 0 && index < dimension() && "Index is out of bounds");
    return m_index[index];
}

int Multi_index::get_degree() const {
    return degree;
}


