//
// Created by Kirk Haller on 7/10/20.
//

#include "gtest/gtest.h"
#include "../polynomial.h"
#include <vector>

namespace {
    class PolynomialTest : public ::testing::Test {
    public:
        Polynomial poly1d, poly3da, poly3db;
        Multi_index index1d, index_000, index_100, index_010, index_001;
        Multi_index index_200, index_110, index_101, index_020, index_011, index_002;

        void SetUp() override {

            index1d = Multi_index({3});
            index_000 = Multi_index({0, 0, 0});
            index_100 = Multi_index({1, 0, 0});
            index_010 = Multi_index({0, 1, 0});
            index_001 = Multi_index({0, 0, 1});
            index_200 = Multi_index({2, 0, 0});
            index_110 = Multi_index({1, 1, 0});
            index_101 = Multi_index({1, 0, 1});
            index_020 = Multi_index({0, 2, 0});
            index_011 = Multi_index({0, 1, 1});
            index_002 = Multi_index({0, 0, 2});

            poly1d.set_coefficient(index1d, 1);

            poly3da.set_coefficient(index_000, 1);
            poly3da.set_coefficient(index_100, 2);
            poly3da.set_coefficient(index_010, 3);
            poly3da.set_coefficient(index_001, 4);

            poly3db.set_coefficient(index_000, 5);
            poly3db.set_coefficient(index_100, 6);
            poly3db.set_coefficient(index_010, 7);
            poly3db.set_coefficient(index_001, 8);
            poly3db.set_coefficient(index_101, 9);
            poly3db.set_coefficient(index_011, 10);
            poly3db.set_coefficient(index_002, 11);
        }
    };

    TEST_F(PolynomialTest, TestBracketOperators) {
        int size = poly3da.get_size();

        double value = poly3da[index_000];
        EXPECT_EQ(value, 1);
        value = poly3da[index_200];
        EXPECT_EQ(value, 0);
        EXPECT_EQ(poly3da.get_size(), size);
        poly3da[index_200] = 200.0;
        EXPECT_EQ(poly3da[index_200], 200.0);
        EXPECT_EQ(poly3da.get_size(), (double) size + 1);
        EXPECT_ANY_THROW(value = poly3da[index1d]);
        size = poly3da.get_size();
        double test_value = poly3da[index_020];
        EXPECT_EQ(test_value, 0);
        test_value = 20;
        EXPECT_NE((double) poly3da[index_020], test_value);
        EXPECT_EQ(poly3da[index_020], 0);
        EXPECT_EQ(poly3da.get_size(), size);
    }

    TEST_F(PolynomialTest, TestDimensionDegree) {
        Polynomial zero_poly;
        EXPECT_EQ(zero_poly.get_degree(), 0);
        EXPECT_EQ(poly1d.get_degree(), 3);
        EXPECT_EQ(poly3da.get_degree(), 1);
        EXPECT_EQ(poly3db.get_degree(), 2);

        EXPECT_EQ(zero_poly.dimension(), 0);
        EXPECT_EQ(poly1d.dimension(), 1);
        EXPECT_EQ(poly3da.dimension(), 3);
        EXPECT_EQ(poly3db.dimension(), 3);

        Multi_index alpha31(3, 1);
        EXPECT_ANY_THROW(poly1d[alpha31] = 1.0);
    }

    TEST_F(PolynomialTest, TestZeroPolynomial) {
        Polynomial zero_poly;
        EXPECT_TRUE(zero_poly.is_zero());
        zero_poly[index_000] = 0;
        EXPECT_TRUE(zero_poly.is_zero());
        EXPECT_FALSE(poly3da.is_zero());
    }

    TEST_F(PolynomialTest, TestOperators) {

    }

}