#include "../../src/core/scale.h"
#include <gtest/gtest.h>

TEST(ScaleTMTest, GetScaleFromLong) {
    wxString stringscale;
    EXPECT_TRUE(ScaleTM::GetScaleFromLong(1000, stringscale));
    EXPECT_EQ(stringscale, "1:1000");

    EXPECT_TRUE(ScaleTM::GetScaleFromLong(0, stringscale));
    EXPECT_EQ(stringscale, "1:0");
}

TEST(ScaleTMTest, GetScaleFromString) {
    EXPECT_EQ(ScaleTM::GetScaleFromString("1:1000"), 1000);
    EXPECT_EQ(ScaleTM::GetScaleFromString("1:0"), 0);

    EXPECT_EQ(ScaleTM::GetScaleFromString(""), -1);
    EXPECT_EQ(ScaleTM::GetScaleFromString("1000"), -1);
}
