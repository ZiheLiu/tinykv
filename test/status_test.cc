//
// Created by liuzihe on 2020/10/14.
//

#include <gtest/gtest.h>
#include <string>
#include <unordered_map>
#include "util/random.h"

#include "util/status.h"

namespace tinykv {

TEST(StatusTest, OkTest) {
    Status status = Status::OK();
    ASSERT_TRUE(status.ok());
    ASSERT_EQ(status.ToString(), "OK");

    // Copy constructor.
    Status other(status);
    ASSERT_TRUE(other.ok());

    // Assigment operator with itself.
    other = status;
    ASSERT_TRUE(other.ok());
    ASSERT_TRUE(status.ok());

    // Assigment operator with others.
    other = Status::IOError("An IO error");
    ASSERT_TRUE(other.IsIOError());
}

TEST(StatusTest, IOErrorTest) {
    Status status = Status::IOError("An IO error");
    ASSERT_TRUE(status.IsIOError());
    ASSERT_EQ(status.ToString(), "IO error: An IO error");

    // Copy constructor.
    Status other(status);
    ASSERT_TRUE(other.IsIOError());

    // Assigment operator with itself.
    other = status;
    ASSERT_TRUE(other.IsIOError());
    ASSERT_TRUE(status.IsIOError());

    // Assigment operator with others.
    other = Status::OK();
    ASSERT_TRUE(other.ok());

    std::unordered_map<int, int> m;
    m[1] = 1;
    auto it = m.find(1);
    m.erase(it);

}

}
