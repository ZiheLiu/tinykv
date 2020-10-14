//
// Created by liuzihe on 2020/10/14.
//

#include <gtest/gtest.h>

#include "util/slice.h"

namespace tinykv {

TEST(SliceTest, FromStringTest) {
    std::string s = "abc";
    Slice slice(s);
    ASSERT_EQ(s, slice.ToString());
}

TEST(SliceTest, FromCStringTest) {
    const char *s = "abc";
    Slice slice(s);
    ASSERT_EQ(std::string(s), slice.ToString());
}

TEST(SliceTest, FromCharArrayTest) {
    const char *s = "abc";
    Slice slice(s, 2);
    ASSERT_EQ("ab", slice.ToString());
}

}
