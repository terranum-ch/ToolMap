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



/***************************************************************************//**
 @brief Creator for two steps creation
 @author Lucien Schreiber (c) CREALP 2008
 see docs of wxCheckBoxList for parameter documentations
 @date 14 May 2008
 *******************************************************************************/
bool tmCheckListBox::Create(wxWindow * parent,
			wxWindowID id, 
			const wxPoint & pos,
			const wxSize & size,
			int nStrings,
			const wxString *choices,
			long style,
			const wxValidator& validator,
			const wxString& name) 
{
	if ( !wxCheckListBox::Create(parent, id, pos, size,
								 nStrings, choices, style, validator, name) )
	{
        return false;
	}
	
	// add popup menu if needed
	if (style & tmLB_MENU)
	{
		m_PopupMenu = new wxMenu();
		CreateStandardMenu();
		
		// attach the menu to the event
		Connect(id, wxEVT_RIGHT_DOWN,
				wxMouseEventHandler(tmCheckListBox::OnDisplayPopupMenu));
		
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
	if (m_PopupMenu != NULL)
		delete m_PopupMenu;
	
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
	if (GetPopupMenu()==NULL)
		return FALSE;
	
	m_PopupMenu->Append(tmCHECK_MENU_MOVE_TOP, _("Move item to the &top"));
	m_PopupMenu->Append(tmCHECK_MENU_MOVE_UP, _("Move item &up"));
	m_PopupMenu->AppendSeparator();
	m_PopupMenu->Append(tmCHECK_MENU_MOVE_DOWN, _("Move item &down"));
	m_PopupMenu->Append(tmCHECK_MENU_MOVE_BOTTOM,_("Move item to the &bottom"));
	
	return TRUE;
}


void tmCheckListBox::OnDisplayPopupMenu(wxMouseEvent & event)
{
	
	if (GetPopupMenu() == NULL)
		return;
	
	if(GetCount() <= 0 || GetSelections(m_Selections) <= 0)
		return;
	
	PopupMenu(m_PopupMenu, event.GetPosition());
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
bool tmCheckListBox::AddItem (long index, long id,
							  const wxString & name, 
							  bool checked)
{
	// adding item
	if (index == -1 || index >= (signed) GetCount())
	{
		Append(name);
		m_ids.Add(id);
		index = GetCount() -1;
	}
	else // inserting item 
	{
		Insert(name, index);
		m_ids.Insert(id, index);
	}
	
	Check(index, checked);
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
bool tmCheckListBox::RemoveItem (long index)
{
	// if index == -1, remove last item
	if (index == -1)
		if(RemoveItem(GetCount()-1))
		   return TRUE;
	
	
	if (index >= (signed) GetCount())
	{
		wxLogError(_T("Trying to delete item out of limits"));
		return FALSE;
	}
	
	Delete(index);
	m_ids.Remove(index);
	
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
bool tmCheckListBox::EditItem (long index, long id,
							   const wxString & name,  short checked)
{
	// check item exists
	if (index >= (signed) GetCount())
	{
		wxLogError(_T("Trying to edit item out of bounds, item number : %d"), index);
		return FALSE;
	}
	
	
	// change id
	if (id != -1)
		m_ids.Item(index) = id;
	
	// change name
	if (name != wxEmptyString)
		SetString(index, name);
	
	// change check state 
	if (checked == 0 || checked == 1)
		Check(index, (bool) checked);
	
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
bool tmCheckListBox::MoveItem (long index1, long index2)
{
	// check that items exists
	if (index2 == -1)
		if (!MoveItem(index1, GetCount() -1))
			return FALSE;
	
	if (index1 < 0 && index1 >= (signed) GetCount())
	{
		wxLogError(_T("Trying to move item out of bounds, item number : %d"), index1);
		return FALSE;
	}
	
	// get info for item 1
	long id1 = m_ids.Item(index1);
	wxString name1 = GetString(index1);
	bool checked1 = IsChecked(index1);
	
	// switch items
	EditItem(index1, m_ids.Item(index2), GetString(index2), IsChecked(index2));
	EditItem(index2, id1, name1, checked1);
	return TRUE;
}


