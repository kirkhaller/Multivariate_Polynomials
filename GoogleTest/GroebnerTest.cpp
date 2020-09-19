//
// Created by Kirk Haller on 9/18/20.
//

#include "gtest/gtest.h"
#include "../GroebnerBasis.h"

TEST(groebner_test, empty_set) {
    absl::btree_map<Multi_index, std::unique_ptr<Polynomial>> list_in = {};
    EXPECT_NO_FATAL_FAILURE(GroebnerBasis groebner(list_in));
}

TEST(groebner_test, one_polynomial) {
    Polynomial one("x^(0,0,0)");
    monomial_term lt = one.leading_term();
    absl::btree_map<Multi_index, std::unique_ptr<Polynomial>> list_in = {};
    list_in.emplace(lt.exponent, std::make_unique<Polynomial>(&one));
    EXPECT_NO_FATAL_FAILURE(GroebnerBasis groebner(list_in));
}

TEST(groebner_test, two_polynomial) {
    Polynomial one("x^(1,0,0) - x^(0,0,0)");
    Polynomial two("x^(0,1,0) - x^(0,0,0)");
    monomial_term lt_one = one.leading_term();
    absl::btree_map<Multi_index, std::unique_ptr<Polynomial>> list_in = {};
    list_in.emplace(lt_one.exponent, std::make_unique<Polynomial>(&one));
    monomial_term lt_two = two.leading_term();
    list_in.emplace(lt_two.exponent, std::make_unique<Polynomial>(&two));
    GroebnerBasis groebner(list_in);
    EXPECT_TRUE(one == *(groebner.groebner_list[lt_one.exponent]));
    EXPECT_TRUE(two == *(groebner.groebner_list[lt_two.exponent]));
}

TEST(groeber_test, example_from_cox_little_oshea) {
    Polynomial one("x^(1,0,1) - x^(0,2,0)");
    Polynomial two("x^(3,0,0) - x^(0,0,2)");
    monomial_term lt_one = one.leading_term();
    absl::btree_map<Multi_index, std::unique_ptr<Polynomial>> list_in = {};
    list_in.emplace(lt_one.exponent, std::make_unique<Polynomial>(&one));
    monomial_term lt_two = two.leading_term();
    list_in.emplace(lt_two.exponent, std::make_unique<Polynomial>(&two));

    GroebnerBasis groebner(list_in);
    EXPECT_EQ(groebner.groebner_list.size(), 5);

    Polynomial test("-4.0 x^(2,2,2) + x^(0,6,0) + 3 x^(0,0,5)");
    test = groebner.divided(test);
    EXPECT_TRUE(test.is_zero());
}