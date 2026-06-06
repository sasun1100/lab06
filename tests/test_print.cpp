#include <sstream>

#include <gtest/gtest.h>
#include <print.hpp>

TEST(PrintTest, WritesPlainString) {
  std::ostringstream os;
  print("hello", os);
  EXPECT_EQ(os.str(), "hello");
}

TEST(PrintTest, WritesEmptyString) {
  std::ostringstream os;
  print("", os);
  EXPECT_EQ(os.str(), "");
}

TEST(PrintTest, AppendsConsecutiveWrites) {
  std::ostringstream os;
  print("foo", os);
  print("bar", os);
  EXPECT_EQ(os.str(), "foobar");
}

TEST(PrintTest, KeepsSpecialCharacters) {
  std::ostringstream os;
  print("a\nb\tc", os);
  EXPECT_EQ(os.str(), "a\nb\tc");
}
