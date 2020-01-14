/***************************************************************************
 tmSelectedDataMemory.cpp
                    Store ID of selected data into memory                             
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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

// for comparaison 
int CompareLongs(long n1, long n2)
{
    return static_cast<int> (n1 - n2);
}


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
    m_HasHalo = false;
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
bool tmSelectedDataMemory::AddSelected(wxArrayLong *selected)
{
    // removing from array if :
    // - one item only
    // - item allready existing into m_SelectedIDs
    if (selected->GetCount() == 1) {
        //TODO: optimize for searching from end if required
        int iIndex = Search(selected->Item(0));
        if (iIndex != wxNOT_FOUND) {
            m_SelectedIDs->RemoveAt(iIndex, 1);
            wxLogMessage(_T("Item %d removed from index because it exsit allready"), iIndex);
            return true;
        }
    }


    // normal behaviour : copying items
    for (unsigned int i = 0; i < selected->GetCount(); i++) {
        m_SelectedIDs->Add(selected->Item(i));
    }

    //wxLogDebug(_T("%d items added to the memory"), m_SelectedIDs->GetCount());
    return true;

}


/***************************************************************************//**
 @brief Save any geometry into database
 @details This function clear the actual selection and specifiy as the selection
 the passed value
 @param selected The selected value
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
void tmSelectedDataMemory::SetSelected(long selected)
{
    Clear();
    if (selected == wxNOT_FOUND) {
        return;
    }
    m_SelectedIDs->Add(selected);
}


/***************************************************************************//**
 @brief Search for existing item in the array
 @param value value we are searching for
 @return  the 0 based index of the result or wxNOT_FOUND
 @author Lucien Schreiber (c) CREALP 2008
 @date 28 October 2008
 *******************************************************************************/
int tmSelectedDataMemory::Search(const long &value)
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
bool tmSelectedDataMemory::Remove(wxArrayLong *selected)
{
    // clearing if required
    if (!selected) {
        m_SelectedIDs->Clear();
        return TRUE;
    }

    // normal behaviour
    int iIndex = 0;
    for (signed int i = (signed) selected->GetCount() - 1; i >= 0; i--) {
        iIndex = Search(selected->Item(i));
        if (iIndex != wxNOT_FOUND)
            m_SelectedIDs->RemoveAt(iIndex);
        else {
            wxLogDebug(_T("Trying to remove non existring item from memory..."));
        }
    }
    return true;
}


/***************************************************************************//**
 @brief Is this object selected
 @param value the Object ID value we want to check for selected state
 @return  true if the item is selected
 @author Lucien Schreiber (c) CREALP 2008
 @date 28 October 2008
 *******************************************************************************/
bool tmSelectedDataMemory::IsSelected(const long &value)
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


/***************************************************************************//**
 @brief Set on which layer we are working on
 @details This function ensure that only selection in one layer is done : if
 layerID is different from member m_LayerID then it indicates that we are
 trying to operate on another layer and all selected data are removed from the
 selection.
 @param layerID ID of the layer on which we want to operate
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
void tmSelectedDataMemory::SetLayerID(long layerID)
{
    if (layerID != m_LayerID)
        m_SelectedIDs->Clear();

    m_LayerID = layerID;
}


/***************************************************************************//**
 @brief Getting selected values
 @return  Adress of a wxArrayLong. or null in case of error. The returned array
 may be destroyed saftly by the caller.
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
wxArrayLong *tmSelectedDataMemory::GetSelectedValues()
{
    unsigned iNbItems = GetCount();
    if (iNbItems == 0)
        return NULL;

    wxArrayLong *myCopyArray = new wxArrayLong();
    for (unsigned int i = 0; i < iNbItems; i++)
        myCopyArray->Add(m_SelectedIDs->Item(i));

    return myCopyArray;
}


/***************************************************************************//**
 @brief Get the selected value
 @details This function is designed to get the selected value when only one
 object is selected.
 @return  The oid of the selected layer, or -1 if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 February 2009
 *******************************************************************************/
long tmSelectedDataMemory::GetSelectedUnique()
{
    unsigned iNbItems = GetCount();
    if (iNbItems != 1)
        return -1;

    return m_SelectedIDs->Item(0);
}

