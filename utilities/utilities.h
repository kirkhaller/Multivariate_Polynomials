//
// Created by Neeza Thandi on 7/26/20.
//

#ifndef MULTIVARIATE_POLYNOMIALS_UTILITIES_H
#define MULTIVARIATE_POLYNOMIALS_UTILITIES_H

#include <algorithm>
#include "../polynomial.h"

std::vector<std::string> split_string (std::vector<std::string> to_split, std::string delim,
        const bool drop_delim);

//Multi_index string_to_multi (std::string desc_str);

Polynomial string_to_poly (std::string desc_str);

#endif //MULTIVARIATE_POLYNOMIALS_UTILITIES_H
