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
}



tmToolManager::~tmToolManager()
{


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
	
	
	// get real layer ids
	wxArrayLong myLayersID;
	TMGetLayersIDs(myLayersID,  layersname);
	
	
	// search dangling nodes
	
	/*
	// search all polygons layers
	if (selectedlayer == 0)
	{
		//ToolDanglingNodes myDN(m_pDB);
		//if (myDN.SearchInit(<#long layerid#>)
	}*/
	
	return true;	
}


void tmToolManager::TMGetLayersIDs (wxArrayLong & layersid, const wxArrayString & layersname)
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
	
	bool bReturn = true;
	switch (iReturn)
	{
		case wxID_OK:
			// compute dangling nodes vertex
			bReturn = TMSearchDanglingNodes(myDlg.GetSelectedLayer(),
								  myLayers);
			
			break;
			
		case ID_DLGDN_CLEAR:
			// clear dangling nodes message
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
