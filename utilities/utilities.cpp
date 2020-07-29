//
// Created by Neeza Thandi on 7/25/20.
//

#include <algorithm>
#include "utilities.h"

// function to split a string into tokens by a delimiter. The flag drop_delim indicates whether the
// delimiter is to be included in the tokens or not.
// Examples:
// str = "4.2 x^(1,0,0) + 1.2 x^(0,1,0)"; delim="x^"; drop_delim = true =>
//      tokens = ["4.2", "(1,0,0) + 1.2", "(0,1,0)"]
// str = "(1,0,0) + 1.2"; delim="+"; drop_delim = false =>
//      tokens = ["(1,0,0)", "+ 1.2"]

std::vector<std::string> split_string (std::vector<std::string> to_split,  std::string delim,
                                        const bool drop_delim) {
    std::vector<std::string> tokens;
    std::string token = "", search_str;
    size_t pos = 0;
    int erase_offset = drop_delim * delim.length();
    int start_offset = 0;

    for (auto str=to_split.begin(); str != to_split.end(); str++) {
        pos = 0;
        start_offset = 0;
        while (pos != std::string::npos) {
            //search_str = (*str).substr(start_offset) ;
            pos = (*str).substr(start_offset).find(delim);
            token = (*str).substr(0, pos);
            tokens.push_back(token);
            (*str).erase(0, pos + erase_offset);
            start_offset = (1-drop_delim) * delim.length();
        }

        /*
        if (! (*str).empty()) {
            tokens.push_back(*str);
        }
         */
    }

    return tokens;

}

Polynomial string_to_poly (std::string desc_str) {
    std::vector<std::string> tokens;
    std::string token;
    size_t pos;

    // tokenize desc_str using delimiter "x^"
    tokens.push_back(desc_str);
    tokens = split_string(tokens, "x^", true);

    // process each (string) element of tokens to split on "+" while retaining the "+" sign
    tokens = split_string(tokens, "+", false);

    // process each (string) element of tokens to split on "-" while retaining the "-" sign
    tokens = split_string(tokens, "-", false);

    // remove whitespace from each element of tokens
    for (auto s = tokens.begin(); s != tokens.end(); s++) {
        (*s).erase( std::remove_if((*s).begin(), (*s).end(), isspace), (*s).end());
    }

    // check case that start of string was "+" or "-" In which case tokens will have a spurious leading ""
    if (tokens[0] == "" && (tokens[1] == "+" || tokens[1] == "-")) {
        tokens.erase(tokens.begin());
    }

    // extract coefficients- they occur in the even numbered positions of tokens
    int len = tokens.size();
    double coeff;
    std::string coeff_str;
    std::vector<double> coeffs;
    for (int i = 0; i < len/2; i++) {
        coeff_str = tokens[i*2];
        // if string is "", "+" replace with 1; if string is "-" replace with "-1"; else convert to float
        if (coeff_str == "" || coeff_str == "+") {
            coeffs.push_back(1.0);
        }
        else if (coeff_str == "-") {
            coeffs.push_back(-1.0);
        }
        else {
            coeffs.push_back(std::stof(coeff_str));
        }
    }

    // extract the string representation of the exponents- they occur in the odd numbered positions of tokens
    std::vector<std::string> exponents_str;
    std::vector<Multi_index> exponents;
    Multi_index m_idx;

    for (int i = 0; i < len/2; i++) {
        exponents_str.push_back(tokens[i*2 + 1]);
    }

    // TODO: Check that coeffs and exponents have the same length
    assert(coeffs.size() == exponents_str.size());

    // convert each element of exponents_str into a multi-index using constructor
    for (auto s=exponents_str.begin(); s!=exponents_str.end(); s++) {
        m_idx = Multi_index(*s);
        exponents.push_back(m_idx);
    }

    // create map of multi-index:coeff pairs
    coefficient_t poly_map;
    for (int i=0; i<coeffs.size(); i++) {
        poly_map[exponents[i]] = coeffs[i];
    }

    // create polynomial based on poly_map
    Polynomial poly;
    poly = Polynomial(poly_map);
    return poly;

}




