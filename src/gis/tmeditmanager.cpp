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
	EVT_COMMAND (wxID_ANY, tmEVT_EM_DRAW_ENTER, tmEditManager::OnDrawFeatureStop)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_CUT_LINE, tmEditManager::OnCutLines)
	EVT_COMMAND (wxID_ANY,tmEVT_EV_DISPLAY_VERTEX_COORD, tmEditManager::OnShowVertexPosition)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_MODIFY_CLICK, tmEditManager::OnModifySearch)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_MODIFY_MOVED, tmEditManager::OnModifyMove)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_MODIFY_UP, tmEditManager::OnModifyUp)
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
	m_OldVertexPos = wxDefaultPosition;

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
	if (IsModifictionAllowed()==false)
		return;
	
	m_Renderer->SetTool(tmTOOL_MODIFY);
	
	long myActualSel = m_SelectedData->GetSelectedUnique();
	tmLayerProperties * mypLayerProp = m_TOC->GetEditLayer();
	wxASSERT(myActualSel != wxNOT_FOUND);
	wxASSERT(mypLayerProp);
	bool bCopy = m_GISMemory->GetLineFromDatabase(m_pDB, myActualSel,
												  mypLayerProp->m_LayerType);
	wxASSERT(bCopy);
	m_GISMemory->SetOID(myActualSel);
}



/***************************************************************************//**
 @brief Set tool to cut line tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void tmEditManager::OnToolCutLines ()
{
	wxASSERT(m_Renderer);
	m_Renderer->SetTool(tmTOOL_CUT_LINES);
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
 @brief Check that a layer of type specified is selected
 @param layertype type of layer. Allowed values are : 
 - LAYER_SPATIAL_LINE
 - LAYER_SPATIAL_POINT
 - LAYER_SPATIAL_POLYGON
 @return  true if seleced layer is of specified type
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 February 2009
 *******************************************************************************/
bool tmEditManager::IsLayerTypeSelected (int layertype)
{
	wxASSERT(m_TOC);
	tmLayerProperties * myEditLayer = m_TOC->GetEditLayer();
	if (myEditLayer == NULL)
	{
		wxLogMessage(_("No editing layer selected. Define an edit layer"));
		return false;
	}
	
	
	if (m_TOC->GetEditLayer()->m_LayerType != layertype)
	{
		wxLogMessage(_("Layer isn't of correct type.") + 
					 wxString::Format(_("Please select a layer of type %s"),
					 TM_GIS_SPATIAL_TYPES_STRING[layertype].c_str()));
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
	unsigned int mySelCount = m_SelectedData->GetCount();
	
	if (mySelCount == 1)
		return true;
	
	wxLogMessage(_("%d object(s) selected, expected 1"), mySelCount);
	return false;
}



/***************************************************************************//**
 @brief Check that minimum x objects are selected
 @param iNumbermin Number of minimum objects to be selected
 @return  true if minimum x objects are selected
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 February 2009
 *******************************************************************************/
bool tmEditManager::IsObjectMinNumberSelected (unsigned int iNumbermin)
{
	if (m_SelectedData->GetCount() >= iNumbermin)
		return true;
	
	wxLogDebug(_T("%d item selected, expected : %d"), m_SelectedData->GetCount(),
			   iNumbermin);
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
	if (IsCorrectLayerSelected() == false)
		return false;
	
	if ( IsObjectSelected() == false)
		return false;
	
	if (m_GISMemory->GetVertexCount()!= 0)
	{
		wxLogDebug(_T("Finish actual line first !"));
			return false;
	}
	
	return true;
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
	
	// display old selected line when first vertex is called
	if (m_GISMemory->GetVertexCount() == 0)
	{
		tmDrawer myEditDrawer;
		myEditDrawer.InitDrawer(m_Renderer->GetBitmap(), 
								*m_Scale, m_Scale->GetWindowExtentReal());
		
		// get the symbology
		tmSymbolVectorLine * mySymbol = (tmSymbolVectorLine*) m_TOC->GetEditLayer()->m_LayerSymbol;
		
		// draw the selected in normal colour (blue)
		tmGISDataVectorMemory myGISMem;
		myGISMem.CreateFeature();
		wxArrayLong * mySelArray = m_SelectedData->GetSelectedValues();
		if (mySelArray != NULL)
		{
			for (unsigned int k = 0; k<mySelArray->GetCount();k++)
			{
				if (myGISMem.GetLineFromDatabase(m_pDB, 
												  mySelArray->Item(k),
												  m_TOC->GetEditLayer()->m_LayerType) == false)
					break;
				wxArrayRealPoints myPts;
				if(myGISMem.GetVertexAll(myPts) == false)
					break;
				
				myEditDrawer.DrawEditLine(myPts, 
										  mySymbol->GetWidth(),
										  mySymbol->GetColour());
			}
			if (mySelArray)
				delete mySelArray;
		}

	}
	
	
	
	
	
	
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

	// store the vertex into database
	long lpOid = StorePoint(pt);
	if (lpOid == -1)
	{
		wxLogError(_T("Error inserting point into database"));
		return false;
	}
	
	// draw the selected in normal colour (blue)
	tmGISDataVectorMemory myGISMem;
	wxArrayLong * mySelArray = m_SelectedData->GetSelectedValues();
	if (mySelArray != NULL)
	{
		for (unsigned int k = 0; k<mySelArray->GetCount();k++)
		{
			wxRealPoint myRealPt = myGISMem.GetPointFromDatabase(m_pDB, mySelArray->Item(k),
																 m_TOC->GetEditLayer()->m_LayerType);
			
			if (myRealPt == wxRealPoint(-1,-1))
				break;
			
			myEditDrawer.DrawEditVertex(myRealPt, mySymbol->GetRadius()+1,
										mySymbol->GetColour());
			
		}
		delete mySelArray;
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
 @brief Store the line in memory into database
 @details @warning No verification are done internally, make all checks before
 calling this function
 @return the OID of the line inserted or -1 if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
long tmEditManager::StoreLine ()
{
	tmLayerProperties * layerprop = m_TOC->GetEditLayer();
	if (layerprop == NULL)
		return -1;
	
	return m_GISMemory->SaveLineToDatabase(m_pDB, layerprop->m_LayerType);
}



/***************************************************************************//**
 @brief Update line in database
 @details This function works only if tmGISDataVectorMemory object contain a
 valid OID
 @param bool true if updating was successfull
 @author Lucien Schreiber (c) CREALP 2009
 @date 29 April 2009
 *******************************************************************************/
bool tmEditManager::UpdateLine()
{
	tmLayerProperties * layerprop = m_TOC->GetEditLayer();
	wxASSERT (layerprop);
	if (layerprop == NULL)
		return false;
	
	bool bContainOID = m_GISMemory->IsUpdating();
	wxASSERT(bContainOID);
	if (bContainOID == false)
		return false;
	
	return m_GISMemory->UpdateLineToDatabase(m_pDB, layerprop->m_LayerType);
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
	wxArrayRealPoints myRealPts;
	wxRealPoint myActualPoint;
	for (int i = 0; i<iNbVertexMemory; i++)
	{
		m_GISMemory->GetVertex(myActualPoint, i);
		myRealPts.Add(myActualPoint);
	}
	
	myEditDrawer.DrawEditLine(myRealPts,
							  mySymbol->GetWidth());
	
	//m_Renderer->Refresh();
	//m_Renderer->Update();

}



/***************************************************************************//**
 @brief Search vertex into memory arround specified point
 @author Lucien Schreiber (c) CREALP 2009
 @date 29 April 2009
 *******************************************************************************/
//bool tmEditManager::SearchModifyVertex (const wxRealPoint & clickedpoint,
	//									wxRealPoint & returned)
//{
//	returned = wxRealPoint(0,0);
	
	
///	return true;
//}




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
 @brief Called when user press enter in the #tmRenderer
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void tmEditManager::OnDrawFeatureStop (wxCommandEvent & event)
{
	if (IsDrawingAllowed() == false)
		return;
	
	// minimum 2 vertex for saving line into database
	if (m_GISMemory->GetVertexCount() < 2) 
	{
		m_GISMemory->DestroyFeature();
		m_GISMemory->CreateFeature();
		return;
	}
	
	long lid = wxNOT_FOUND;
	// UPDATING
	if (m_GISMemory->IsUpdating())
	{
		lid = m_SelectedData->GetSelectedUnique();
		bool bUpdate = UpdateLine();
		wxASSERT(bUpdate);
	}
	else // CREATING NEW
	{
		lid = StoreLine();
		if (lid == -1)
		{
			wxLogDebug(_T("Line not saved into database"));
			return;
		}
		
	}

	wxLogDebug(_T("Line saved : OID = %d"), lid);
	
	// Clear memory
	m_GISMemory->DestroyFeature();
	m_GISMemory->CreateFeature();
	
	// set selection
	m_SelectedData->SetLayerID(m_TOC->GetEditLayer()->m_LayerID);
	m_SelectedData->SetSelected(lid);
	wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt);
	
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);

}


/***************************************************************************//**
 @brief Called if user try to cut lines
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void tmEditManager::OnCutLines (wxCommandEvent & event)
{
	// get mouse position
	wxPoint * mypCutPos = (wxPoint*) event.GetClientData();
	wxPoint myCutPos = wxPoint(*mypCutPos);	
	delete mypCutPos;
	
	// change cursor
	m_Renderer->SetTool(tmTOOL_SELECT);
	
	// some checks (1 object selected)
	if (IsModifictionAllowed() == false)
	{
		wxLogDebug(_T("Unable to cut line, select a line first"));
		return;
	}
	if (m_TOC->GetEditLayer()->m_LayerSpatialType != LAYER_SPATIAL_LINE)
		return;
	
	
	wxLogDebug(_T("Ok for cutting line @ %d / %d"),myCutPos.x, myCutPos.y);
	
	
	// Get the selected line 
	tmGISDataVector * mySelLayer = (tmGISDataVector*) tmGISData::LoadLayer(m_TOC->GetEditLayer());
	if (!mySelLayer)
		return;
	
	// get the geometry of selected line to cut
	OGRLineString * myLine = (OGRLineString*) mySelLayer->GetGeometryByOID(m_SelectedData->GetSelectedUnique());
	if (!myLine)
		return;
	
	// create buffer for point
	int myRadius = tmSELECTION_DIAMETER;
	wxRect myClickRect = wxRect (myCutPos.x - myRadius,
								 myCutPos.y - myRadius,
								 tmSELECTION_DIAMETER * 2,
								 tmSELECTION_DIAMETER * 2);
	tmRealRect myClickReal = m_Scale->PixelsToReal(myClickRect);
	OGRGeometry * myClickBuffer =  tmGISDataVector::CreateOGRGeometry(myClickReal);
	if (!myClickBuffer)
		return;
		
	OGRLineString myLine1;
	OGRLineString myLine2;
	
	wxRealPoint myClickedPoint = m_Scale->PixelToReal(myCutPos);
	bool bCut = mySelLayer->CutLineGeometry(myLine, myClickBuffer,myClickedPoint,
							   myLine1, myLine2);
	OGRGeometryFactory::destroyGeometry(myLine);
	OGRGeometryFactory::destroyGeometry (myClickBuffer);
	
	if (bCut == false)
		return;
	
	// update and insert geometry 
	mySelLayer->UpdateGeometry(&myLine1, m_SelectedData->GetSelectedUnique());
	mySelLayer->AddGeometry(&myLine2, 0);
	
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
}



/***************************************************************************//**
 @brief Called to display vertex position
 @details Respond to tmEVT_EV_DISPLAY_VERTEX_COORD event.
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
void tmEditManager::OnShowVertexPosition (wxCommandEvent & event)
{
	wxRealPoint * myPt = (wxRealPoint*) event.GetClientData();
	if (myPt == NULL)
		return;
	
	wxLogDebug(_T("Showing vertex @ %.*f / %.*f"), 2, myPt->x, 2, myPt->y);
	
	// convert to px
	wxPoint myPxPt = m_Scale->RealToPixel(*myPt);
	delete myPt;
	
	if (m_OldVertexPos != wxDefaultPosition && m_OldVertexPos != myPxPt)
		m_Renderer->DrawCircleVideoInverse(m_OldVertexPos, 7);
		
	m_Renderer->DrawCircleVideoInverse(myPxPt, 7);
	m_OldVertexPos = myPxPt;

}


/***************************************************************************//**
 @brief Called to when modify is clicked down for searching vertex
 @details Respond to tmEVT_EM_MODIFY_CLICK event.
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 April 2009
 *******************************************************************************/
void tmEditManager::OnModifySearch (wxCommandEvent & event)
{
	// getting point
	wxPoint * myTempPt = (wxPoint*) event.GetClientData();
	wxASSERT (myTempPt);
	wxRealPoint myRPt = m_Scale->PixelToReal(*myTempPt);
	//wxLogDebug(_T("Searching vertex @ %.*f / %.*f"), 2, myRPt.x, 2, myRPt.y);
	
	// getting symbology
	tmSymbolVectorLine * myLSymbol = 
	(tmSymbolVectorLine*) m_TOC->GetEditLayer()->m_LayerSymbol;
	m_DrawLine.SetSymbology(myLSymbol->GetColour(), myLSymbol->GetWidth());
	
	// searching vertex
	int iIndex = wxNOT_FOUND;
	if (m_GISMemory->SearchVertex(myRPt, iIndex, tmSELECTION_DIAMETER)==false)
	{
		wxLogDebug(_T("No Vertex Found"));
		m_Renderer->StopModifyEvent();
		delete myTempPt;
		return;
	}
	
	wxPoint * myLeft = NULL;
	wxPoint * myRight = NULL;
	wxPoint myPoint;
	
	wxRealPoint myPtToConvert;
	bool bGetPoint = m_GISMemory->GetVertex(myPtToConvert, iIndex);
	wxASSERT(bGetPoint);
	myPoint = m_Scale->RealToPixel(myPtToConvert);
	
	if (m_GISMemory->GetVertex(myPtToConvert, iIndex + 1)==true)
	{
		myRight = new wxPoint(m_Scale->RealToPixel(myPtToConvert));
	}
	
	if (m_GISMemory->GetVertex(myPtToConvert, iIndex -1)==true)
	{
		myLeft = new wxPoint (m_Scale->RealToPixel(myPtToConvert));
	}
	
	
	// converting
	bool bCreateDrawerLine = m_DrawLine.CreateVertex(myPoint, myLeft, myRight, iIndex);
	wxASSERT(bCreateDrawerLine);
	
	if (myLeft != NULL)
		delete myLeft;
	
	if (myRight != NULL)
		delete myRight;
	
	delete myTempPt;
}



void tmEditManager::OnModifyMove (wxCommandEvent & event)
{
	wxPoint * myPt = (wxPoint*) event.GetClientData();
	wxASSERT (myPt);
	
	wxClientDC dc (m_Renderer);
	bool BDraw = m_DrawLine.DrawEditPart(&dc);
	wxASSERT(BDraw);
	bool bSetVertex = m_DrawLine.SetVertex(*myPt);
	wxASSERT(bSetVertex);
	
	BDraw = m_DrawLine.DrawEditPart(&dc);
	wxASSERT(BDraw);
	
	delete myPt;
}


void tmEditManager::OnModifyUp (wxCommandEvent & event)
{
	wxPoint * myPt = (wxPoint*) event.GetClientData();
	wxASSERT (myPt);
	
	//TODO: check snapping here

	bool bSetVertex = m_DrawLine.SetVertex(*myPt);
	wxASSERT(bSetVertex);

	wxRealPoint myRPt = m_Scale->PixelToReal(*myPt);
	bool BSave = m_GISMemory->SetVertex(myRPt, m_DrawLine.GetVertexIndex());
	wxASSERT(BSave);
	
	
	m_Renderer->Refresh();
	m_Renderer->Update();
	DrawEditLine();
	
}


/***************************************************************************//**
 @brief Search function
 @details This function is widly inspired from #tmLayerManager
 @param screenpt Coordinate of the clicked point
 @return  true if an object was found
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 February 2009
 *******************************************************************************/
bool tmEditManager::SelectedSearch (const wxPoint & screenpt)
{
	wxASSERT(IsDrawingAllowed());
	tmLayerProperties * pLayerprop = m_TOC->GetEditLayer();
	wxASSERT(pLayerprop);
	
	// create rect from point
	int myRadius = tmSELECTION_DIAMETER / 2;
	wxRect myClickRect = wxRect (screenpt.x - myRadius,
								 screenpt.y - myRadius,
								 tmSELECTION_DIAMETER,
								 tmSELECTION_DIAMETER);
	
	tmRealRect myClickReal = m_Scale->PixelsToReal(myClickRect);
	tmGISData * myLayerData = tmGISData::LoadLayer(pLayerprop);
	wxASSERT (myLayerData);
	if (!myLayerData)
		return false;
	
	
	
	wxArrayLong * myArray = myLayerData->SearchData(myClickReal, pLayerprop->m_LayerType);
	if (!myArray)
		return false;
	
	int myArrayCount = myArray->GetCount();
	if (myArrayCount == 0)
	{
		wxLogDebug(_T("No data found at specified coordinates"));
		return false;
	}
	wxLogDebug(_T("Number of features selected : %d"), myArrayCount);
	
	
	m_SelectedData->SetLayerID(pLayerprop->m_LayerID);
	m_SelectedData->Clear();
	m_SelectedData->AddSelected(myArray);
	myArray->Clear();
	delete myArray;
	
	return true;
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



/***************************************************************************//**
 @brief Delete the geometry and attribution of selected object
 @param Clearselection true if we sould clear selection ids, if you need to
 delete more stuff (attribution for exemple) set to false
 @return true if geometry are deleted
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
bool tmEditManager::DeleteSelected(bool Clearselection)
{
	// make some checks 
	if (!IsDrawingAllowed() || m_SelectedData->GetCount() <= 0)
		return false;
	
	// delete ids from database
	wxArrayLong * mySelectedIds = m_SelectedData->GetSelectedValues();
	if (mySelectedIds == NULL)
		return false;
	m_pDB->DeleteGeometry(mySelectedIds, m_TOC->GetEditLayer()->m_LayerType);
	m_pDB->DeleteAttribution(mySelectedIds, m_TOC->GetEditLayer()->m_LayerType);
	delete mySelectedIds;
	
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	
	return true;
}



/***************************************************************************//**
 @brief Remove the last vertex from memory
 @details This function will only work while editing lines, calling this for
 points wont work
 @return  true if the last vertex was successfully removed
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
bool tmEditManager::UndoLastVertex ()
{
	// some checks
	if (!IsDrawingAllowed() || m_GISMemory->GetVertexCount() == 0)
		return false;
	
	// remove last vertex
	m_GISMemory->RemoveVertex(-1);
	
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	
	return true;
}



/***************************************************************************//**
 @brief Create intersections
 @details between the selected line and all crossed lines.
 @return  true if all works, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 09 February 2009
 *******************************************************************************/
bool tmEditManager::CreateIntersections ()
{
	// checks (editing mode and 1 line selected)
	if (!IsLayerTypeSelected(LAYER_SPATIAL_LINE) || !IsObjectMinNumberSelected(1))
		return false;

	// Get the Layer (Line MySQL) 
	tmGISDataVector * mySelLayer = (tmGISDataVector*) tmGISData::LoadLayer(m_TOC->GetEditLayer());
	if (!mySelLayer)
		return false;
	
	OGRLineString * myOGRSelLine = (OGRLineString*) mySelLayer->GetGeometryByOID(m_SelectedData->GetSelectedUnique());
	if (!myOGRSelLine)
		return false;
	
	// Get all line crossing
	wxArrayLong * myLinesCrossing = mySelLayer->SearchIntersectingGeometry(myOGRSelLine);
	if (myLinesCrossing == NULL)
	{
		OGRGeometryFactory::destroyGeometry(myOGRSelLine);
		wxLogDebug(_T("No intersections found"));
		return false;
	}
	OGRMultiLineString LinesCrossing;
	for (unsigned int j = 0; j<myLinesCrossing->GetCount();j++)
	{
		OGRGeometry * myLine = mySelLayer->GetGeometryByOID(myLinesCrossing->Item(j));
		wxASSERT(myLine);
		LinesCrossing.addGeometry(myLine);
		OGRGeometryFactory::destroyGeometry(myLine);
	}
	
		
	// for selected lines, compute all intersections (with all lines)
	wxArrayLong myInsertedIDs1;
	OGRMultiLineString selectedsegments;
	mySelLayer->CutLineMultiple(myOGRSelLine, &LinesCrossing, selectedsegments);
	mySelLayer->SplitGeometry (&selectedsegments, m_SelectedData->GetSelectedUnique(),
							   myInsertedIDs1);
	
	
	
	// add attributions for new segment of selected line
	wxCommandEvent attribevt1(tmEVT_AM_COPY_ATTRIBUTION, wxID_ANY);
	attribevt1.SetExtraLong(m_SelectedData->GetSelectedUnique());
	wxArrayLong * myTempArray = new wxArrayLong(myInsertedIDs1); 
	attribevt1.SetClientData(myTempArray);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(attribevt1);
	
	
	//TODO: temp code, remove me
	//for (unsigned int w = 0; w < myInsertedIDs1.GetCount();w++)
	//	wxLogDebug(_T("ID inserted : %d"), myInsertedIDs1.Item(w));
	// end of temp code
	
	
	// compute intersections for other lines
	OGRMultiLineString myRes1;
	OGRMultiLineString myRes2;
	wxArrayLong myInsertedIDs2;
	wxCommandEvent attribevt2(tmEVT_AM_COPY_ATTRIBUTION, wxID_ANY);
	// for all crossing line, compute intersections
	for (unsigned int i = 0 ; i< myLinesCrossing->GetCount(); i++)
	{
		OGRLineString * myCrossedL = (OGRLineString*) LinesCrossing.getGeometryRef(i);
		if (myCrossedL)
		{
			mySelLayer->CutLineGeometry(myOGRSelLine, myCrossedL, 
										myRes1,	myRes2);
			mySelLayer->SplitGeometry(&myRes2, myLinesCrossing->Item(i),
									  myInsertedIDs2);
			myRes1.empty();
			myRes2.empty();
			
			// send attribution message
			attribevt2.SetExtraLong(myLinesCrossing->Item(i));
			wxArrayLong * myTempArray2 = new wxArrayLong(myInsertedIDs2); 
			attribevt2.SetClientData(myTempArray2);
			m_ParentEvt->GetEventHandler()->AddPendingEvent(attribevt2);
			myInsertedIDs2.Clear();
		}
	}
	OGRGeometryFactory::destroyGeometry(myOGRSelLine);
	delete myLinesCrossing;
	
	// add segment to selection
	m_SelectedData->AddSelected(&myInsertedIDs1);
	wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->AddPendingEvent(evt);
	
	
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	
	return true;	
}



/***************************************************************************//**
 @brief Display a dialog for editing vertex position
 @details Checks are done internally for ensuring that :
 - An object is selected
 - Editing layer is selected
 @return  true if Editing vertex is allowed, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
bool tmEditManager::EditVertexPosition ()
{
	// validation
	if (IsModifictionAllowed()==false)
		return false;
	
	// get the editing layers
	tmGISDataVector * mySelLayer = (tmGISDataVector*) tmGISData::LoadLayer(m_TOC->GetEditLayer());
	if (!mySelLayer)
		return false;
	
	// getting geometry info
	long lSelectedOID = m_SelectedData->GetSelectedUnique();
	OGRGeometry * myGeom = mySelLayer->GetGeometryByOID(lSelectedOID);
	if (!myGeom)
		return false;
	OGRwkbGeometryType myType =  wkbFlatten ( myGeom->getGeometryType());
	
	// preparing dialog and dialog data
	m_OldVertexPos = wxDefaultPosition;
	EditVertexDLG myDlg (m_Renderer);
	myDlg.m_SelectedOID = lSelectedOID;
	myDlg.m_LayerType = m_TOC->GetEditLayer()->m_LayerSpatialType;
	OGRLineString * myLine = NULL;
	OGRPoint * myPt = NULL;
	
	switch (myType)
	{
		case wkbPoint:
			myPt = (OGRPoint*) myGeom;
			myDlg.m_VertexPts.Add(wxRealPoint(myPt->getX(), myPt->getY()));
			break;
			
		case wkbLineString:
			myLine = (OGRLineString*) myGeom;
			for (int i = 0; i< myLine->getNumPoints();i++)
				myDlg.m_VertexPts.Add(wxRealPoint(myLine->getX(i), myLine->getY(i)));
			break;
		
		default:
			OGRGeometryFactory::destroyGeometry(myGeom);
			return false;
			break;
	}
	
	// displaying dialog
	if(myDlg.ShowModal() == wxID_OK)
	{
		// apply modification
		if (myPt)
		{
			wxASSERT (myDlg.m_VertexPts.GetCount() == 1);
			myPt->setX(myDlg.m_VertexPts.Item(0).x);
			myPt->setY(myDlg.m_VertexPts.Item(0).y);
		}
		else if (myLine)
		{
			myLine->empty();
			for (unsigned int j = 0; j < myDlg.m_VertexPts.GetCount(); j++)
			{
				myLine->addPoint(myDlg.m_VertexPts.Item(j).x,
								 myDlg.m_VertexPts.Item(j).y);	
			}
		}
		mySelLayer->UpdateGeometry(myGeom, myDlg.m_SelectedOID);
	}
	OGRGeometryFactory::destroyGeometry(myGeom);
	
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	
	return true;
}



/***************************************************************************//**
 @brief Merge selected lines
 @details  This function Checks the following rules:
 - Minimum two objects of type lines should been selected.
 - Editing mode should be turned on.
 Merging is done without user input if all selected lines share the same attribution.
 If a line is different, a dialog-box is displayed allowing the user to choice
 the attribution he want to keep for the new merged line.
 @return  true if merging was successfull or allowed, see above
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 February 2009
 *******************************************************************************/
bool tmEditManager::MergeSelectedLines ()
{
	//verifications
	if (!IsLayerTypeSelected(LAYER_SPATIAL_LINE) || !IsObjectMinNumberSelected(2))
		return false;
	
	
	// getting selections ids
	wxArrayLong * mySelectedIDs = m_SelectedData->GetSelectedValues();
	wxASSERT (mySelectedIDs);
	
	// merge lines
	tmGISDataVector * myLayer = (tmGISDataVector*) tmGISData::LoadLayer(m_TOC->GetEditLayer());
	wxASSERT (myLayer);
	OGRGeometry * myMergeGeom = NULL;
	OGRMultiLineString * mySelGeom = 
				(OGRMultiLineString*) myLayer->GetGeometryColByOID (mySelectedIDs);
	bool bMergeSuccess = myLayer->LinesMerge(mySelGeom, &myMergeGeom);

	
	OGRGeometryFactory::destroyGeometry(mySelGeom);
	if (bMergeSuccess == false)
	{
		wxMessageBox(_("Selected lines are not adjacent"),
					 _("Merge failed"), wxOK | wxICON_ERROR);
		return false;
	}
	
	
	// get attributions
	tmAttributionBasicArray myAttributions;
	tmAttributionDataLine myAttrib (m_SelectedData->GetSelectedValues(), m_pDB);
	if (myAttrib.GetInfoBasicArray(myAttributions) == false)
	{
		wxLogDebug(_T("Unable to get attributions from selected lines"));
		return false;
	}
	
	//FIXME: remove, this is temp logging code
	for (unsigned int k = 0; k<myAttributions.GetCount();k++)
	{
		wxString sTmp = wxString::Format(_T("Values for OID : %d are : "), 
										 myAttributions.Item(k).m_Oid);
		for (unsigned int m = 0; m< myAttributions.Item(k).m_Values.GetCount() ;m++)
		{
			sTmp.Append(wxString::Format(_T("%d, "), myAttributions.Item(k).m_Values.Item(m)));
		}
		
		wxLogDebug(sTmp);
	}
	//END 
	
	
	// compare attributions
	int iLineToKeep = 0;
	wxArrayString myConcatenedAttrib;
	if (tmAttributionData::IsAttributionSimilar(myAttributions) == false)
	{
		// get attribution name 
		if (!myAttrib.GetConcatenedBasicName(myAttributions, myConcatenedAttrib))
		{
			wxLogError(_T("Error getting concatenated basic values"));
			return false;
		}
		
		// display dialog
		wxSingleChoiceDialog myDlg (m_ParentEvt,
									_("Select the attribution to be kept"),
									_("Merge lines"),
									myConcatenedAttrib);
		if (myDlg.ShowModal()==wxID_CANCEL)
			return false;
		
		iLineToKeep = myDlg.GetSelection();
	}
	
	// remove lines
	wxLogDebug(_T("We keep : OID %d"), myAttributions.Item(iLineToKeep).m_Oid);
	mySelectedIDs->RemoveAt(iLineToKeep);
	m_pDB->DeleteGeometry(mySelectedIDs, m_TOC->GetEditLayer()->m_LayerType);
	m_pDB->DeleteAttribution(mySelectedIDs, m_TOC->GetEditLayer()->m_LayerType);

	// update geometry
	wxASSERT (myMergeGeom);
	myLayer->UpdateGeometry(myMergeGeom, myAttributions.Item(iLineToKeep).m_Oid);
	OGRGeometryFactory::destroyGeometry(myMergeGeom);
		
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
		
	return true;
}




