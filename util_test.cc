#include "util.h"

#include <gtest/gtest.h>

TEST(CominationTest, BasicTest) {
  Combination comb;

  // Adding and checking
  EXPECT_TRUE(comb.AddOption("flag1", true));
  EXPECT_TRUE(comb.HasOption("flag1").first);
  EXPECT_TRUE(comb.HasOption("flag1").second);

  // Adding the same option twice
  EXPECT_TRUE(comb.AddOption("flag2", true));
  EXPECT_FALSE(comb.AddOption("flag2", true));

  // Adding multiple options
  EXPECT_TRUE(comb.AddOption("flag3", true));
  EXPECT_TRUE(comb.AddOption("flag4", false));
  EXPECT_TRUE(comb.AddOption("flag5", false));

  EXPECT_TRUE(comb.HasOption("flag3").first);
  EXPECT_TRUE(comb.HasOption("flag3").second);
  EXPECT_TRUE(comb.HasOption("flag4").first);
  EXPECT_FALSE(comb.HasOption("flag4").second);
  EXPECT_TRUE(comb.HasOption("flag5").first);
  EXPECT_FALSE(comb.HasOption("flag5").second);
}