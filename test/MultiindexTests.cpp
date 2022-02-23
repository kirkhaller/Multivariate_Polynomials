//
// Created by Kirk Haller on 6/27/20.
//

#include <gtest/gtest.h>
#include <numeric>
#include "../Polynomials/Multiindex.h"

namespace {

    TEST(index_test, index_creation_test) {
        m_index_t initial_value{0, 1, 5};
        m_index_t bad_value{0, -1, 2};
        Multi_index multi_index = Multi_index(initial_value);

        EXPECT_EQ(3, multi_index.dimension());
        EXPECT_EQ(initial_value[0], multi_index.get_value(0));
        EXPECT_EQ(initial_value[1], multi_index.get_value(1));
        EXPECT_EQ(initial_value[2], multi_index.get_value(2));
        EXPECT_EQ(6, multi_index.get_degree());
        EXPECT_ANY_THROW(int value = multi_index.get_value(-1));
        EXPECT_ANY_THROW(int value = multi_index.get_value(4));
        EXPECT_ANY_THROW(Multi_index multi_index2 = Multi_index(bad_value));
        EXPECT_ANY_THROW(Multi_index multiIndex3 = Multi_index(3, 4));
        EXPECT_ANY_THROW(Multi_index multiIndex5 = Multi_index(-1, 4));
        EXPECT_ANY_THROW(Multi_index multiIndex6 = Multi_index(3, -1));
        EXPECT_NO_THROW(Multi_index multiIndex7 = Multi_index(3, 1));
    }

    TEST(index_test, index_comparision_test) {
        Multi_index index_000(m_index_t({0, 0, 0}));
        Multi_index index_100(m_index_t({1, 0, 0}));
        Multi_index index_110(m_index_t({1, 1, 0}));
        Multi_index index_011(m_index_t({0, 1, 1}));
        Multi_index index_002(m_index_t({0, 0, 2}));
        EXPECT_TRUE(index_000 < index_100);
        EXPECT_FALSE(index_100 < index_000);
        EXPECT_TRUE(index_000 < index_110);
        EXPECT_FALSE(index_110 < index_000);
        EXPECT_TRUE(index_100 < index_110);
        EXPECT_FALSE(index_110 < index_100);
        EXPECT_TRUE(index_100 < index_002);
        EXPECT_FALSE(index_002 < index_100);
        EXPECT_TRUE(index_002 < index_011);
        EXPECT_FALSE(index_100 < index_100);
        EXPECT_TRUE(index_100 == index_100);
        EXPECT_FALSE(index_100 != index_100);
        EXPECT_TRUE(index_100 != index_110);
    }

    TEST(index_test, index_operator_test) {
        Multi_index index_000(m_index_t({0, 0, 0}));
        Multi_index index_100(m_index_t({1, 0, 0}));
        Multi_index index_010(m_index_t({0, 1, 0}));
        Multi_index index_110(m_index_t({1, 1, 0}));
        Multi_index index_11(m_index_t({1, 1}));
        EXPECT_EQ(index_000 + index_100, index_100);
        EXPECT_EQ(index_100 - index_000, index_100);
        EXPECT_EQ(index_100 + index_010, index_110);
        EXPECT_ANY_THROW(index_100 - index_010);
        EXPECT_ANY_THROW(index_100 + index_11);
        EXPECT_ANY_THROW(index_100 - index_11);

    }

    TEST(index_test, index_to_degree_test) {
        std::vector<m_index_t > indices_vector1;
        m_index_t new_index1;
        new_index1.reserve(1);
        compute_index_values_to_degree(3, 1, new_index1, indices_vector1);
        EXPECT_EQ(indices_vector1.size(), 4);
        for (auto &vec : indices_vector1) {
            EXPECT_EQ(vec.size(), 1);
            EXPECT_TRUE(vec[0] < 4);
        }

        std::vector<m_index_t > indices_vector2;
        m_index_t new_index2;
        new_index2.reserve(2);
        compute_index_values_to_degree(3, 2, new_index2, indices_vector2);
        EXPECT_EQ(indices_vector2.size(), 10);
        for (auto &vec : indices_vector2) {
            EXPECT_EQ(vec.size(), 2);
            int degree = std::accumulate(vec.begin(), vec.end(), 0);
            EXPECT_TRUE(degree < 4);
        }

        std::vector<Multi_index> test_vector_non_homogeneous =
                compute_exponents_to_degree(3, 2, false);
        EXPECT_EQ(test_vector_non_homogeneous.size(), 10);
        for (auto &exponent: test_vector_non_homogeneous) {
            EXPECT_EQ(exponent.dimension(), 2);
            EXPECT_TRUE(exponent.get_degree() < 4);
        }

        std::vector<Multi_index> test_vector_homogeneous =
                compute_exponents_to_degree(3, 2, true);
        EXPECT_EQ(test_vector_homogeneous.size(), 4);
        for (auto &exponent: test_vector_homogeneous) {
            EXPECT_EQ(exponent.dimension(), 2);
            EXPECT_EQ(exponent.get_degree(), 3);
        }

    }

} //namespace
