//
// Created by Neeza Thandi on 7/23/20.
//


#include "gtest/gtest.h"
#include <algorithm>
#include "../Polynomials/Multiindex.h"
#include "../Polynomials/polynomial.h"
#include "../utilities/utilities.h"

std::vector<int> buildIndex(std::string s);

namespace {

    TEST(index_test, scratch) {
        std::vector<std::string> tokens1, tokens2, tokens3;
        std::string str = "1x^(3 ,  4,5) + 6.7 x^(8,9,0) - 1.1 x^(0 ,0 ,0 )";
        std::string token;
        std::string delimiter1 = "x^", delimiter2 = "+", delimiter3 = "-";
        size_t pos = 0;

        while ( (pos=str.find(delimiter1)) != std::string::npos) {
            token = str.substr(0,pos);
            tokens1.push_back(token);
            str.erase(0, pos + delimiter1.length());
        }

        if (! str.empty()) {
            tokens1.push_back(str);
        }

       std::cout << "\nTokens found on first split: \n";
        for (auto s = tokens1.begin(); s != tokens1.end(); s++) {
           std::cout << *s << "\n";
       }

        // Split on second delimiter
        for (auto s = tokens1.begin(); s != tokens1.end(); s++){
            pos = (*s).find(delimiter2);
            token = (*s).substr(0,pos);
            tokens2.push_back(token);
            if (pos != std::string::npos) {
                token = (*s).substr(pos, std::string::npos);
                tokens2.push_back(token);
            }
        }

        std::cout << "\nTokens found on second split: \n";
        for (auto s = tokens2.begin(); s != tokens2.end(); s++) {
            std::cout << *s << "\n";
        }

        // Split in third delimiter
        for (auto s = tokens2.begin(); s != tokens2.end(); s++){
            pos = (*s).find(delimiter3);
            token = (*s).substr(0,pos);
            tokens3.push_back(token);
            if (pos != std::string::npos) {
                token = (*s).substr(pos, std::string::npos);
                tokens3.push_back(token);
            }
        }

        std::cout << "\nTokens found on third split: \n";
        for (auto s = tokens3.begin(); s != tokens3.end(); s++) {
            std::cout << *s << "\n";
        }

        // Eliminate all whitespace
        printf("\nTesting getting rid of whitespace\n");

        for (auto s = tokens3.begin(); s != tokens3.end(); s++) {
            (*s).erase( std::remove_if((*s).begin(), (*s).end(), isspace), (*s).end());
            std::cout << *s << "\n";
        }

        // Extract coefficients
        printf("\nExtracting coefficients\n");
        int len = tokens3.size();
        float coeff;
        std::vector<float> coeffs;
        for (int i = 0; i < len/2; i++) {
            coeff = std::stof(tokens3[i*2]);
            coeffs.push_back(coeff);
        }

        // Extract exponents
        std::vector<std::vector<int>> exponents;
        std::vector<int> index;
        /* for (int i = 0; i < len/2; i++) {
            index = std::stof(tokens3[i*2]);
            coeffs.push_back(coeff);
        } */

        std::cout << "\n testing for_each\n";
        std::vector<int> v {0,1,2,3};
        std::for_each(v.begin(), v.end(), [](int &n) {n = n*2;} );
        for (int i= 0; i < v.size(); i++) {
            std::cout << std::to_string(v[i]) << "\n";
        }

        std::string test = "(0,0,1)";
        std::vector<int> exponents_test;
        int index_test;
        std::cout << "\n More testing of substring operations\n";
        std::cout << "test string is: " << test << "\n";
        /* std::cout << test.substr(0, test.size()) << "\n";
        std::cout << test.substr(0, test.size() - 1) << "\n";
        std::cout << test.substr(1, test.size() - 1) << "\n";
        std::cout << test.substr(1, test.size() - 2) << "\n"; */

        test = test.substr(1, test.size() - 2);
        std::cout << "test string is: " << test << "\n";
        while (!test.empty()) {
            pos = test.find(",");
            if (pos != std::string::npos) {
                token = test.substr(0, pos);
            }
            else {
                token = test;
            }
            std::cout << "token is: " << token << "\n";
            index_test = std::stoi(token);
            exponents_test.push_back(index_test);
            test.erase(0, token.size()+1);
            //std::cout << "reduced test string is: " << test << "\n";
        }
        for (int i= 0; i < exponents_test.size(); i++) {
            std::cout << "element " << i << ": " << std::to_string(exponents_test[i]) << "\n";
        }
    };
    TEST(string_conversion_test, test_A) {
        std::string desc_str = "1x^(3 ,  4,5) + 6.7 x^(8,9,0) - 1.1 x^(0 ,0 ,0 )";
        std::vector<std::string> tokens;
        std::string token;
        size_t pos;

        // tokenize desc_str using delimiter "x^"
        tokens.push_back(desc_str);
        tokens = split_string(tokens, "x^", true);
        std::cout << "finished splitting on x^\n";

        // process each (string) element of tokens to split on "+" while retaining the "+" sign
        tokens = split_string(tokens, "+", false);
        std::cout << "finished splitting on +\n";

        // process each (string) element of tokens to split on "-" while retaining the "-" sign
        tokens = split_string(tokens, "-", false);
        std::cout << "finished splitting on -\n";

        // remove whitespace from each element of tokens
        for (auto s = tokens.begin(); s != tokens.end(); s++) {
            (*s).erase( std::remove_if((*s).begin(), (*s).end(), isspace), (*s).end());
        }
        std::cout << "finished removing whitespace\n";

        // print tokens
        for (auto s = tokens.begin(); s != tokens.end(); s++) {
            std::cout << *s << "\n";
        }

    };
    TEST(poly_test, test_B){
        Polynomial poly;
        std::string desc_str = " + x^(3 ,  4,5) + 0.5 x^(8,9,0) - 1.1 x^(0 ,0 ,0 )";
        std::string poly_desc;

        poly = string_to_poly(desc_str);
        poly_desc = poly.get_description();
        std::cout << "Original string: " << desc_str << "\n";
        std::cout << "Generated polynomial: " << poly_desc << "\n";
    };

} //namespace
