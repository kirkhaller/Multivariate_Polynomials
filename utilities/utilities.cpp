//
// Created by Neeza Thandi on 7/25/20.
//


#include "absl/strings/str_split.h"
#include "absl/strings/str_join.h"
#include "absl/strings/numbers.h"
#include <algorithm>
#include "utilities.h"

using namespace std;
using namespace absl;

bool parse_multiindex_string(const string &string_in, m_index_t *exponent_out) {
    // validate input
    if (string_in.empty() || !exponent_out) {
        return false;
    }

    //Get the terms
    vector<string> working_string_vector;
    working_string_vector = StrSplit(string_in, ByAnyChar("(,)"), SkipWhitespace());
    for (const auto &exponent_string : working_string_vector) {
        int exp_term = 0;
        if (!SimpleAtoi(exponent_string, &exp_term)) {
            return false;
        }
        exponent_out->push_back(exp_term);
    }

    return true;
}

bool parse_polynomial_string(const string &string_in, coefficient_t *term_map_out) {
    // validate input
    if (string_in.empty() || !term_map_out) {
        return false;
    }

    // Working variables
    vector<string> working_string_vector;
    string working_string;

    // Clear extra white space (change "- 1", to "-1")
    working_string_vector = StrSplit(string_in, ' ', absl::SkipEmpty());
    working_string = StrJoin(working_string_vector, "");

    // Special case that the first term is negative
    bool first_term_negative = (working_string[0] == '-');
    // Change any minuses between terms to +-
    working_string_vector = StrSplit(working_string, '-', absl::SkipEmpty());
    working_string = StrJoin(working_string_vector, "+-");
    if (first_term_negative) {
        working_string = "-" + working_string;
    }

    //Break into terms: a coefficient, variable and exponent
    working_string_vector = StrSplit(working_string, '+', absl::SkipWhitespace()); //TODO: Write a better predicate

    //Process each term, and add to term_map_out
    vector<string> working_term_vector;

    for (const auto &term: working_string_vector) {
        working_term_vector = StrSplit(term, "x^", SkipEmpty());

        double coefficient;
        if (working_term_vector.size() > 2) {
            return false;
        } else if (working_term_vector.size() == 1) {
            coefficient = 1.0;
        } else { // size == 2
            //Get the coefficient
            if (working_term_vector[0] == "-") {
                coefficient = -1;
            } else if (!SimpleAtod(working_term_vector[0], &coefficient)) {
                return false;
            }
        }

        m_index_t working_exponent_vector;
        if (!parse_multiindex_string(*working_term_vector.rbegin(), &working_exponent_vector)) {
            return false;
        }

        Multi_index exponent(working_exponent_vector);
        (*term_map_out)[exponent] = coefficient;
    }

    return true;
}

void create_point_grid(int dim, int samples_per_dim, point_t &point_template, std::vector<Point> *grid) {
    if (point_template.size() == dim) {
        Point new_point(point_template);
        grid->push_back(new_point);
        return;
    }
    for (int index = 1; index < samples_per_dim; index++) {
        double value = 2.0 * index / (samples_per_dim + 1) - 1.0;
        point_template.push_back(value);
        create_point_grid(dim, samples_per_dim, point_template, grid);
        point_template.pop_back();
    }

}



