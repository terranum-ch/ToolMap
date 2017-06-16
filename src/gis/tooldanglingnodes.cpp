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


#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY (tmArrayDanglingPtsToCheck);


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
	m_GisData.SetDataBaseHandle(m_pDB);
}



void ToolDanglingNodes::DNInitValues()
{
	m_pDB = NULL;
	m_bSearchInited = false;
	m_bSearchRun = false;
	m_LoopNum = 0;
	m_pDlg = NULL;
	m_GeomFrame = NULL;
	m_LayerID = wxNOT_FOUND;
}



bool ToolDanglingNodes::DNIsLayerCorrect(long layerid)
{
	wxASSERT(IsOk());
	
	wxString mySentence = wxString::Format(_T("SELECT TYPE_CD FROM %s")
										   _T(" WHERE LAYER_INDEX = %ld"),
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
		wxLogDebug(_T("Layer with index %ld doesn't exist"), layerid);
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
	_T(" o.OBJECT_ID WHERE o.THEMATIC_LAYERS_LAYER_INDEX =%ld ORDER BY g.OBJECT_ID ");
	
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
		DNSearchCleanUp();
		return false;
	}
	
	m_bSearchInited = true;
	return true;
}



bool ToolDanglingNodes::DNSearchValidVertex()
{
	wxASSERT(IsOk());
	//wxASSERT(m_pDB->DataBaseHasResults());
	wxASSERT(m_GeomFrame);
	wxASSERT(m_PtsToCheck.IsEmpty());

	long myOid = wxNOT_FOUND;

	while (1)
	{
		OGRLineString * myLineToCheck = (OGRLineString*) m_GisData.GetNextDataLine(myOid);	
		if (myLineToCheck == NULL)
			break;
		
		wxASSERT(myLineToCheck);
		OGRPoint  p1;
		OGRPoint  p2;
		
		myLineToCheck->StartPoint(&p1);
		myLineToCheck->EndPoint(&p2);
		
		if (p1.Equals(&p2) ==false)
		{
			
			if(DNIsPointInside(&p1))
				m_PtsToCheck.Add(DanglingPtsToCheck(wxRealPoint(p1.getX(), p1.getY()), myOid));
			
			if(DNIsPointInside(&p2))
				m_PtsToCheck.Add(DanglingPtsToCheck(wxRealPoint(p2.getX(), p2.getY()), myOid));
			
		}
		
		OGRGeometryFactory::destroyGeometry(myLineToCheck);
	}
	
	wxLogDebug(_T("%zu nodes to check"), m_PtsToCheck.GetCount());
	DNSearchCleanUp();
	
	if (m_PtsToCheck.IsEmpty()==true)
	{
		wxLogDebug(_T("No vertex to check"));
		return false;
	}
		
	return true;
}


bool ToolDanglingNodes::DNIsPointInside(OGRPoint * pt) {
    if (m_GeomFrame == NULL) {
        return false;
    }

    if (pt->Within(m_GeomFrame) == true)
        return true;

    // test also for touching
    OGRGeometry *myExtRing = m_GeomFrame->getExteriorRing();
    if (myExtRing == NULL) {
        return false;
    }
    if (pt->Touches(myExtRing) == true) {
        return true;
    }
	
	return false;
}


void ToolDanglingNodes::DNSearchCleanUp ()
{
	wxASSERT(IsOk());
	if(m_pDB->DataBaseHasResults())
		m_pDB->DataBaseClearResults();
	
	if (m_GeomFrame != NULL)
	{
		OGRGeometryFactory::destroyGeometry(m_GeomFrame);
		m_GeomFrame = NULL;
	}
	
	m_bSearchInited = false;
}



bool ToolDanglingNodes::DNGetFrameGeometry()
{
	wxASSERT(m_pDB);
	wxString sSentence = _T("SELECT * FROM ") + TABLE_NAME_GIS_GENERIC[4];
	
	if (m_pDB->DataBaseQuery(sSentence)==false)
	{
		return false;
	}
	
	long loid = 0;
	long myRows = 0;
	if(m_pDB->DataBaseGetResultSize(NULL, &myRows)==true)
	{
		if (myRows > 1)
		{
			wxLogError(_("Too many frame results"));
			m_pDB->DataBaseClearResults();
			return false;
		}
	}
	
	OGRLineString * myFrameLine = m_GisData.GetNextDataLine(loid);
	if(myFrameLine == NULL)
	{
		wxLogError(_("Unable to get the frame"));
		m_pDB->DataBaseClearResults();
		return false;
	}

	m_GeomFrame = (OGRPolygon*) OGRGeometryFactory::forceToPolygon(myFrameLine);
    wxASSERT(m_GeomFrame);

	m_pDB->DataBaseClearResults();
	return true;
	
}



bool ToolDanglingNodes::DNIsSearchInitedOk ()
{
	wxASSERT(IsOk());
	if (m_bSearchInited == false)
	{
		wxLogDebug(_T("Please use searchinit() first"));
		return false;
	}
	
	if (m_GeomFrame == NULL)
	{
		wxLogDebug(_T("No frame detected, searching dangling nodes unavaillable"));
		return false;
	}

	return true;
}


ToolDanglingNodes::~ToolDanglingNodes()
{
	//m_DanglingPts.Clear();
	
	if (m_bSearchInited == true)
		DNSearchCleanUp();
	
	wxASSERT(m_GeomFrame == NULL);
}



bool ToolDanglingNodes::DNFlagNodes ()
{	
	wxASSERT(m_PtsToCheck.GetCount() > 0);
	bool bReturn = true;
	
	
	
	long iNumberLines = wxNOT_FOUND;
	if(m_pDB->DataBaseGetResultSize(NULL, &iNumberLines)==false)
		return false;
	
	
	long myOid = 0;
	int iLoop = 0;
	while (1)
	{
		
		if(DNUpdateProgress(iNumberLines, iLoop)==false)
		{
			bReturn = false;
			break;
		}
		iLoop++;
		
		OGRLineString * myLineToCheck = (OGRLineString*) m_GisData.GetNextDataLine(myOid);	
		if (myLineToCheck == NULL)
			break;
		
		wxASSERT(myLineToCheck);
		OGRPoint  p1;
		OGRPoint  p2;
		
		myLineToCheck->StartPoint(&p1);
		myLineToCheck->EndPoint(&p2);

		
		for (unsigned int i = 0; i<m_PtsToCheck.GetCount();i++)
		{
				
			if (m_PtsToCheck.Item(i).m_LineOID != myOid)
			{
				if (m_PtsToCheck.Item(i).m_Pt.x == p1.getX())
					if (m_PtsToCheck.Item(i).m_Pt.y == p1.getY())
						m_PtsToCheck.Item(i).m_Flaged++;
				
				if (m_PtsToCheck.Item(i).m_Pt.x == p2.getX())
					if (m_PtsToCheck.Item(i).m_Pt.y == p2.getY())
						m_PtsToCheck.Item(i).m_Flaged++;
			}
		}
		OGRGeometryFactory::destroyGeometry(myLineToCheck);

	}
	
	return bReturn;
}



bool ToolDanglingNodes::DNUpdateProgress(unsigned int ptstocheck,unsigned int iloop)
{
	if (m_pDlg == NULL)
		return true;
	
	wxASSERT(iloop <= ptstocheck);
		
	
	// computing percent
	double dloop = iloop;
	double dTotal = ptstocheck;
	double dVal = dloop / dTotal * 100.0;
	
	if (iloop == ptstocheck)
		m_pDlg->Update(m_LoopNum * 100);
	
	if(m_pDlg->Update(int(dVal) + ((m_LoopNum -1) * 100))==false)
	{
		wxLogDebug(_T("Searching dangling nodes stoped by user"));
		return false;
	}
		
	return true;
}



void ToolDanglingNodes::DNParseFlagedPts (wxArrayRealPoints & dpts)

{
	//dpts.Clear();
	for (unsigned int i = 0; i<m_PtsToCheck.GetCount();i++)
	{
		//wxLogDebug(_T("Checking pts : node : %d has %d flags"),
		//		   i,m_PtsToCheck.Item(i).m_Flaged);
		
		if (m_PtsToCheck.Item(i).m_Flaged == 0)
			dpts.Add(m_PtsToCheck.Item(i).m_Pt);
	}
	
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
	if (IsOk()==false)
		return false;
	
	if (m_bSearchRun == false)
	{
		wxLogDebug(_T("Use searchrun() first"));
		return false;
	}
	
	DNParseFlagedPts(pts);

	
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



bool ToolDanglingNodes::SearchInit (long layerid, const wxString & layername)
{
	// ready ?
	if (IsOk() == false)
		return false;
	
	if (m_bSearchInited == true)
		DNSearchCleanUp();
	
	// layer exist and correct format (poly)
	if (DNIsLayerCorrect(layerid)==false)
		return false;
	
	// get frame first
	if (DNGetFrameGeometry()==false)
		return false;
	
	// get all lines
	if (DNGetAllLines(layerid)==false){
        wxLogWarning(_("No object in: '%s' searching for danglings nodes in this layer is impossible"), layername);
        return false;
    }
	
	m_LayerID = layerid;
	m_LoopNum ++; 
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



bool ToolDanglingNodes::SearchRun (wxProgressDialog * myProgDlg)
{
	m_bSearchRun = false;
	if (IsOk() == false)
		return false;
	
	if (DNIsSearchInitedOk()==false)
		return false;
	
	m_pDlg = myProgDlg;
	m_PtsToCheck.Clear();
	if (DNSearchValidVertex()==false){
		// if no vertex, we return true for continuing
		return true;
	}
	
	wxASSERT(m_LayerID != wxNOT_FOUND);
	if (DNGetAllLines(m_LayerID)==false)
	{
		return false;
	}
	
	if(DNFlagNodes()==false)
	   return false;

	m_LayerID = wxNOT_FOUND;
	m_bSearchRun = true;
	return true;
}
