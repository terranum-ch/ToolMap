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
		Connect(tmCHECK_MENU_MOVE_TOP,
				tmCHECK_MENU_MOVE_DOWN,
				wxEVT_COMMAND_MENU_SELECTED, 
				wxCommandEventHandler(tmCheckListBox::OnMoveItemInList));	
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

/***************************************************************************//**
 @brief Called when user right click an item
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 May 2008
 *******************************************************************************/
void tmCheckListBox::OnDisplayPopupMenu(wxMouseEvent & event)
{
	
	if (GetPopupMenu() == NULL)
		return;
	
	if(GetCount() <= 0 || GetSelections(m_Selections) <= 0)
		return;
	
	// enable all
	m_PopupMenu->Enable(tmCHECK_MENU_MOVE_UP, TRUE);
	m_PopupMenu->Enable(tmCHECK_MENU_MOVE_TOP, TRUE);
	m_PopupMenu->Enable(tmCHECK_MENU_MOVE_DOWN, TRUE);
	m_PopupMenu->Enable(tmCHECK_MENU_MOVE_BOTTOM, TRUE);
	
	// disable popup items based on selected items
	for (unsigned int i = 0; i< m_Selections.GetCount(); i++)
	{
		if (m_Selections.Item(i) == 0)
		{
			m_PopupMenu->Enable(tmCHECK_MENU_MOVE_UP, FALSE);
			m_PopupMenu->Enable(tmCHECK_MENU_MOVE_TOP, FALSE);
		}
		
		if (m_Selections.Item(i) == (signed) (GetCount() -1))
		{
			m_PopupMenu->Enable(tmCHECK_MENU_MOVE_DOWN, FALSE);
			m_PopupMenu->Enable(tmCHECK_MENU_MOVE_BOTTOM, FALSE);
		}
		
	}

	// show the menu
	PopupMenu(m_PopupMenu, event.GetPosition());
	event.Skip();
}



/***************************************************************************//**
 @brief Called for moving items in the list
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 May 2008
 *******************************************************************************/
void tmCheckListBox::OnMoveItemInList (wxCommandEvent & event)
{
	// selected items where get from the OnDisplayPopupMenu functions
	int i=0;
	int idestpos = 0;
	int iCountSelected = m_Selections.GetCount();
	
	if (iCountSelected <= 0)
	{
		wxLogDebug(_T("No items selected, select an item first"));
		return;
	}
	
	
	switch (event.GetId())
	{
		case tmCHECK_MENU_MOVE_TOP:
			// compute movement 
			idestpos = m_Selections[0] - 0;
			
			for (i=0; i< iCountSelected; i++)
			{
				MoveItem(m_Selections[i],m_Selections[i] - idestpos);
			}
			break;
			
			case tmCHECK_MENU_MOVE_UP:
			for (i=0; i< iCountSelected; i++)
			{
				SwapItem(m_Selections[i],m_Selections[i] - 1);
			}
			break;
			
			case tmCHECK_MENU_MOVE_DOWN:
			for (i = iCountSelected -1; i >= 0 ; i--)
			{
				SwapItem(m_Selections[i],m_Selections[i] + 1);
			}
			break;
			
			case tmCHECK_MENU_MOVE_BOTTOM:
			// compute movement 
			idestpos = (GetCount() - 1) - m_Selections.Last();
			
			for (i = iCountSelected -1; i >= 0 ; i--)
			{
				MoveItem(m_Selections[i],m_Selections[i] + idestpos + 1);
			}
			break;
	}
	
	// no selection
	//DeselectAll();
	/// @bug DeselectAll() is not working as espected in the actual version of wxWidgets

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
	
	m_ids.RemoveAt(index);
		
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
	if (checked == 0)
		Check (index, FALSE);
	if (checked == 1)
		Check (index, TRUE);

	
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
bool tmCheckListBox::SwapItem (long index1, long index2)
{
	// check that items exists
	if (index2 == -1)
		if (!SwapItem(index1, GetCount() -1))
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
bool tmCheckListBox::MoveItem (long index1, long index2)
{

	// copy item 1
	long id1 = 0;
	wxString name1 = wxEmptyString;
	bool bcheck1 = FALSE;
	
	if (!GetItem(index1, id1, name1, bcheck1))
	{
		return FALSE;
	}
	
	// delete item 1
	RemoveItem(index1);
	
	AddItem(index2, id1, name1, bcheck1);
	
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
bool tmCheckListBox::GetItem (long index, long & id, wxString & name, bool & bcheck)
{
	// check that we aren't outside the limits
	if (index > (signed) GetCount() - 1)
	{
		wxLogDebug(_T("Index : %d is outside the limits"), index);
		return FALSE;
	}
	
	if (index > (signed) m_ids.GetCount() - 1)
	{
		wxLogDebug(_T("Index %d is greather than the wxArrayLong"), index);
		return FALSE;
	}
	
	id = m_ids.Item(index);
	name = GetString(index);
	bcheck = IsChecked(index);
	return TRUE;
}


/***************************************************************************//**
 @brief Empty the checklist and associated array
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 May 2008
 *******************************************************************************/
void tmCheckListBox::ClearItems ()
{
	Clear();
	m_ids.Clear();
}


