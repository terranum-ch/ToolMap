#include "gtest/gtest.h"
#include "../../src/core/bezierethz.h"

TEST(BezierEthzTest, ConstructorTest) {
    // Test the constructor of BezierEthz
    BezierEthz bezierEthz(5.0, 10);
    ASSERT_EQ(bezierEthz.GetPointListRef()->GetCount(), 0);
}

TEST(BezierEthzTest, InitTest) {
    // Test the Init method of BezierEthz
    BezierEthz bezierEthz(5.0, 10);
    wxRealPoint P1(1.0, 1.0);
    wxRealPoint P2(2.0, 2.0);
    wxRealPoint P3(3.0, 3.0);
    wxRealPoint P4(4.0, 4.0);
    bezierEthz.Init(P1, P2, P3, P4);
    ASSERT_EQ(bezierEthz.GetPointListRef()->GetCount(), 3);
}

