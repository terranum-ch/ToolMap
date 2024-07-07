#include <gtest/gtest.h>
#include "../../src/core/vrrealrect.h"

// Test fixture for vrRealRect
class VrRealRectTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Set up common data members here
    }

    void TearDown() override {
        // Clean up common data members here
    }

    vrRealRect rect1;
    vrRealRect rect2;
};

TEST_F(VrRealRectTest, DefaultConstructor) {
    EXPECT_FALSE(rect2.IsOk());  // rect is empty
}

TEST_F(VrRealRectTest, CopyConstructor) {
    rect1 = vrRealRect(0.0, 0.0, 10.0, 10.0);
    rect2 = rect1;
    vrRealRect rect3(rect1);
    EXPECT_TRUE(rect2.IsOk());
    EXPECT_EQ(rect1.GetLeft(), rect2.GetLeft());
    EXPECT_EQ(rect1.GetTop(), rect2.GetTop());
    EXPECT_EQ(rect1.m_width, rect2.m_width);
    EXPECT_EQ(rect1.m_width, rect3.m_width);
    EXPECT_EQ(rect1.m_height, rect2.m_height);
    EXPECT_EQ(rect1.m_height, rect3.m_height);
}

TEST_F(VrRealRectTest, Constructor) {
    vrRealRect rect(0.0, 0.0, 10.0, 10.0);
    EXPECT_TRUE(rect.IsOk());
    EXPECT_EQ(rect.GetLeft(), 0.0);
    EXPECT_EQ(rect.GetTop(), 0.0);
    EXPECT_EQ(rect.m_width, 10.0);
    EXPECT_EQ(rect.m_height, 10.0);
}

TEST_F(VrRealRectTest, Intersect) {
    vrRealRect rect1(0.0, 0.0, 10.0, 10.0);
    vrRealRect rect2(5.0, 5.0, 5.0, 5.0);

    vrRealRect result = rect1.Intersect(rect2);

    EXPECT_TRUE(result.IsOk());
    EXPECT_EQ(result.GetLeft(), 5.0);
    EXPECT_EQ(result.GetTop(), 5.0);
    EXPECT_EQ(result.m_width, 5.0);
    EXPECT_EQ(result.m_height, 5.0);
}

TEST_F(VrRealRectTest, IntersectNeg) {
    vrRealRect rect1(-10.0, -10.0, 10.0, 10.0);
    vrRealRect rect2(-5.0, -5.0, 10.0, 10.0);

    vrRealRect result = rect1.Intersect(rect2);

    EXPECT_TRUE(result.IsOk());
    EXPECT_EQ(result.GetLeft(), -5.0);
    EXPECT_EQ(result.GetTop(), -5.0);
    EXPECT_EQ(result.m_width, 5.0);
    EXPECT_EQ(result.m_height, 5.0);
}

TEST_F(VrRealRectTest, IntersectNeg2) {
    vrRealRect rect1(0, 0, 10.0, 10.0);
    vrRealRect rect2(5, -5, 10.0, 10.0);

    vrRealRect result = rect1.Intersect(rect2);

    EXPECT_TRUE(result.IsOk());
    EXPECT_EQ(result.GetLeft(), 5.0);
    EXPECT_EQ(result.GetTop(), 0.0);
    EXPECT_EQ(result.m_width, 5.0);
    EXPECT_EQ(result.m_height, 5.0);
}