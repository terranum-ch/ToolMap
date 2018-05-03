/***************************************************************************
								tmchecklistbox.cpp
                    Base class for dealing with checklistbox with long
						item associed to every item
                             -------------------
    copyright            : (C) 2008 CREALP Lucien Schreiber 
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

// comment doxygen

#include "tmchecklistbox.h"
#include "../gis/tmmanagerevent.h"


/***************************************************************************//**
 @brief Creator for two steps creation
 @author Lucien Schreiber (c) CREALP 2008
 see docs of wxCheckBoxList for parameter documentations
 @date 14 May 2008
 *******************************************************************************/
bool tmCheckListBox::Create(wxWindow *parent,
                            wxWindowID id,
                            const wxPoint &pos,
                            const wxSize &size,
                            int nStrings,
                            const wxString *choices,
                            long style,
                            const wxValidator &validator,
                            const wxString &name)
{
    if (!wxCheckListBox::Create(parent, id, pos, size,
                                nStrings, choices, style, validator, name)) {
        return false;
    }

    // add popup menu if needed
    if (style & tmLB_MENU) {
        m_PopupMenu = new wxMenu();
        CreateStandardMenu();

        // attach the menu to the event
        Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(tmCheckListBox::OnDisplayPopupMenu));
        Connect(tmCHECK_MENU_MOVE_TOP,
                tmCHECK_MENU_MOVE_DOWN,
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(tmCheckListBox::OnMoveItemInList));
        Connect(tmCHECK_MENU_TOGGLE_FREQUENT,
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(tmCheckListBox::OnToggleFrequent));
    }

    return TRUE;
}


/***************************************************************************//**
 @brief Destructor
 @details Delete the popup menu if needed
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 May 2008
 *******************************************************************************/
tmCheckListBox::~tmCheckListBox()
{
    if (m_PopupMenu) {
        Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(tmCheckListBox::OnDisplayPopupMenu));
        Disconnect(tmCHECK_MENU_MOVE_TOP,
                   tmCHECK_MENU_MOVE_DOWN,
                   wxEVT_COMMAND_MENU_SELECTED,
                   wxCommandEventHandler(tmCheckListBox::OnMoveItemInList));
    }

    wxDELETE(m_PopupMenu);

}


/***************************************************************************//**
 @brief Fill the popup menu with standard items
 @details Standard items are :
 - Move item to the top
 - Move item up
 - Move item down
 - Move item to the bottom
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 May 2008
 *******************************************************************************/
bool tmCheckListBox::CreateStandardMenu()
{
    if (GetPopupMenu() == NULL) {
        return false;
    }

    GetPopupMenu()->Append(tmCHECK_MENU_TOGGLE_FREQUENT, _("Change frequent / less frequent"));
    GetPopupMenu()->AppendSeparator();
    GetPopupMenu()->Append(tmCHECK_MENU_MOVE_TOP, _("Move item to the &top"));
    GetPopupMenu()->Append(tmCHECK_MENU_MOVE_UP, _("Move item &up"));
    GetPopupMenu()->Append(tmCHECK_MENU_MOVE_DOWN, _("Move item &down"));
    GetPopupMenu()->Append(tmCHECK_MENU_MOVE_BOTTOM, _("Move item to the &bottom"));

    return true;
}

/***************************************************************************//**
 @brief Called when user right click an item
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 May 2008
 *******************************************************************************/
void tmCheckListBox::OnDisplayPopupMenu(wxMouseEvent &event)
{
    if (GetPopupMenu() == NULL) {
        return;
    }

    if (GetCount() <= 0 || GetSelection() == wxNOT_FOUND) {
        return;
    }

    // enable all
    GetPopupMenu()->Enable(tmCHECK_MENU_MOVE_UP, true);
    GetPopupMenu()->Enable(tmCHECK_MENU_MOVE_TOP, true);
    GetPopupMenu()->Enable(tmCHECK_MENU_MOVE_DOWN, true);
    GetPopupMenu()->Enable(tmCHECK_MENU_MOVE_BOTTOM, true);
    GetPopupMenu()->Enable(tmCHECK_MENU_TOGGLE_FREQUENT, true);

    // disable popup items based on selected items
    if (GetSelection() == 0) {
        GetPopupMenu()->Enable(tmCHECK_MENU_MOVE_UP, false);
        GetPopupMenu()->Enable(tmCHECK_MENU_MOVE_TOP, false);
    }

    if (GetSelection() == (signed) (GetCount() - 1)) {
        GetPopupMenu()->Enable(tmCHECK_MENU_MOVE_DOWN, false);
        GetPopupMenu()->Enable(tmCHECK_MENU_MOVE_BOTTOM, false);
    }

    if (m_IsFiltered) {
        GetPopupMenu()->Enable(tmCHECK_MENU_MOVE_UP, false);
        GetPopupMenu()->Enable(tmCHECK_MENU_MOVE_TOP, false);
        GetPopupMenu()->Enable(tmCHECK_MENU_MOVE_DOWN, false);
        GetPopupMenu()->Enable(tmCHECK_MENU_MOVE_BOTTOM, false);
    }

    // show the menu
    PopupMenu(GetPopupMenu());
    event.Skip();
}


/***************************************************************************//**
 @brief Called for moving items in the list
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 May 2008
 *******************************************************************************/
void tmCheckListBox::OnMoveItemInList(wxCommandEvent &event)
{
    wxLogDebug(_T("Move called !"));
    int iSelectedPos = GetSelection();

    if (GetSelection() == wxNOT_FOUND) {
        wxLogDebug(_T("No items selected, select an item first"));
        return;
    }

    switch (event.GetId()) {
        case tmCHECK_MENU_MOVE_TOP:
            MoveItem(iSelectedPos, 0);
            break;

        case tmCHECK_MENU_MOVE_UP:
            SwapItem(iSelectedPos, iSelectedPos - 1);
            break;

        case tmCHECK_MENU_MOVE_DOWN:
            SwapItem(iSelectedPos, iSelectedPos + 1);
            break;

        case tmCHECK_MENU_MOVE_BOTTOM:
            MoveItem(iSelectedPos, GetCount() - 1);
            break;
    }
}


/***************************************************************************//**
 @brief Called for setting the object kind as frequent / less frequent
 @author Pascal Horton (c) TERRANUM 2018
 @date 26 April 2018
 *******************************************************************************/
void tmCheckListBox::OnToggleFrequent(wxCommandEvent &event)
{
    int iSelectedPos = GetSelection();

    if (GetSelection() == wxNOT_FOUND) {
        wxLogDebug(_T("No items selected, select an item first"));
        return;
    }
    // get item
    long id = 0;
    wxString name = wxEmptyString;
    bool bcheck = FALSE;

    if (!GetItem(iSelectedPos, id, name, bcheck)) {
        return;
    }

    wxCommandEvent evt(tmEVT_TOGGLE_FREQUENT, wxID_ANY);
    evt.SetInt(id);
    m_parent->GetEventHandler()->QueueEvent(evt.Clone());

}


/***************************************************************************//**
 @brief Add an item in the list
 @details This add the item in the list and join the ID to this item.
 @param index The zero based position of the item to add, -1 means add item to
 the end of the list
 @param id the id to attach to this item
 @param name the name of the item
 @param checked Should we check this item, default is false
 @return  TRUE if item was added succesfully
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 May 2008
 *******************************************************************************/
bool tmCheckListBox::AddItem(long index, long id,
                             const wxString &name,
                             bool checked, bool keepFilters)
{
	if (!keepFilters)
		ClearFilters();

    // adding item
    if (index == -1 || index >= (signed) GetCount()) {
        Append(name);
        m_ids.Add(id);
        index = GetCount() - 1;
    } else // inserting item
    {
        Insert(name, index);
        m_ids.Insert(id, index);
    }

    Check(index, checked);

	if (!keepFilters)
		ResetOriginalArrays();

    return TRUE;
}


/***************************************************************************//**
 @brief Remove an item in the list
 @details the specified item is removed from the list and from the array
 @param index zero based index of item to remove. If default value (-1) is used,
 last item is removed
 @return  TRUE if item removed sucessfully, FALSE otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 May 2008
 *******************************************************************************/
bool tmCheckListBox::RemoveItem(long index)
{
    ClearFilters();

    // if index == -1, remove last item
    if (index == -1)
        if (RemoveItem(GetCount() - 1))
            return TRUE;


    if (index >= (signed) GetCount()) {
        wxLogError(_T("Trying to delete item out of limits"));
        return FALSE;
    }

    if (index >= 0) {
        Delete(index);
        m_ids.RemoveAt(index);
    }

    ResetOriginalArrays();

    return TRUE;
}


/***************************************************************************//**
 @brief Modify an item
 @details With this function one can change values for an item only for usefull
 parameters. If using default
 @param index zero based index of the item to modify
 @param id the new id (or specify -1 for not changing the id)
 @param name the new name (or specify wxEmptyString for not changing the name)
 @param checked the new check state passed as short : 
 - 0 = FALSE
 - 1 = TRUE
 - 2 = Don't change
 @return  TRUE if item exist and, FALSE otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 May 2008
 *******************************************************************************/
bool tmCheckListBox::EditItem(long index, long id,
                              const wxString &name, short checked)
{
    ClearFilters();

    // check item exists
    if (index >= (signed) GetCount()) {
        wxLogError(_T("Trying to edit item out of bounds, item number : %ld"), index);
        return FALSE;
    }


    // change id
    if (id != -1)
        m_ids.Item(index) = id;

    // change name
    if (name != wxEmptyString) {
        SetString(index, name);
    }

    // change check state
    if (checked == 0) {
        Check(index, FALSE);
    } else if (checked == 1) {
        Check(index, TRUE);
    }

    ResetOriginalArrays();

    return TRUE;
}


/***************************************************************************//**
 @brief Switch two items
 @details This functions may be used for switching two items and keeping id's
 array organised.
 @param index1 zero based index of item 1
 @param index2 zero based index of item 2, if -1 (default) is used, we use the
 last item from the list
 @return  TRUE if item exists and FALSE if trying to move items out of limits
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 May 2008
 *******************************************************************************/
bool tmCheckListBox::SwapItem(long index1, long index2)
{
    ClearFilters();

    if (index1 < 0 && index1 >= (signed) GetCount()) {
        wxLogError(_T("Trying to move item out of bounds, item number : %ld"), index1);
        return false;
    }

    // get info for item 1
    long id1 = m_ids.Item(index1);
    wxString name1 = GetString(index1);
    bool checked1 = IsChecked(index1);

    // switch items
    EditItem(index1, m_ids.Item(index2), GetString(index2), IsChecked(index2));
    EditItem(index2, id1, name1, checked1);
    SetSelection(index2);

    ResetOriginalArrays();

    return TRUE;
}


/***************************************************************************//**
 @brief Move an items to a specified position
 @details This functions may be used for moving items and keeping id's
 array organised.
 @param index1 zero based index of item 1
 @param index2 zero based index of item 2, 
 @return  TRUE if item exists and FALSE if trying to move items out of limits
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 May 2008
 *******************************************************************************/
bool tmCheckListBox::MoveItem(long index1, long index2)
{

    // copy item 1
    long id1 = 0;
    wxString name1 = wxEmptyString;
    bool bcheck1 = FALSE;

    if (!GetItem(index1, id1, name1, bcheck1)) {
        return FALSE;
    }

    // delete item 1
    RemoveItem(index1);
    AddItem(index2, id1, name1, bcheck1);
    this->SetSelection(index2, true);

    ResetOriginalArrays();

    return TRUE;
}

/***************************************************************************//**
 @brief Get value for item at the specified position
 @param index zero based index of the item we want to get
 @param id the ID of the item stored in the corresponding array
 @param name the string displayed in the list
 @param bcheck the status of the check box
 @return  TRUE if all was correct, FALSE if we pass an index outside the limits
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 May 2008
 *******************************************************************************/
bool tmCheckListBox::GetItem(long index, long &id, wxString &name, bool &bcheck)
{
    // check that we aren't outside the limits
    if (index > (signed) GetCount() - 1) {
        wxLogDebug(_T("Index : %ld is outside the limits"), index);
        return FALSE;
    }

    if (index > (signed) m_ids.GetCount() - 1) {
        wxLogDebug(_T("Index %ld is greater than the wxArrayLong"), index);
        return FALSE;
    }

    id = m_ids.Item(index);
    name = GetString(index);
    bcheck = IsChecked(index);
    return TRUE;
}


bool tmCheckListBox::GetItemId(long index, long &id)
{
    // check that we aren't outside the limits
    if (index > (signed) GetCount() - 1) {
        wxLogDebug(_T("Index : %ld is outside the limits"), index);
        return FALSE;
    }

    if (index > (signed) m_ids.GetCount() - 1) {
        wxLogDebug(_T("Index %ld is greater than the wxArrayLong"), index);
        return FALSE;
    }

    id = m_ids.Item(index);
    return TRUE;
}


bool tmCheckListBox::SetItemCheck(long index, short checked)
{
    // check item exists
    if (index >= (signed) GetCount()) {
        wxLogError(_T("Trying to edit item out of bounds, item number : %ld"), index);
        return FALSE;
    }

    // change check state
    if (checked == 0) {
        Check(index, FALSE);
    } else if (checked == 1) {
        Check(index, TRUE);
    }

    return TRUE;
}

/***************************************************************************//**
 @brief Empty the checklist and associated array
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 May 2008
 *******************************************************************************/
void tmCheckListBox::ClearItems()
{
    Clear();
    m_ids.Clear();
    m_originalIds.Clear();
    m_originalLabels.Clear();
    m_originalChecks.Clear();
}

/***************************************************************************//**
@brief Clear the filters and restore original arrays
@author Pascal Horton (c) TERRANUM 2018
@date 3 May 2018
*******************************************************************************/
void tmCheckListBox::ClearFilters()
{
    if (!m_IsFiltered)
        return;

    m_ids.Clear();
    m_ids = m_originalIds;

    Clear();
    for (unsigned int i = 0; i < m_originalLabels.Count(); ++i) {
        wxString label = m_originalLabels.Item(i);
        Insert(label, i);
        Check(i, m_originalChecks.Item(i) == 1);
    }
}


/***************************************************************************//**
 @brief Uncheck all items
 @author Lucien Schreiber (c) CREALP 2009
 @date 13 February 2009
 *******************************************************************************/
void tmCheckListBox::ClearCheckMarks()
{
    for (unsigned int i = 0; i < GetCount(); i++) {
        EditItem(i, -1, wxEmptyString, 0);
    }
}


/***************************************************************************//**
@brief Filter the list and keep only the labels containing the string.
@author Pascal Horton (c) TERRANUM 2018
@date 3 May 2018
*******************************************************************************/
void tmCheckListBox::Filter(wxString filter)
{
    filter.Trim(true).Trim(false);

    // If filtered before, save checks
    if (m_IsFiltered) {
        if (!StoreActualChecks())
            return;
    }

    // If filter empty, clear
    if (filter.IsEmpty()) {
        if (m_IsFiltered) {
            ClearFilters();
            m_IsFiltered = false;
        }
        return;
    }

    // If newly filtered, save original arrays
    if (!m_IsFiltered) {
        ResetOriginalArrays();
    }

    m_IsFiltered = true;
    Clear();
    m_ids.Clear();

    for (unsigned int i = 0; i < m_originalLabels.GetCount(); ++i) {
        wxString label = m_originalLabels.Item(i);
        if (label.Lower().Find(filter.Lower()) != wxNOT_FOUND) {
            AddItem(-1, m_originalIds.Item(i), m_originalLabels.Item(i), m_originalChecks.Item(i) == 1, true);
        }
    }
}


void tmCheckListBox::ResetOriginalArrays()
{
    m_originalIds = m_ids;
    m_originalLabels = GetStrings();
    m_originalChecks.Clear();
    for (unsigned int i = 0; i < GetCount(); ++i) {
        if (IsChecked(i)) {
            m_originalChecks.Add(1);
        } else {
            m_originalChecks.Add(0);
        }

    }
}

bool tmCheckListBox::StoreActualChecks()
{
    for (unsigned int i = 0; i < GetCount(); ++i) {
        for (unsigned int j = 0; j < m_originalIds.GetCount(); ++j) {

            long id = -1;
            if (!GetItemId(i, id)) {
                wxLogError(_("Failed at storing actual checks."));
                return false;
            }

            if (m_originalIds.Item(j) == id) {
                if (IsChecked(i)) {
                    m_originalChecks.Item(j) = 1;
                } else {
                    m_originalChecks.Item(j) = 0;
                }
            }
        }
    }

    return true;
}