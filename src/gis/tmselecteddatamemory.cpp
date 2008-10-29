/***************************************************************************
							tmSelectedDataMemory.cpp
                    Store ID of selected data into memory                             
							-------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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

#include "tmselecteddatamemory.h"

 

/***************************************************************************//**
 @brief Constructor
 @details Init defaut values only
 @author Lucien Schreiber (c) CREALP 2008
 @date 28 October 2008
 *******************************************************************************/
tmSelectedDataMemory::tmSelectedDataMemory()
{	
	InitMemberValues();
}



/***************************************************************************//**
 @brief Destructor
 @details 
 @author Lucien Schreiber (c) CREALP 2008
 @date 28 October 2008
 *******************************************************************************/
tmSelectedDataMemory::~tmSelectedDataMemory()
{
	m_SelectedIDs->Clear();
	delete m_SelectedIDs;
}



/***************************************************************************//**
 @brief Member initialization
 @details 
 @author Lucien Schreiber (c) CREALP 2008
 @date 28 October 2008
 *******************************************************************************/
void tmSelectedDataMemory::InitMemberValues()
{
	m_LayerID = -1;
	m_SelectedIDs = new tmArraySortedLong(CompareLongs);
	m_Colour = wxColour(*wxRED);
}



/***************************************************************************//**
 @brief Add selected ID to memory
 @details Principal behaviour is to add items to the sorted array. But if
 selected contain only one item and this item allready exists into the array it
 is removed from the array
 @param selected Adress of a wxArrayLong structure. This dosen't take ownership
 of the wxArrayInt and it is the responsability of the caller to destroy it
 @return  true if adding successfull
 @author Lucien Schreiber (c) CREALP 2008
 @date 28 October 2008
 *******************************************************************************/
bool tmSelectedDataMemory::AddSelected (wxArrayLong * selected)
{
	// removing from array if :
	// - one item only
	// - item allready existing into m_SelectedIDs
	if (selected->GetCount() == 1)
	{
		//TODO: optimize for searching from end if required
		int iIndex = Search(selected->Item(0));
		if (iIndex != wxNOT_FOUND)
		{
			m_SelectedIDs->Remove(iIndex);
			wxLogDebug(_T("Item %d removed from index because it exsit allready"));
			return true;
		}
	}
	
	
	// normal behaviour : copying items
	for (unsigned int i = 0; i<selected->GetCount(); i++)
	{
		m_SelectedIDs->Add(selected->Item(i));
	}
	
	wxLogDebug(_T("%d items added to the memory"), m_SelectedIDs->GetCount());
	return true;
	
}



/***************************************************************************//**
 @brief Search for existing item in the array
 @param value value we are searching for
 @return  the 0 based index of the result or wxNOT_FOUND
 @author Lucien Schreiber (c) CREALP 2008
 @date 28 October 2008
 *******************************************************************************/
int tmSelectedDataMemory::Search (const long & value)
{
	int iIndex = m_SelectedIDs->Index(value);
	return iIndex;
}



/***************************************************************************//**
 @brief Removing items from the array
 @details May be used for following operations :
 - Removing one item
 - Removing multiple items
 - Removing all items
 @param selected array of items we want to remove. If NULL, all values are
 removed
 @return return true if removing items successfull
 @author Lucien Schreiber (c) CREALP 2008
 @date 28 October 2008
 *******************************************************************************/
bool tmSelectedDataMemory::Remove (wxArrayLong * selected)
{
	// clearing if required
	if (!selected)
	{
		m_SelectedIDs->Clear();
		return TRUE;
	}
	
	// normal behaviour
	int iIndex = 0;
	for (unsigned int i = selected->GetCount()-1 ; i >= 0 ; i--)
	{
		iIndex = Search(selected->Item(i));
		if (iIndex != wxNOT_FOUND)
			m_SelectedIDs->Remove(iIndex);
		else
		{
			wxLogDebug(_T("Trying to remove non existring item from memory..."));
		}
	}
	
}



/***************************************************************************//**
 @brief Is this object selected
 @param value the Object ID value we want to check for selected state
 @return  true if the item is selected
 @author Lucien Schreiber (c) CREALP 2008
 @date 28 October 2008
 *******************************************************************************/
bool tmSelectedDataMemory::IsSelected (const long & value)
{
	if (Search(value) != wxNOT_FOUND)
		return true;
	return false;
}



/***************************************************************************//**
 @brief Count the number of selected features
 @return  the number of selected features
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
unsigned int tmSelectedDataMemory::GetCount()
{
	return m_SelectedIDs->GetCount();
}

