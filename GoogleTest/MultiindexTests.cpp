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

    };

} //namespace
