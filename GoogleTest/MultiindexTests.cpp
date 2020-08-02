//
// Created by Kirk Haller on 6/27/20.
//

#include "gtest/gtest.h"
#include "../Multiindex.h"

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
        EXPECT_ANY_THROW(multi_index.get_value(4));
        EXPECT_ANY_THROW(Multi_index multi_index2 = Multi_index(bad_value));

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
    }

} //namespace
