#include <gtest/gtest.h>
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
int main(int argc, char **argv) {
  wxInitialize();
  wxLog::SetActiveTarget(new wxLogStderr());
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}