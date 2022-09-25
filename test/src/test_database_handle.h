

#ifndef TOOLMAP_TEST_DATABASE_HANDLE_H
#define TOOLMAP_TEST_DATABASE_HANDLE_H

#include "../../src/database/database_tm.h"
#include "gtest/gtest.h"

class DatabaseHandle : public ::testing::Test {
 protected:
  static DataBaseTM *m_db;

  static void SetUpTestSuite();

  static void TearDownTestSuite();
};

#endif  // TOOLMAP_TEST_DATABASE_HANDLE_H
