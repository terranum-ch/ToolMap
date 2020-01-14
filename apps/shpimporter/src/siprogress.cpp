/***************************************************************************
 siprogress.cpp
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber
 email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "siprogress.h"

siProgressIndicator::siProgressIndicator() {
  m_Total = 0;
}

siProgressIndicator::~siProgressIndicator() {}

void siProgressIndicator::UpdateProgress(int value, int step) {
  if (value >= (step + m_Total)) {
    m_Total = m_Total + step;
    wxPrintf(_T("#"));
  }
}

void siProgressIndicator::StartProgress() {
  wxPrintf(_T("Progress: ["));
}

void siProgressIndicator::StopProgress() {
  wxPrintf(_T("]\n"));
}
