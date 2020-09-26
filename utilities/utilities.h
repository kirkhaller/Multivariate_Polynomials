//
// Created by Neeza Thandi on 7/26/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_UTILITIES_H
#define MULTIVARIATE_POLYNOMIALS_UTILITIES_H

#include <algorithm>
#include "../Polynomials/Multiindex.h"
#include "../Geometry/point.h"
#include "../Polynomials/polynomial.h"

bool parse_multiindex_string(const std::string &string_in, m_index_t *exponent_out);

bool parse_polynomial_string(const std::string &string_in, coefficient_t *term_map_out);

void create_point_grid(int dim, int samples_per_dim, point_t &point_template, std::vector<Point> *grid);

#endif //MULTIVARIATE_POLYNOMIALS_UTILITIES_H
