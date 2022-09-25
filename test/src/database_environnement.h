#ifndef TOOLMAP_DATABASE_ENVIRONNEMENT_H
#define TOOLMAP_DATABASE_ENVIRONNEMENT_H

#include <gtest/gtest.h>
#include "../../src/database/database_tm.h"

class DatabaseEnvironment : public ::testing::Environment {
 public:
  static DataBaseTM *m_db;

  // Override this to define how to set up the environment.
  void SetUp() override;

  // Override this to define how to tear down the environment.
  void TearDown() override;
};


#endif  // TOOLMAP_DATABASE_ENVIRONNEMENT_H
