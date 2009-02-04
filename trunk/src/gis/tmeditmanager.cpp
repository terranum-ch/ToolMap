/***************************************************************************
								tmeditmanager.cpp
				Deals with editing data in construction layers
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

#include "tmeditmanager.h"


BEGIN_EVENT_TABLE(tmEditManager, wxEvtHandler)
	EVT_COMMAND (wxID_ANY, tmEVT_SNAPPING_UPDATED, tmEditManager::OnSnappingChange)
	EVT_COMMAND (wxID_ANY, tmEVT_VIEW_REFRESHED, tmEditManager::OnViewUpdated)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_DRAW_CLICK, tmEditManager::OnDrawClicked)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_EDIT_START, tmEditManager::OnEditStart)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_EDIT_STOP, tmEditManager::OnEditStop)
END_EVENT_TABLE()



/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
tmEditManager::tmEditManager(wxWindow * parent,tmTOCCtrl * toc,
							 tmSelectedDataMemory * seldata,
							 tmRenderer * renderer,
							 tmGISScale * scale)
{
	InitMemberValues();
	m_ParentEvt = parent;
	m_TOC = toc;
	m_SelectedData = seldata;
	m_Renderer = renderer;
	m_Scale = scale;
	m_EditStarted = false;

	m_ParentEvt->PushEventHandler(this);

	m_GISMemory = new tmGISDataVectorMemory();
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
tmEditManager::~tmEditManager()
{
	m_ParentEvt->PopEventHandler(false);
	m_ParentEvt->SetEventHandler(m_ParentEvt);
	
	delete m_GISMemory;
}



/***************************************************************************//**
 @brief Init pointer to default values
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmEditManager::InitMemberValues()
{
	m_ParentEvt = NULL;
	m_TOC = NULL;
	m_SelectedData = NULL;
	m_Renderer = NULL;
	m_pDB = NULL;
	m_SnapMem = NULL;
}



/***************************************************************************//**
 @brief User select edit tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmEditManager::OnToolEdit ()
{
	wxASSERT (m_Renderer);
	m_Renderer->SetTool(tmTOOL_DRAW);
}



/***************************************************************************//**
 @brief User select modify tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmEditManager::OnToolModify ()
{
	wxASSERT (m_Renderer);
	m_Renderer->SetTool(tmTOOL_MODIFY);
}



/***************************************************************************//**
 @brief Called when snapping change
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmEditManager::OnSnappingChange (wxCommandEvent & event)
{
	m_SnapMem = (tmSnappingMemory*) event.GetClientData();
	wxASSERT (m_SnapMem);
	DisplayRendererSnappingTolerence();
}



/***************************************************************************//**
 @brief Called when the view is updated
 @details After a zoom, pan or when loading the project
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 January 2009
 *******************************************************************************/
void tmEditManager::OnViewUpdated (wxCommandEvent & event)
{
	DisplayRendererSnappingTolerence();
	wxLogDebug(_T("View updated"));
	
	// draw memory line
	DrawEditLine();
}



/***************************************************************************//**
 @brief Display snapping if snapping enabled
 @details This function display a video inverse circle if snapping is enabled
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 January 2009
 *******************************************************************************/
void tmEditManager::DisplayRendererSnappingTolerence()
{
	if (m_SnapMem && m_Scale)
	{
		int iSnapRadius = 0;
		if (m_SnapMem->IsSnappingEnabled())
		{
			iSnapRadius = m_Scale->DistanceToReal(m_SnapMem->GetTolerence());
		}
		m_Renderer->ToogleSnapping(iSnapRadius);
	}
	else
		wxLogDebug(_T("Snapping memory or scale not defined"));
}



/***************************************************************************//**
 @brief Checks that a layer is selected
 @details 
 @return  true if : a layer is defined as editing layer 
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
bool tmEditManager::IsCorrectLayerSelected()
{
	wxASSERT(m_TOC);
	if (!m_TOC->GetEditLayer())
	{
		wxLogMessage(_("No layer selected, select a layer for drawing"));
		return false;
	}
		
	return true;
}


/***************************************************************************//**
 @brief Checks that an object is selected
 @details 
 @return  true if an object is selected, false if no object or more than one
 objects are selected
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
bool tmEditManager::IsObjectSelected()
{
	if (m_SelectedData->GetCount() == 1)
		return true;
	
	return false;
}


/***************************************************************************//**
 @brief Are we ready for drawing
 @details 
 @return  True if all validation are ok for editing
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
bool tmEditManager::IsDrawingAllowed()
{
	if (IsCorrectLayerSelected() && m_EditStarted)
		return true;
	
	return false;
}



/***************************************************************************//**
 @brief Are we ready for drawing
 @details 
 @return  True if all validation are ok for editing
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
bool tmEditManager::IsModifictionAllowed()
{
	if (IsCorrectLayerSelected() && IsObjectSelected())
		return true;
		
	return false;
}



/***************************************************************************//**
 @brief Called when a click is issued with Draw tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
void tmEditManager::OnDrawClicked (wxCommandEvent & event)
{
	// get coordinate and dont forget to delete it
	wxPoint * myPxCoord = (wxPoint*) event.GetClientData();
	wxRealPoint myRealCoord = m_Scale->PixelToReal(*myPxCoord);
	wxRealPoint * mySnapCoord = NULL;
	
	
	// check drawing allowed
	if (!IsDrawingAllowed())
		return;
		

	// check snapping if needed
	//TODO: add support for stopping snapping with keyboard 
	if (m_SnapMem->IsSnappingEnabled())
	{
		mySnapCoord = IterateAllSnappingLayers(myRealCoord);
		
		//TODO: Remove this temp logging code
		if (mySnapCoord)
			wxLogDebug(_T("Point found for snapping @ : %.*f, %.*f"),
					   2, mySnapCoord->x, 
					   2, mySnapCoord->y);
		else
			wxLogDebug(_T("No snapping found"));
		// END of temp logging code
		
		if (mySnapCoord)
		{
			myRealCoord.x = mySnapCoord->x;
			myRealCoord.y = mySnapCoord->y;
			delete mySnapCoord;
		}
	}
	
	// add  line vertex
	if (m_TOC->GetEditLayer()->m_LayerSpatialType == LAYER_SPATIAL_LINE)
	{
		AddLineVertex(myRealCoord);
		DrawLastSegment();
	}
	else // add point 
	{
		AddPointVertex(myRealCoord);
	}
	
	delete myPxCoord;
}



/***************************************************************************//**
 @brief Add a vertex for line
 @details store the line vertex in memory
 @param pt the coordinate to store in memory
 @return  true if vertex was stored in memory
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
bool tmEditManager::AddLineVertex (const wxRealPoint & pt)
{
	bool bReturn = m_GISMemory->InsertVertex(pt, -1);
	wxLogDebug(_T("Number of stored vertex : %d"), m_GISMemory->GetVertexCount());

	return bReturn;
}


/***************************************************************************//**
 @brief Draw and store the point into database
 @param pt the point (real)
 @return  true if the point was successfully added to the database
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
bool tmEditManager::AddPointVertex (const wxRealPoint & pt)
{
	bool bReturn = true;
	// draw the point
	tmDrawer myEditDrawer;
	myEditDrawer.InitDrawer(m_Renderer->GetBitmap(), 
							*m_Scale, m_Scale->GetWindowExtentReal());
	
	// get the symbology
	tmSymbolVectorPoint * mySymbol = (tmSymbolVectorPoint*) m_TOC->GetEditLayer()->m_LayerSymbol;
	
	// draw the vertex in selected colour
	myEditDrawer.DrawEditVertex(pt, mySymbol->GetRadius(),
								*wxRED);

	
	//TODO: draw the previous vertex in normal colour (blue) 
	
	// store the vertex into database
	long lpOid = StorePoint(pt);
	if (lpOid == -1)
	{
		wxLogError(_T("Error inserting point into database"));
		return false;
	}
	
	// select the last inserted oid
	m_SelectedData->SetLayerID(m_TOC->GetEditLayer()->m_LayerID);
	m_SelectedData->SetSelected(lpOid);
	wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt);
	
	//TODO: Remove this temp code
	wxLogDebug(_T("Selected OID = %d"), lpOid);
	
	
	m_Renderer->Refresh();
	m_Renderer->Update();
	return bReturn;
}



/***************************************************************************//**
 @brief Save the passed point to the database
 @param pt the point (real)
 @return  the OID of the stored point, -1 if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
long tmEditManager::StorePoint (const wxRealPoint & pt)
{
	// get table
	tmLayerProperties * layerprop = m_TOC->GetEditLayer();
	if (layerprop == NULL)
		return -1;
	
	tmGISDataVectorMemory myTempPoint;
	myTempPoint.CreateFeature();
	myTempPoint.InsertVertex(pt);
	long lid = myTempPoint.SavePointToDatabase(m_pDB, layerprop->m_LayerType);
	
	return lid;
}


/***************************************************************************//**
 @brief Directly draw the last segment
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
void tmEditManager::DrawLastSegment ()
{
	wxRealPoint LastRealPt(0,0), LastLastRealPt(0,0);
	
	// init a drawer 
	tmDrawer myEditDrawer;
	myEditDrawer.InitDrawer(m_Renderer->GetBitmap(), 
							*m_Scale, m_Scale->GetWindowExtentReal());
	
	// get the symbology
	tmSymbolVectorLine * mySymbol = (tmSymbolVectorLine*) m_TOC->GetEditLayer()->m_LayerSymbol;
	
	// get two last vertex 
	m_GISMemory->GetVertex(LastRealPt, -1);
	m_GISMemory->GetVertex(LastLastRealPt, m_GISMemory->GetVertexCount()-2);
	
	myEditDrawer.DrawEditSegment(LastLastRealPt,
								LastRealPt, 
								mySymbol->GetWidth());
	m_Renderer->Refresh();
	m_Renderer->Update();
}



/***************************************************************************//**
 @brief Draw again the line in editing when image reloaded
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
void tmEditManager::DrawEditLine ()
{
	// check edit memory data for drawing
	int iNbVertexMemory = m_GISMemory->GetVertexCount();
	if (iNbVertexMemory == 0)
		return;
	
	// init a drawer 
	tmDrawer myEditDrawer;
	myEditDrawer.InitDrawer(m_Renderer->GetBitmap(), 
							*m_Scale, m_Scale->GetWindowExtentReal());
	// get the symbology
	tmSymbolVectorLine * mySymbol = (tmSymbolVectorLine*) m_TOC->GetEditLayer()->m_LayerSymbol;

	
	
	// get all vertex 
	wxRealPoint * myRealPts = new wxRealPoint[iNbVertexMemory];
	for (int i = 0; i<iNbVertexMemory; i++)
	{
		m_GISMemory->GetVertex(myRealPts[i], i);
	}
	
	myEditDrawer.DrawEditLine(myRealPts, iNbVertexMemory,
							  mySymbol->GetWidth());
	
	m_Renderer->Refresh();
	m_Renderer->Update();

}


/***************************************************************************//**
 @brief Called when TOC start editing
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
void tmEditManager::OnEditStart (wxCommandEvent & event)
{
	m_GISMemory->CreateFeature();
	m_EditStarted = true;
	event.Skip();
}


/***************************************************************************//**
 @brief Called when TOC stop editing
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
void tmEditManager::OnEditStop (wxCommandEvent & event)
{
	m_GISMemory->DestroyFeature();
	m_EditStarted = false;
	event.Skip();
}



/***************************************************************************//**
 @brief Iterate all layers in snapping memory
 @details Try to get the snapped coordinate for the clicked point
 @param clickedpoint The real coordinate of the clicked point
 @return  a valid wxRealPoint if snapped point found, null otherwise (don't
 forget to delete)
 @author Lucien Schreiber (c) CREALP 2009
 @date 30 January 2009
 *******************************************************************************/
wxRealPoint * tmEditManager::IterateAllSnappingLayers(const wxRealPoint & clickedpoint)
{
	long myLayerId = 0;
	int mySnapStatus = tmSNAPPING_OFF;
	tmLayerProperties * myActualLayer = NULL;
	
	wxRealPoint * myReturnedSnappedPoint = NULL;
	wxRealPoint mySnapPoint;
	
	for (unsigned int i = 0; i< m_SnapMem->GetCount(); i++)
	{
		m_SnapMem->GetSnappingInfo(i, myLayerId, mySnapStatus);
		myActualLayer = m_TOC->GetLayerById(myLayerId);
		if (!myActualLayer)
			break;
		
		// search snapping for that layer
		tmGISData * myActualGISData = tmGISData::LoadLayer(myActualLayer);
		if (!myActualGISData)
			break;
				
		if (myActualGISData->GetSnapCoord(clickedpoint,
									  m_SnapMem->GetTolerence(),
									  mySnapPoint,mySnapStatus))
		{
			myReturnedSnappedPoint = new wxRealPoint(mySnapPoint);
			delete myActualGISData;
			break;
		}
		delete myActualGISData;
	}
	return myReturnedSnappedPoint;
}


