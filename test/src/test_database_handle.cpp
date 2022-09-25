//
// Created by Lucien Schreiber on 25.09.22.
//

#include "test_database_handle.h"

void DatabaseHandle::SetUpTestSuite(){
  if (m_db == nullptr) {
    m_db = new DataBaseTM();
  }
}

void DatabaseHandle::TearDownTestSuite() {
  wxDELETE(m_db);
}

DataBaseTM * DatabaseHandle::m_db = nullptr;