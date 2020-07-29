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
    for (auto s=tokens.begin(); s != tokens.end(); s++) {
        exponent = std::stoi(*s);
        exponents.push_back(exponent);
    }

    // TODO: try to use the constructor that uses the m_index_t as a parameter
    // Now set object properties based on values in exponents.
    m_index = m_index_t(exponents);

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






