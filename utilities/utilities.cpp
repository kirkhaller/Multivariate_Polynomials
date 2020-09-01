//
// Created by Neeza Thandi on 7/25/20.
//


#include "absl/strings/str_split.h"
#include "absl/strings/str_join.h"
#include "absl/strings/numbers.h"
#include <algorithm>
#include <string>
#include "utilities.h"

// function to split a string into tokens by a delimiter. The flag drop_delim indicates whether the
// delimiter is to be included in the tokens or not.
// Examples:
// str = "4.2 x^(1,0,0) + 1.2 x^(0,1,0)"; delim="x^"; drop_delim = true =>
//      tokens = ["4.2", "(1,0,0) + 1.2", "(0,1,0)"]
// str = "(1,0,0) + 1.2"; delim="+"; drop_delim = false =>
//      tokens = ["(1,0,0)", "+ 1.2"]

std::vector<std::string> split_string(std::vector<std::string> to_split, const std::string &delim,
                                      const bool drop_delim) {
    std::vector<std::string> tokens;
    std::string token, search_str;
    unsigned int erase_offset = drop_delim * delim.length();

    for (auto &str : to_split) {
        size_t pos = 0;
        unsigned int start_offset = 0;
        while (pos != std::string::npos) {
            //search_str = (*str).substr(start_offset) ;
            pos = str.substr(start_offset).find(delim);
            token = str.substr(0, pos);
            tokens.push_back(token);
            str.erase(0, pos + erase_offset);
            start_offset = (1 - drop_delim) * delim.length();
        }
    }

    return tokens;

}

Polynomial string_to_poly(const std::string &desc_str) {
    std::vector<std::string> tokens;

    // tokenize desc_str using delimiter "x^"
    tokens.push_back(desc_str);
    tokens = split_string(tokens, "x^", true);

    // process each (string) element of tokens to split on "+" while retaining the "+" sign
    tokens = split_string(tokens, "+", false);

    // process each (string) element of tokens to split on "-" while retaining the "-" sign
    tokens = split_string(tokens, "-", false);

    // remove whitespace from each element of tokens
    for (auto &token : tokens) {
        token.erase(std::remove_if(token.begin(), token.end(), isspace), token.end());
    }

    // check case that start of string was "+" or "-" In which case tokens will have a spurious leading ""
    if (tokens[0].empty() && (tokens[1] == "+" || tokens[1] == "-")) {
        tokens.erase(tokens.begin());
    }

    // extract coefficients- they occur in the even numbered positions of tokens
    int len = tokens.size();
    std::string coeff_str;
    std::vector<double> coeffs;
    for (int i = 0; i < len/2; i++) {
        coeff_str = tokens[i*2];
        // if string is "", "+" replace with 1; if string is "-" replace with "-1"; else convert to float
        if (coeff_str.empty() || coeff_str == "+") {
            coeffs.push_back(1.0);
        } else if (coeff_str == "-") {
            coeffs.push_back(-1.0);
        } else {
            coeffs.push_back(std::stof(coeff_str));
        }
    }

    // extract the string representation of the exponents- they occur in the odd numbered positions of tokens
    std::vector<std::string> exponents_str;
    std::vector<Multi_index> exponents;

    int exp_size = ceil(len / 2);
    exponents_str.reserve(exp_size);
    exponents.reserve(exp_size);
    for (int i = 0; i < len / 2; i++) {
        exponents_str.push_back(tokens[i * 2 + 1]);
    }

    // Check that coeffs and exponents have the same length
    assert(coeffs.size() == exponents_str.size());

    // convert each element of exponents_str into a multi-index using constructor
    for (auto &s : exponents_str) {
        Multi_index m_idx(s);
        exponents.push_back(m_idx);
    }

    // create map of multi-index:coeff pairs
    coefficient_t poly_map;
    for (int i = 0; i < coeffs.size(); i++) {
        poly_map[exponents[i]] = coeffs[i];
    }

    // create polynomial based on poly_map
    Polynomial poly(poly_map);
    return poly;

}

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

    // Change any minuses between terms to +-
    working_string_vector = StrSplit(working_string, '-', absl::SkipEmpty());
    working_string = StrJoin(working_string_vector, "+ -");

    //Break into terms: a coefficient, variable and exponent
    working_string_vector = StrSplit(working_string, '+', absl::SkipWhitespace()); //TODO: Write a better predicate

    //Process each term, and add to term_map_out
    vector<string> working_term_vector;

    for (const auto &term: working_string_vector) {
        working_term_vector = StrSplit(term, "x^", SkipEmpty());
        if (working_term_vector.size() > 2) {
            return false;
        }

        //Get the coefficient
        double coefficient;
        if (!SimpleAtod(working_term_vector[0], &coefficient)) {
            return false;
        }

        m_index_t working_exponent_vector;
        if (!parse_multiindex_string(working_term_vector[1], &working_exponent_vector)) {
            return false;
        }

        Multi_index exponent(working_exponent_vector);
        (*term_map_out)[exponent] = coefficient;
    }

    return true;
}




