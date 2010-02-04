/***************************************************************************
								tmtoolmanager.cpp
                    Take control of all tools (Dangling nodes, etc.)
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



#include "tmtoolmanager.h"


BEGIN_EVENT_TABLE(tmToolManager, wxEvtHandler)
	EVT_COMMAND (wxID_ANY, tmEVT_TM_UPDATE_TOOL_VIEW, tmToolManager::OnViewUpdated)
END_EVENT_TABLE()




void tmToolManager::InitMemberValues()
{
	m_Parent = NULL;
	m_TOC = NULL;
	m_Selected = NULL;
	m_Renderer = NULL;
	m_Scale = NULL;
	m_pDB = NULL;
}



tmToolManager::tmToolManager(wxWindow * parent,
			  tmTOCCtrl * toc,
			  tmSelectedDataMemory * seldata,
			  tmRenderer * renderer,
			  tmGISScale * scale)
{
	InitMemberValues();
	m_Parent = parent;
	m_TOC = toc;
	m_Selected = seldata;
	m_Renderer = renderer;
	m_Scale = scale;
	
	
	m_Parent->PushEventHandler(this);
	
}



tmToolManager::~tmToolManager()
{
	m_Parent->PopEventHandler(false);
	m_Parent->SetEventHandler(m_Parent);

}



bool tmToolManager::TMGetLayers(wxArrayString & layersname)
{
	if (TMIsOk()==false)
		return false;
	
	layersname = m_pDB->GetLayerNameByType(LAYER_SPATIAL_POLYGON);
	if (layersname.GetCount() == 0)
	{
		wxLogDebug(_T("No polygons layer defined"));
		return false;
	}
	return true;
}



bool tmToolManager::TMSearchDanglingNodes(int selectedlayer,
										  const wxArrayString & layersname)
{
	wxASSERT(selectedlayer != wxNOT_FOUND);
	wxASSERT(m_pDB);
	TMClearDangling();
	
	// get real layer ids
	wxArrayLong myLayersID;
	if(TMGetLayersIDs(myLayersID,  layersname)==false)
		return false;
	if (selectedlayer != 0)
	{
		long myLayer = myLayersID.Item(selectedlayer -1);
		myLayersID.Clear();
		myLayersID.Add(myLayer);
	}
	

	
	unsigned int iLayersCount = myLayersID.GetCount();
	wxLogDebug(_T("%d layers to check"), iLayersCount);
	
	wxString smsg = wxString::Format(_("Searching dangling nodes on %d layer(s)"),iLayersCount);
	wxProgressDialog myDlg(_("Searching dangling nodes"), smsg, iLayersCount * 100, m_Parent,wxPD_AUTO_HIDE| wxPD_CAN_ABORT); 
	
	// search dangling nodes
	ToolDanglingNodes myTool (m_pDB);
	bool bStoped = false;
	for (unsigned int i = 0; i< iLayersCount; i++)
	{
		myTool.SearchInit(myLayersID.Item(i));
		if(myTool.SearchRun(&myDlg)==false)
		{
			bStoped = true;
			break;
		}
		wxLogDebug(_T("Dangling nodes searched for layers : %d"), i);
		
		myTool.GetDanglingNodes(m_DanglingPts);
	}
	
	if (bStoped==true)
	{
		TMClearDangling();
		return false;
	}
	
	wxString myMsg = wxString::Format(_T("%d dangling nodes found"), m_DanglingPts.GetCount());
	wxLogDebug(myMsg);
	wxMessageBox(myMsg, _("Searching dangling nodes done"), wxICON_INFORMATION | wxOK, m_Parent);
	

	return true;	
}


bool tmToolManager::TMGetLayersIDs (wxArrayLong & layersid, const wxArrayString & layersname)
{
	wxASSERT(m_pDB);
	wxArrayString mySearchedLayers(layersname);
	mySearchedLayers.RemoveAt(0);
	
	ProjectDefMemoryLayers myActualLayer;
	int iFirst = m_pDB->GetNextLayer(&myActualLayer);
	wxASSERT(iFirst == 0);
	while (m_pDB->GetNextLayer(&myActualLayer) != -1)
	{
		for(unsigned int i = 0; i<mySearchedLayers.GetCount();i++)
			if (mySearchedLayers.Item(i) == myActualLayer.m_LayerName)
			{
				layersid.Add(myActualLayer.m_LayerID);
				break;
			}
	}
	
	wxASSERT(layersid.GetCount() == mySearchedLayers.GetCount());
	if (layersid.GetCount() > 0)
		return true;
	
	wxLogDebug(_T("No ID found for layers"));
	return false;
}


bool tmToolManager::FindDanglingNodes()
{
	// check DB pointer
	if (TMIsOk()==false)
		return false;

	// get polygons layers
	wxArrayString myLayers;
	if(TMGetLayers(myLayers)==false)
		return false;
	
	myLayers.Insert(_("All polygon layers"), 0);
	
	
	DanglingNode_DLG myDlg(m_Parent);
	myDlg.InitDialog(myLayers, wxNOT_FOUND);
	int iReturn = myDlg.ShowModal();
	
	wxCommandEvent myUnusedEvent;
	
	bool bReturn = true;
	switch (iReturn)
	{
		case wxID_OK:
			// compute dangling nodes vertex
			bReturn = TMSearchDanglingNodes(myDlg.GetSelectedLayer(),
								  myLayers);
			
			OnViewUpdated(myUnusedEvent);
			
			break;
			
		case ID_DLGDN_CLEAR:
			// clear dangling nodes message
			TMClearDangling();
			OnViewUpdated(myUnusedEvent);
			break;
			
		default:
			bReturn = false;
			break;
	}
	
	return bReturn;
}



bool tmToolManager::TMIsOk()
{
	if (m_pDB == NULL)
	{
		wxLogDebug(_T("Database not inited, init database first"));
		return false;
	}
	
	return true;
	
}



void tmToolManager::OnViewUpdated (wxCommandEvent & event)
{
	if(TMDrawDanglingNodes(true)==true)
	{
		wxLogDebug(_T("Dangling nodes drawed"));
	}
	
}


bool tmToolManager::TMDrawDanglingNodes(bool refresh)
{	
	if(refresh==true){
		m_Renderer->Refresh();
		m_Renderer->Update();
	}
	
	if (m_DanglingPts.GetCount() == 0)
		return false;
	
	wxASSERT(m_Renderer != NULL);
	wxClientDC dc (m_Renderer);
	wxPen myPen (*wxRED_PEN);
	dc.SetPen(myPen);
			 
	
	for (unsigned int i = 0; i< m_DanglingPts.GetCount() ;i ++)
	{
		wxPoint myPt = m_Scale->RealToPixel(m_DanglingPts.Item(i));
		dc.DrawCircle(myPt, tmDANGLING_NODE_DRAW_SIZE);
	}
		

	return true;
}



