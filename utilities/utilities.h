//
// Created by Neeza Thandi on 7/26/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_UTILITIES_H
#define MULTIVARIATE_POLYNOMIALS_UTILITIES_H

#include <algorithm>
#include "../Multiindex.h"
#include "../polynomial.h"

std::vector<std::string> split_string(std::vector<std::string> to_split, std::string delim,
                                      const bool drop_delim);

//Multi_index string_to_multi (std::string desc_str);

Polynomial string_to_poly(std::string desc_str);

bool parse_multiindex_string(const std::string &string_in, m_index_t *exponent_out);

bool parse_polynomial_string(const std::string &string_in, coefficient_t *term_map_out);

#endif //MULTIVARIATE_POLYNOMIALS_UTILITIES_H
