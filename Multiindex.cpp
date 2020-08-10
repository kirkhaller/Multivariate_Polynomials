//
// Created by Kirk Haller on 6/25/20.
//

#include <functional>
#include "Multiindex.h"
#include <numeric>
#include "utilities/utilities.h"


Multi_index::Multi_index(std::string str) {
    int begin = 0, end=str.length()-1;
    std::vector<std::string> tokens;

    // remove any whitespace
    str.erase( std::remove_if(str.begin(), str.end(), isspace), str.end());

    // exclude any leading and/or trailing parentheses
    if (str[0] == '(' ) {
        begin = 1;
    }
    if (str[end] == ')') {
        end -= 1;
    }
    str = str.substr(begin, end - begin + 1);

    // split str on "," The resulting vector of strings will contain string representations of the indices
    tokens.push_back(str);  //initialize tokens with the str to split. Required to call split_string
    tokens = split_string(tokens, ",", true);

    // convert each element of tokens to an integer
    int exponent;
    std::vector<int> exponents;
    for (auto &token : tokens) {
        exponent = std::stoi(token);
        exponents.push_back(exponent);
    }

    // TODO: try to use the constructor that uses the m_index_t as a parameter
    // Now set object properties based on values in exponents.
    m_index = m_index_t(exponents);
    m_index.shrink_to_fit();

    // check that values are all non-negative
    for (int index = 0; index < dimension(); index++) {
        if (get_value(index) < 0) {
            throw std::invalid_argument("Multi Indices must be non-negative.");
        }
    }

    // sum the values of m_index to calculate the degree of the monomial represented by m_index
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


void compute_index_values_to_degree(int degree, int dimension, m_index_t &index, std::vector<m_index_t > &indices_out) {
    std::vector<m_index_t > new_indices;
    for (int count = 0; count < degree; count++) {
        m_index_t new_index(index);
        new_index.push_back(count);
        new_indices.push_back(new_index);
    }
    index.push_back(degree);
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

std::vector<Multi_index> compute_exponents_to_degree(int degree, int dimension, bool homogeneous) {
    std::vector<Multi_index> indices;
    std::vector<m_index_t > index_vectors;
    m_index_t working_index;
    working_index.reserve(dimension);

    int working_dimension = (homogeneous ? dimension - 1 : dimension);
    compute_index_values_to_degree(degree, working_dimension, working_index, index_vectors);

    if (homogeneous) {
        for (auto &index : index_vectors) {
            int index_degree = std::accumulate(index.begin(), index.end(), 0);
            assert(index_degree <= degree);
            index.push_back(degree - index_degree);
        }
    }

    for (auto &index: index_vectors) {
        Multi_index new_index(index);
        indices.push_back(new_index);
    }

    return indices;
}
