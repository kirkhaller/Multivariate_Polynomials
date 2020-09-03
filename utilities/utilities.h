//
// Created by Neeza Thandi on 7/26/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_UTILITIES_H
#define MULTIVARIATE_POLYNOMIALS_UTILITIES_H

#include <algorithm>
#include "../Multiindex.h"
#include "../polynomial.h"

bool parse_multiindex_string(const std::string &string_in, m_index_t *exponent_out);

bool parse_polynomial_string(const std::string &string_in, coefficient_t *term_map_out);

#endif //MULTIVARIATE_POLYNOMIALS_UTILITIES_H
