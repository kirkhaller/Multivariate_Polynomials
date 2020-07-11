//
// Created by Kirk Haller on 7/10/20.
//

#include "gtest/gtest.h"
#include "../polynomial.h"

namespace {
    class PolynomialTest : public ::testing::Test {
    public:
        Polynomial poly1d, poly3da, poly3db;

        void SetUp() override {
            Multi_index index1d({3});
            Multi_index index_000({0, 0, 0});
            Multi_index index_100({1, 0, 0});
            Multi_index index_010({0, 1, 0});
            Multi_index index_001({0, 0, 1});

            poly1d[index1d] = 1;

            poly3da[index_000] = 1;
            poly3da[index_100] = 2;
            poly3da[index_010] = 3;
            poly3da[index_001] = 4;

            poly3db[index_000] = 5;
            poly3db[index_100] = 6;
            poly3db[index_010] = 7;
            poly3db[index_001] = 8;
            poly3db[index_100 + index_001] = 9;
            poly3db[index_010 + index_001] = 10;
            poly3db[index_001 + index_001] = 11;
        }
    };

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
        EXPECT_DEATH(poly1d[alpha31] = 1.0, "Mismatched dimension");
    }

}