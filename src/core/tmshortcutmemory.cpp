/***************************************************************************
 tmShortcutMemory.cpp
 Store shortcut in memory for attribution
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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

#include "tmshortcutmemory.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(tmShortCutObjArray);

tmShortcutObject::tmShortcutObject() {
  InitMemberValues();
}

tmShortcutObject::~tmShortcutObject() {}

void tmShortcutObject::InitMemberValues() {
  m_LayerType = -1;
  m_ShortcutKey = 0;
  m_ShortcutDescription = _T("");
  m_ShortcutValues.Clear();
  m_ShortcutValid = false;
}

/******************************* tmShortcutMemory *******************/
tmShortcutMemory::tmShortcutMemory() {}

tmShortcutMemory::~tmShortcutMemory() {}

/***************************************************************************/ /**
  @brief Add a shortcut to the memory
  @details This function store in memory all the active shortcuts.
  @param iLayerType the type of the layer, valid values are : 0, 1, 2 see
  #PRJDEF_LAYERS_TYPE
  @param iKey the active shortcut key, between 1-12
  @param description The shortcut description.
  @param lShortcutValue the value of the shortcut
  @return  true if shortcut was added sucessfully
  @author Lucien Schreiber (c) CREALP 2008
  @date 18 December 2008
  *******************************************************************************/
void tmShortcutMemory::AddShortcutMemory(int iLayerType, int iKey, const wxString &description, long lShortcutValue) {
  // find if the key already exists in the memory array
  for (unsigned int i = 0; i < m_Shortcuts.GetCount(); i++)
    if (m_Shortcuts.Item(i).m_ShortcutKey == iKey) {
      m_Shortcuts.Item(i).m_ShortcutValues.Add(lShortcutValue);
      return;
    }

  // adding non existant item
  tmShortcutObject myShortcutObj;
  myShortcutObj.m_LayerType = iLayerType;
  myShortcutObj.m_ShortcutKey = iKey;
  myShortcutObj.m_ShortcutDescription = description;
  myShortcutObj.m_ShortcutValues.Add(lShortcutValue);
  myShortcutObj.m_ShortcutValid = true;
  m_Shortcuts.Add(myShortcutObj);
}

/***************************************************************************/ /**
  @brief Clear all shortcuts from memory
  @author Lucien Schreiber (c) CREALP 2008
  @date 18 December 2008
  *******************************************************************************/
void tmShortcutMemory::Clear() {
  m_Shortcuts.Clear();
}

/***************************************************************************/ /**
  @brief Get shortcut by key
  @param iKey The key (1-12) for which we are searching the shortcut
  @param iLayerType The layer type to which the shortcut apply
  @param description The description of the shortcut
  @param shortcutvalues An array of all values for the shortcut
  @return  The number of values for shortcut key specified by iKey. if the
  shortcut isn't defined, wxNOT_FOUND is returned
  @author Lucien Schreiber (c) CREALP 2008
  @date 18 December 2008
  *******************************************************************************/
int tmShortcutMemory::GetShortcut(int iKey, int &iLayerType, wxString &description, wxArrayLong &shortcutvalues) {
  tmShortcutObject myShortcutObj;

  for (unsigned int i = 0; i < m_Shortcuts.GetCount(); i++) {
    if (m_Shortcuts.Item(i).m_ShortcutKey == iKey) {
      // shortcut found
      myShortcutObj = m_Shortcuts.Item(i);
      iLayerType = myShortcutObj.m_LayerType;
      description = myShortcutObj.m_ShortcutDescription;
      shortcutvalues = myShortcutObj.m_ShortcutValues;

      int iNbValues = shortcutvalues.GetCount();
      wxASSERT(iNbValues > 0);
      return iNbValues;
    }
  }

  return wxNOT_FOUND;
}
