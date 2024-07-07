#include "gtest/gtest.h"
#include "../../src/core/tmarraysize.h" // Include the header file for tmRealPointDist

// Test case for default constructor
TEST(tmRealPointDistTest, DefaultConstructor) {
    tmRealPointDist pt;
    // Add assertions to test the default constructor
    ASSERT_EQ(pt.GetCoordinate(), wxRealPoint(0, 0));
    ASSERT_EQ(pt.GetOrigin(), wxRealPoint(0, 0));
}

// Test case for custom constructor
TEST(tmRealPointDistTest, CustomConstructor) {
    tmRealPointDist pt(1.0, 2.0, wxRealPoint(3.0, 4.0));
    // Add assertions to test the custom constructor
    ASSERT_EQ(pt.GetCoordinate(), wxRealPoint(1.0, 2.0));
    ASSERT_EQ(pt.GetOrigin(), wxRealPoint(3.0, 4.0));
}

// Test case for copy function
TEST(tmRealPointDistTest, CopyFunction) {
    tmRealPointDist pt1(1.0, 2.0, wxRealPoint(3.0, 4.0));
    tmRealPointDist pt2 = pt1;
    // Add assertions to test the copy function
    ASSERT_EQ(pt2.GetCoordinate(), wxRealPoint(1.0, 2.0));
    ASSERT_EQ(pt2.GetOrigin(), wxRealPoint(3.0, 4.0));
}

// Test case for operator=
TEST(tmRealPointDistTest, OperatorEqual) {
    tmRealPointDist pt1(1.0, 2.0, wxRealPoint(3.0, 4.0));
    tmRealPointDist pt2 = pt1;
    // Add assertions to test the operator=
    ASSERT_EQ(pt2.GetCoordinate(), wxRealPoint(1.0, 2.0));
    ASSERT_EQ(pt2.GetOrigin(), wxRealPoint(3.0, 4.0));
}

// Test case for GetDistFromOrigin
TEST(tmRealPointDistTest, GetDistFromOrigin) {
    tmRealPointDist pt(1.0, 2.0, wxRealPoint(1.0, 3.0));
    // Add assertions to test the GetDistFromOrigin
    ASSERT_FLOAT_EQ(pt.GetDistFromOrigin(), 1.0);
}

// Test case for GetDistFromOrigin2
TEST(tmRealPointDistTest, GetDistFromOrigin2) {
    tmRealPointDist pt(1.0, 1.0, wxRealPoint(3.0, 3.0));
    // Add assertions to test the GetDistFromOrigin
    ASSERT_FLOAT_EQ(pt.GetDistFromOrigin(), 2.8284271247);
}