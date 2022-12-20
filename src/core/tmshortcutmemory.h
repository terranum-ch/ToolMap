/***************************************************************************
 tmShortcutMemory.h
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

#ifndef _TM_SHORTCUT_MEMORY_H_
#define _TM_SHORTCUT_MEMORY_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class tmShortcutObject {
  private:
    void InitMemberValues();

  public:
    int m_LayerType;
    int m_ShortcutKey;
    wxString m_ShortcutDescription;
    wxArrayLong m_ShortcutValues;
    bool m_ShortcutValid;

    tmShortcutObject();

    ~tmShortcutObject();
};

// Creating a list of MemoryObjects
WX_DECLARE_OBJARRAY(tmShortcutObject, tmShortCutObjArray);

/***************************************************************************/ /**
  @brief Store shortcuts for attribution in memory
  @details Shortcuts are stored in :
  - The database
  - Memory
  For the latest,
  shortcuts are stored in memory for direct access when attributing. When the
  following operations are exectued in the #Shortcuts_PANEL all shortcuts are
  read again from the database and stored into memory
  - Project is open
  - New shortcut is added
  - Shortcut is edited
  - A shortcut is removed
  @author Lucien Schreiber (c) CREALP 2008
  @date 18 December 2008
  *******************************************************************************/
class tmShortcutMemory : public wxObject {
  private:
    tmShortCutObjArray m_Shortcuts;

  protected:
  public:
    tmShortcutMemory();

    ~tmShortcutMemory();

    // shortcut operations
    void AddShortcutMemory(int iLayerType, int iKey, const wxString& description, long lShortcutValue);

    void Clear();

    int GetShortcut(int iKey, int& iLayerType, wxString& description, wxArrayLong& shortcutvalues);

    int GetCount() {
        return m_Shortcuts.GetCount();
    }
};

#endif
