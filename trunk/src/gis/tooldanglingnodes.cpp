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
	m_bSearchDone = false;
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



bool ToolDanglingNodes::DNGetAllLines(long layerid)
{
	wxASSERT(IsOk());
	
	
	wxString sTemp = _T("SELECT g.OBJECT_ID, g.OBJECT_GEOMETRY")
	_T(" FROM  %s g LEFT JOIN %s a ON (g.OBJECT_ID")
	_T(" = a.OBJECT_GEOM_ID) LEFT JOIN %s o ON a.OBJECT_VAL_ID =")
	_T(" o.OBJECT_ID WHERE o.THEMATIC_LAYERS_LAYER_INDEX =%d ORDER BY g.OBJECT_ID ");
	
	wxString sSentence = wxString::Format(sTemp,
										  TABLE_NAME_GIS_GENERIC[0].c_str(), // lines
										  TABLE_NAME_GIS_ATTRIBUTION[0].c_str(), // lines attrib.
										  TABLE_NAME_OBJECTS.c_str(),
										  layerid);

	if(m_pDB->DataBaseQuery(sSentence)==false)
	{
		wxLogDebug(_T("Sentence is wrong : %s"),sSentence.c_str());
		return false;
	}
	
	
	if (m_pDB->DataBaseHasResults()==false)
	{
		wxLogDebug(_T("No object for searching dangling nodes"));
		m_pDB->DataBaseClearResults();
		return false;
	}
	
	return true;
}



bool ToolDanglingNodes::DNProcessSearchResults()
{
	wxASSERT(IsOk());
	wxASSERT(m_pDB->DataBaseHasResults());
	
	
	
	m_pDB->DataBaseClearResults();
	return true;
}


void ToolDanglingNodes::DNSearchCleanUp ()
{
	wxASSERT(IsOk());
	wxASSERT(m_pDB->DataBaseHasResults());
	m_pDB->DataBaseClearResults();
	m_bSearchDone = false;
}


bool ToolDanglingNodes::DNIsSearchInitedOk ()
{
	wxASSERT(IsOk());
	if (m_bSearchDone == false)
	{
		wxLogDebug(_T("Please use searchinit() first"));
		return false;
	}

	return true;
}


ToolDanglingNodes::~ToolDanglingNodes()
{
	m_DanglingPts.Clear();
	
	if (m_bSearchDone == true)
		DNSearchCleanUp();
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



bool ToolDanglingNodes::SearchInit (long layerid)
{
	// ready ?
	if (IsOk() == false)
		return false;
	
	if (m_bSearchDone == true)
		DNSearchCleanUp();
	
	// layer exist and correct format (poly)
	if (DNIsLayerCorrect(layerid)==false)
		return false;
	
	// search
	if (DNGetAllLines(layerid)==false)
		return false;
	
	m_bSearchDone = true;
	return true;
}


bool ToolDanglingNodes::SearchInfo (int & numberlines)
{
	if (IsOk()== false)
		return false;
	
	if (DNIsSearchInitedOk() == false)
		return false;
	
	numberlines = 0;
	long numrows = 0;
	bool bsize = m_pDB->DataBaseGetResultSize(NULL, &numrows);
	wxASSERT(bsize);
	wxASSERT(numrows > 0);
	
	numberlines = int(numrows);
	return true;
}

