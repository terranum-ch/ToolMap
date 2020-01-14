/***************************************************************************
 tmchecklistbox.cpp
                    Base class for dealing with checklistbox with long
 item associed to every item
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


#ifndef _TM_CHECKLISTBOX_H_
#define _TM_CHECKLISTBOX_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/checklst.h>    // for checklistbox
#include <wx/listctrl.h>    // for event

// check box list styles
#define tmLB_MENU 0x10000    // we want menu

// check box event list
const int tmCHECK_MENU_MOVE_TOP = 13001;
const int tmCHECK_MENU_MOVE_BOTTOM = 13002;
const int tmCHECK_MENU_MOVE_UP = 13003;
const int tmCHECK_MENU_MOVE_DOWN = 13004;
const int tmCHECK_MENU_TOGGLE_FREQUENT = 13005;

/***************************************************************************//**
 @brief Deals with checkbox list
 @details This class enhance the wxChecklistBox by allowing the following
 operations :
 - Adding items with related id's
 - Removing items (and related id's)
 - Moving items between positions
 - Dealing with right click on items for
 displaying a contextual menu containing options for moving items.
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 May 2008
 *******************************************************************************/
class tmCheckListBox : public wxCheckListBox
{
private:
    wxArrayLong m_ids;
    wxArrayLong m_originalIds;
    wxArrayString m_originalLabels;
    wxArrayShort m_originalChecks;
    wxMenu *m_PopupMenu;
    bool m_IsFiltered;

    bool CreateStandardMenu();

    // event functions
    void OnDisplayPopupMenu(wxMouseEvent &event);

    void OnMoveItemInList(wxCommandEvent &event);

    void OnToggleFrequent(wxCommandEvent &event);

protected:

    //wxArrayInt m_Selections;

    void Init()
    {
        m_PopupMenu = NULL;
        m_IsFiltered = false;
    }

    wxMenu *GetPopupMenu()
    { return m_PopupMenu; }


public:
    // ctor
    tmCheckListBox()
    { Init(); }

    ~tmCheckListBox();

    tmCheckListBox(wxWindow *parent,
                   wxWindowID id,
                   const wxPoint &pos = wxDefaultPosition,
                   const wxSize &size = wxDefaultSize,
                   int nStrings = 0,
                   const wxString *choices = NULL,
                   long style = 0,
                   const wxValidator &validator = wxDefaultValidator,
                   const wxString &name = wxListBoxNameStr)
    {
        Init();
        Create(parent, id, pos, size, nStrings, choices, style, validator, name);
    }


    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                int nStrings = 0,
                const wxString *choices = NULL,
                long style = 0,
                const wxValidator &validator = wxDefaultValidator,
                const wxString &name = wxListBoxNameStr);

    // functions
    bool AddItem(long index = -1, long id = -1,
                 const wxString &name = wxEmptyString,
                 bool checked = false, bool keepFilters = false);

    bool RemoveItem(long index = -1);

    bool EditItem(long index, long id = -1,
                  const wxString &name = wxEmptyString,
                  short checked = 2);

    bool SwapItem(long index1, long index2 = -1);

    bool MoveItem(long index1, long index2);

    bool GetItem(long index, long &id, wxString &name, bool &bcheck);

    bool GetItemId(long index, long &id);

    bool SetItemCheck(long index, short checked = 2);

    void ClearItems();

    void ClearFilters();

    void ClearCheckMarks();

    void Filter(wxString filter);

    void ResetOriginalArrays();

    bool StoreActualChecks();
};


#endif
