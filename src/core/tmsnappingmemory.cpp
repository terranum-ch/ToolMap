/***************************************************************************
								tmsnappingmemory.cpp
				Store snapping status into memory for faster access
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

#include "tmsnappingmemory.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY (tmSnappingObjArray);


tmSnappingObject::tmSnappingObject()
{
	InitMemberValues();
}

tmSnappingObject::~tmSnappingObject()
{
	
}


void tmSnappingObject::InitMemberValues()
{
	m_LayerID = -1;
	m_SnappingStatus = 0;
}


/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 21 January 2009
 *******************************************************************************/
tmSnappingMemory::tmSnappingMemory()
{
	
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 21 January 2009
 *******************************************************************************/
tmSnappingMemory::~tmSnappingMemory()
{
	
	
}



/***************************************************************************//**
 @brief Add snapping to memory
 @details Store in an array the passed values
 @param lid ID value of the layer (see TOC_ID in the prj_snapping table)
 @param snapstatus snap status : one of the following : 
 - 0 : No snapping 
 - 1: Snapping vertex 
 - 2: Snapping BeginEnd 
 - 3: Snapping both = snapping vertex
 @author Lucien Schreiber (c) CREALP 2009
 @date 21 January 2009
 *******************************************************************************/
void tmSnappingMemory::AddSnappingMemory (long lid, int snapstatus)
{
	tmSnappingObject mySnapObj;
	mySnapObj.m_LayerID = lid;
	mySnapObj.m_SnappingStatus = snapstatus;
	m_Snapping.Add(mySnapObj);
}



/***************************************************************************//**
 @brief Delete a layer from the memory array
 @param lid layer ID
 @return  true if the item was deleted from memory (if the item was found in
 memory). False otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 21 January 2009
 *******************************************************************************/
bool tmSnappingMemory::DeleteSnappingMemory (const long & lid)
{
	int iIndex = FindSnappingItem(lid);
	if (iIndex == wxNOT_FOUND)
		return false;
	
	m_Snapping.RemoveAt(iIndex);
	return true;
}



/***************************************************************************//**
 @brief Get the snapping status for specified layer
 @param lid the layer id (as stored into database)
 @return  one of the snapping status. If the layer dosen't exist into the
 snapping array stored in memory, it will return 0
 @author Lucien Schreiber (c) CREALP 2009
 @date 21 January 2009
 *******************************************************************************/
int tmSnappingMemory::GetSnappingMemoryStatus (const long & lid)
{
	int iIndex = FindSnappingItem(lid);
	if (iIndex == wxNOT_FOUND)
		return tmSNAPPING_OFF;
		
	return m_Snapping.Item(iIndex).m_SnappingStatus;
}


/***************************************************************************//**
 @brief Change, or set snapping to existing layer
 @param lid layer ID to modify
 @param snapstatus the new snapping status
 @return  true if the layer ID exists, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 21 January 2009
 *******************************************************************************/
bool tmSnappingMemory::SetSnappingMemoryStatus  (const long & lid, int snapstatus)
{
	int iIndex = FindSnappingItem(lid);
	if (iIndex == wxNOT_FOUND)
		return false;
	
	m_Snapping.Item(iIndex).m_SnappingStatus = snapstatus;
	return true;
}


/***************************************************************************//**
 @brief Get snapping info by index
 @param iIndex the zero based index of the snapping info we want back
 @param lid the layer id
 @param snapstatus the snapping status
 @author Lucien Schreiber (c) CREALP 2009
 @date 21 January 2009
 *******************************************************************************/
bool tmSnappingMemory::GetSnappingInfo (unsigned int iIndex, long & lid, int & snapstatus)
{
	if (iIndex >= GetCount())
		return false;
	
	lid = m_Snapping.Item(iIndex).m_LayerID;
	snapstatus = m_Snapping.Item(iIndex).m_SnappingStatus;
	return true;
}


/***************************************************************************//**
 @brief Search for specified layer ID in memory array
 @param lid layer ID (see TOC_ID in prj_snapping table)
 @return  the zero based index of the found item or wxNOT_FOUND otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 21 January 2009
 *******************************************************************************/
int tmSnappingMemory::FindSnappingItem (const long & lid)
{
	int iReturn = wxNOT_FOUND;
	for (unsigned int i = 0; i< GetCount(); i++)
	{
		if (m_Snapping.Item(i).m_LayerID == lid)
		{
			iReturn = i;
			break;
		}
	}
	
	return iReturn;
}



/***************************************************************************//**
 @brief Set all snapping status to #tmSNAPPING_OFF
 @details This function doesn't remove layers from the memory
 @author Lucien Schreiber (c) CREALP 2009
 @date 22 January 2009
 *******************************************************************************/
void tmSnappingMemory::ClearSnappingStatus ()
{
	for (unsigned int i = 0; i< GetCount(); i++)
	{
		m_Snapping.Item(i).m_SnappingStatus = tmSNAPPING_OFF;
	}
}

