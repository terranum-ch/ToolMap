/***************************************************************************
 tmattributionbasicarray.cpp
                    An array for storing basic attributions values
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// comment doxygen

#include "tmattributionbasicarray.h"

tmAttributionBasic::tmAttributionBasic() {
  InitMemberValues();
}

tmAttributionBasic::~tmAttributionBasic() {}

void tmAttributionBasic::InitMemberValues() {
  m_Oid = wxNOT_FOUND;
  m_Values.Clear();
}

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(tmAttributionBasicArray);

tmLayerValue::tmLayerValue() {
  InitMemberValues();
}

tmLayerValue::~tmLayerValue() {}

void tmLayerValue::InitMemberValues() {
  m_Oid = wxNOT_FOUND;
  m_Value = wxEmptyString;
}

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(tmLayerValueArray);
