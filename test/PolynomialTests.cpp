//
// Created by Kirk Haller on 7/10/20.
//

#include <gtest/gtest.h>
#include "../Geometry/point.h"
#include "../Polynomials/polynomial.h"

namespace {
    class PolynomialTest : public ::testing::Test {
    public:
        Polynomial poly1d, poly3da, poly3db;
        Multi_index index1d, index_000, index_100, index_010, index_001;
        Multi_index index_200, index_110, index_101, index_020, index_011, index_002;

        void SetUp() override {

            index1d = Multi_index(m_index_t({3}));
            index_000 = Multi_index(m_index_t({0, 0, 0}));
            index_100 = Multi_index(m_index_t({1, 0, 0}));
            index_010 = Multi_index(m_index_t({0, 1, 0}));
            index_001 = Multi_index(m_index_t({0, 0, 1}));
            index_200 = Multi_index(m_index_t({2, 0, 0}));
            index_110 = Multi_index(m_index_t({1, 1, 0}));
            index_101 = Multi_index(m_index_t({1, 0, 1}));
            index_020 = Multi_index(m_index_t({0, 2, 0}));
            index_011 = Multi_index(m_index_t({0, 1, 1}));
            index_002 = Multi_index(m_index_t({0, 0, 2}));

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

    TEST_F(PolynomialTest, TestConstruction) {
        Polynomial test_polynomial("2.0 x^(1,1) - 1.0 x^(0,1) + 10 x^(0,0)");
        Multi_index index_11(m_index_t({1, 1}));
        Multi_index index_10(m_index_t({0, 1}));
        Multi_index index_00(m_index_t({0, 0}));
        EXPECT_TRUE(test_polynomial.get_size() == 3);
        EXPECT_EQ(test_polynomial[index_11], 2.0);
        EXPECT_EQ(test_polynomial[index_10], -1.0);
        EXPECT_EQ(test_polynomial[index_00], 10.0);
        EXPECT_NO_THROW(Polynomial("2.0 x^(1,1) + + -1.0 x^(0,1) +  + 10 x^(0,0)"));
        // The following case should be handled, but current is not.
        EXPECT_ANY_THROW(Polynomial("2.0 x^(1,1) + 1.0 x^(0,1) +  + 10"));
    }

    TEST_F(PolynomialTest, TestBracketOperators) {
        long size_a = poly3da.get_size();
        long size_b = poly3db.get_size();

        EXPECT_EQ((double) poly3db[index_100], 6);
        double value = poly3da[index_000];
        EXPECT_EQ(value, 1);
        value = poly3db[index_200];
        EXPECT_EQ(value, 0);
        EXPECT_EQ(poly3db.get_size(), size_b);
        poly3db[index_200] = 200.0;
        EXPECT_EQ(poly3db[index_200], 200.0);
        EXPECT_EQ(poly3db.get_size(), (double) size_b + 1);
        EXPECT_ANY_THROW(value = poly3da[index1d]);

        double test_value = poly3da[index_020];
        EXPECT_EQ(test_value, 0);
        test_value = 20;
        EXPECT_NE(poly3da[index_020], test_value);
        EXPECT_EQ(poly3da[index_020], 0);
        EXPECT_EQ(poly3da.get_size(), size_a);
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
        Polynomial test_sum = poly3da + poly3db;
        EXPECT_EQ((double) test_sum[index_000], poly3da[index_000] + poly3db[index_000]);
        EXPECT_EQ((double) test_sum[index_001], poly3da[index_001] + poly3db[index_001]);
        EXPECT_EQ((double) test_sum[index_010], poly3da[index_010] + poly3db[index_010]);
        EXPECT_EQ((double) test_sum[index_100], poly3da[index_100] + poly3db[index_100]);
        EXPECT_EQ((double) test_sum[index_200], poly3da[index_200] + poly3db[index_200]);
        EXPECT_EQ((double) test_sum[index_110], poly3da[index_110] + poly3db[index_110]);
        EXPECT_EQ((double) test_sum[index_101], poly3da[index_101] + poly3db[index_101]);
        EXPECT_EQ((double) test_sum[index_020], poly3da[index_020] + poly3db[index_020]);
        EXPECT_EQ((double) test_sum[index_011], poly3da[index_011] + poly3db[index_011]);
        EXPECT_EQ((double) test_sum[index_002], poly3da[index_002] + poly3db[index_002]);

        Polynomial test_minus = poly3da - poly3db;
        EXPECT_EQ((double) test_minus[index_000], poly3da[index_000] - poly3db[index_000]);
        EXPECT_EQ((double) test_minus[index_001], poly3da[index_001] - poly3db[index_001]);
        EXPECT_EQ((double) test_minus[index_100], poly3da[index_100] - poly3db[index_100]);
        EXPECT_EQ((double) test_minus[index_010], poly3da[index_010] - poly3db[index_010]);
        EXPECT_EQ((double) test_minus[index_200], poly3da[index_200] - poly3db[index_200]);
        EXPECT_EQ((double) test_minus[index_110], poly3da[index_110] - poly3db[index_110]);
        EXPECT_EQ((double) test_minus[index_101], poly3da[index_101] - poly3db[index_101]);
        EXPECT_EQ((double) test_minus[index_020], poly3da[index_020] - poly3db[index_020]);
        EXPECT_EQ((double) test_minus[index_011], poly3da[index_011] - poly3db[index_011]);
        EXPECT_EQ((double) test_minus[index_002], poly3da[index_002] - poly3db[index_002]);

        test_minus += poly3db;
        EXPECT_EQ((double) test_minus[index_000], (double) poly3da[index_000]);
        EXPECT_EQ((double) test_minus[index_100], (double) poly3da[index_100]);
        EXPECT_EQ((double) test_minus[index_010], (double) poly3da[index_010]);
        EXPECT_EQ((double) test_minus[index_001], (double) poly3da[index_001]);
        EXPECT_EQ((double) test_minus[index_200], (double) poly3da[index_200]);
        EXPECT_EQ((double) test_minus[index_110], (double) poly3da[index_110]);
        EXPECT_EQ((double) test_minus[index_101], (double) poly3da[index_101]);
        EXPECT_EQ((double) test_minus[index_020], (double) poly3da[index_020]);
        EXPECT_EQ((double) test_minus[index_011], (double) poly3da[index_011]);
        EXPECT_EQ((double) test_minus[index_002], (double) poly3da[index_002]);

        test_sum -= poly3db;
        EXPECT_EQ((double) test_sum[index_000], (double) poly3da[index_000]);
        EXPECT_EQ((double) test_sum[index_100], (double) poly3da[index_100]);
        EXPECT_EQ((double) test_sum[index_010], (double) poly3da[index_010]);
        EXPECT_EQ((double) test_sum[index_001], (double) poly3da[index_001]);
        EXPECT_EQ((double) test_sum[index_200], (double) poly3da[index_200]);
        EXPECT_EQ((double) test_sum[index_110], (double) poly3da[index_110]);
        EXPECT_EQ((double) test_sum[index_101], (double) poly3da[index_101]);
        EXPECT_EQ((double) test_sum[index_020], (double) poly3da[index_020]);
        EXPECT_EQ((double) test_sum[index_011], (double) poly3da[index_011]);
        EXPECT_EQ((double) test_sum[index_002], (double) poly3da[index_002]);

        test_sum *= 2;
        EXPECT_EQ((double) test_sum[index_000], 2 * poly3da[index_000]);
        EXPECT_EQ((double) test_sum[index_100], 2 * poly3da[index_100]);
        EXPECT_EQ((double) test_sum[index_010], 2 * poly3da[index_010]);
        EXPECT_EQ((double) test_sum[index_001], 2 * poly3da[index_001]);
        EXPECT_EQ((double) test_sum[index_200], 2 * poly3da[index_200]);
        EXPECT_EQ((double) test_sum[index_110], 2 * poly3da[index_110]);
        EXPECT_EQ((double) test_sum[index_101], 2 * poly3da[index_101]);
        EXPECT_EQ((double) test_sum[index_020], 2 * poly3da[index_020]);
        EXPECT_EQ((double) test_sum[index_011], 2 * poly3da[index_011]);
        EXPECT_EQ((double) test_sum[index_002], 2 * poly3da[index_002]);

    }

    TEST_F(PolynomialTest, TestPolynomialEvaluation) {
        Point point1d_0({0});
        Point point1d_1({1});
        Point point1d_2({2});
        EXPECT_EQ(poly1d.evaluate(point1d_0), 0);
        EXPECT_EQ(poly1d.evaluate(point1d_1), 1);
        EXPECT_EQ(poly1d.evaluate(point1d_2), 8);

        Point point3d_000({0, 0, 0});
        Point point3d_100({1, 0, 0});
        Point point3d_010({0, 1, 0});
        Point point3d_001({0, 0, 1});
        EXPECT_EQ(poly3da.evaluate(point3d_000), 1);
        EXPECT_EQ(poly3da.evaluate(point3d_100), 3);
        EXPECT_EQ(poly3da.evaluate(point3d_010), 4);
        EXPECT_EQ(poly3da.evaluate(point3d_001), 5);
    }

    TEST_F(PolynomialTest, TestPolynomialDivide) {
        Polynomial cubic("x^(3,0) + 3 x^(2,1) + 3 x^(1,2) + x^(0,3)");
        Polynomial linear("x^(1,0) + x^(0,1)");

        Polynomial quotient1;
        Polynomial remainder = linear.divided_by(cubic, &quotient1);
        EXPECT_TRUE(remainder == linear);
        EXPECT_TRUE(quotient1.is_zero());

        Polynomial quotient("x^(2,0) + 2.0 x^(1,1) + x^(0,2)");
        Polynomial quotient2;
        remainder = cubic.divided_by(linear, &quotient2);
        quotient2.describe();
        remainder.describe();
        EXPECT_TRUE(remainder.is_zero());
        EXPECT_TRUE(quotient2 == quotient);

        Polynomial new_cubic("x^(3,0) + x^(0,0)");
        Polynomial new_remainder("x^(0,0) - x^(0,3)");
        remainder = new_cubic.divided_by(linear);
        remainder.describe();
        EXPECT_TRUE(remainder == new_remainder);

        remainder = new_cubic;
        remainder = remainder.divided_by(linear);
        EXPECT_TRUE(remainder == new_remainder);
    }

}