/***************************************************************************
								tooldanglingnodes.cpp
                    Tool engine for searching dangling nodes
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

#include "tooldanglingnodes.h"


ToolDanglingNodes::ToolDanglingNodes()
{
	DNInitValues();
	
}



ToolDanglingNodes::ToolDanglingNodes(DataBaseTM * database)
{
	DNInitValues();
	Create(database);
}



void ToolDanglingNodes::Create (DataBaseTM * database)
{
	m_pDB = database;
}



void ToolDanglingNodes::DNInitValues()
{
	m_pDB = NULL;
}



bool ToolDanglingNodes::DNIsLayerCorrect(long layerid)
{
	wxASSERT(IsOk());
	
	wxString mySentence = wxString::Format(_T("SELECT TYPE_CD FROM %s")
										   _T(" WHERE LAYER_INDEX = %d"),
										   TABLE_NAME_LAYERS.c_str(),
										   layerid);
	if(m_pDB->DataBaseQuery(mySentence)==false)
	{
		wxFAIL;
		return false;
	}
		
	long myLayerType = 0;
	if (m_pDB->DataBaseGetNextResult(myLayerType)==false)
	{
		m_pDB->DataBaseClearResults();
		wxLogDebug(_T("Layer with index %d doesn't exist"), layerid);
		return false;
	}
	m_pDB->DataBaseClearResults();
	
	if (myLayerType == LAYER_SPATIAL_POLYGON)
		return true;
	
	wxLogDebug(_T("Layer is wrong type"));
	return false;
}



ToolDanglingNodes::~ToolDanglingNodes()
{
	m_DanglingPts.Clear();
}



/***************************************************************************//**
 @brief Get array of dangling nodes
 @param pts will be filled with dangling nodes
 @return  true if there is dangling nodes, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 13 May 2009
 *******************************************************************************/
bool ToolDanglingNodes::GetDanglingNodes(wxArrayRealPoints & pts)
{
	if (IsOk() == false || m_DanglingPts.GetCount() == 0)
		return false;
	
	return true;
}



bool ToolDanglingNodes::IsOk()
{
	if (m_pDB == NULL)
	{
		wxLogDebug(_T("Database not inited in Dangling nodes, init the DB first"));
		return false;
	}
	
	return true;
}



bool ToolDanglingNodes::Search (long layerid)
{
	// ready ?
	if (IsOk() == false)
		return false;
	
	// layer exist and correct format (poly)
	if (DNIsLayerCorrect(layerid)==false)
		return false;
	
	
	
	
	return true;
}
