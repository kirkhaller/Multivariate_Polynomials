//
// Created by Neeza Thandi on 7/25/20.
//



#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept> // For std::stoi, std::stod
#include "utilities.h"

using namespace std;

// --- Start of Standard Library Replacements for Abseil ---

// Helper structs to mimic the Abseil API for function calls.
// These allow the original StrSplit calls to work without changes.
struct ByAnyChar {
    const char* delimiters;
    explicit ByAnyChar(const char* delims) : delimiters(delims) {}
};

/**
 * @brief Joins a vector of strings with a separator.
 * @param parts The vector of strings to join.
 * @param separator The string to insert between elements.
 * @return The joined string.
 */
string StrJoin(const vector<string>& parts, const string& separator) {
    string result;
    if (parts.empty()) {
        return result;
    }
    result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        result += separator;
        result += parts[i];
    }
    return result;
}

// Base implementation for splitting by a single character.
// Always skips empty parts as per usage in this file.
vector<string> StrSplitImpl(const string& text, char delimiter) {
    vector<string> result;
    string token;
    stringstream ss(text);
    while (getline(ss, token, delimiter)) {
        if (!token.empty()) {
            result.push_back(token);
        }
    }
    return result;
}

// Base implementation for splitting by a string delimiter.
// Always skips empty parts as per usage in this file.
vector<string> StrSplitImpl(const string& text, const string& delimiter) {
    vector<string> result;
    if (delimiter.empty()) {
        if (!text.empty()) {
           result.push_back(text);
        }
        return result;
    }
    string::size_type start = 0;
    string::size_type end = text.find(delimiter);
    while (end != string::npos) {
        string token = text.substr(start, end - start);
        if (!token.empty()) {
            result.push_back(token);
        }
        start = end + delimiter.length();
        end = text.find(delimiter, start);
    }
    string last_token = text.substr(start);
    if (!last_token.empty()) {
        result.push_back(last_token);
    }
    return result;
}

// Overloads to match the calling syntax in the file.
vector<string> StrSplit(const string& text, char delimiter) {
    return StrSplitImpl(text, delimiter);
}

vector<string> StrSplit(const string& text, const string& delimiter) {
    return StrSplitImpl(text, delimiter);
}

// Overload for splitting by any character in a given set.
vector<string> StrSplit(const string& text, ByAnyChar delims) {
    vector<string> result;
    string::size_type start = 0;
    string::size_type end = text.find_first_of(delims.delimiters, start);
    while (end != string::npos) {
        string token = text.substr(start, end - start);
        if (!token.empty()) {
            result.push_back(token);
        }
        start = end + 1;
        end = text.find_first_of(delims.delimiters, start);
    }
    string last_token = text.substr(start);
    if (!last_token.empty()) {
        result.push_back(last_token);
    }
    return result;
}

// --- End of Standard Library Replacements for Abseil ---


bool parse_multiindex_string(const string &string_in, m_index_t *exponent_out) {
    // validate input
    if (string_in.empty() || !exponent_out) {
        return false;
    }

    //Get the terms
    vector<string> working_string_vector;
    working_string_vector = StrSplit(string_in, ByAnyChar("(,)"));
    for (const auto &exponent_string : working_string_vector) {
        char *endptr;
        int exp_term = std::strtol(exponent_string.c_str(),&endptr, 10);
        if (exp_term == 0 || *endptr != '\0' || (exp_term < 0 && exponent_string.find("-") == string::npos)) {
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
    working_string_vector = StrSplit(string_in, ' ');
    working_string = StrJoin(working_string_vector, "");

    // Special case that the first term is negative
    bool first_term_negative = (working_string[0] == '-');
    // Change any minuses between terms to +-
    working_string_vector = StrSplit(working_string, '-');
    working_string = StrJoin(working_string_vector, "+-");
    if (first_term_negative) {
        working_string = "-" + working_string;
    }

    //Break into terms: a coefficient, variable and exponent
    working_string_vector = StrSplit(working_string, '+'); //TODO: Write a better predicate

    //Process each term, and add to term_map_out
    vector<string> working_term_vector;

    for (const auto &term: working_string_vector) {
        working_term_vector = StrSplit(term, "x^");

        double coefficient;
        if (working_term_vector.size() > 2) {
            return false;
        }

        if (working_term_vector.size() == 1) {
            coefficient = 1.0;
        } else { // size == 2
            //Get the coefficient
            if (working_term_vector[0] == "-") {
                coefficient = -1;
            } else {
                char *endptr;
                int exp_term = std::strtol(working_term_vector[0].c_str(),&endptr, 10);
                if (exp_term != 0  && endptr != working_term_vector[0].c_str())
                {
                    coefficient = exp_term;
                } else
                {
                    return false;
                }
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



