/***************************************************************************
							tmattributionmanager.cpp
							Manage the attribution
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

#include "tmattributionmanager.h"


BEGIN_EVENT_TABLE(tmAttributionManager, wxEvtHandler)
	EVT_COMMAND (wxID_ANY, tmEVT_SELECTION_DONE, tmAttributionManager::OnSelection)
	EVT_COMMAND (wxID_ANY, tmEVT_ATTRIBUTION_BTN_PRESSED, tmAttributionManager::OnAttributeBtn)
END_EVENT_TABLE()


/***************************************************************************//**
 @brief Constructor
 @param parent Adress of the parent, used for event processing
 @param toc Adress of a #tmTOCCtrl
 @param panel Adress of a #AttribObjType_PANEL object
 @param selection Adress of a #tmSelectedDataMemory object
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
tmAttributionManager::tmAttributionManager(wxWindow * parent,
										   tmTOCCtrl * toc, 
										   AttribObjType_PANEL * panel,
										   tmSelectedDataMemory * selection)
{
	m_Parent = parent;
	m_TOC = toc;
	m_Panel = panel;
	m_SelData = selection;
	
	m_pDB = NULL;
	m_pLayerProperties = NULL;
	m_Parent->PushEventHandler(this);
}



/***************************************************************************//**
 @brief Destructor
 @details Doesn't do anything actually
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
tmAttributionManager::~tmAttributionManager()
{
	UnInitAttributionManager();
	m_Parent->PopEventHandler(false);
	m_Parent->SetEventHandler(m_Parent);
}



/***************************************************************************//**
 @brief Init attribution manager
 @details This function should be the first called for beeing able to use the
 attibution manager
 @return  true if Database, TOC, selection data, and panel are valid
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
bool tmAttributionManager::InitAttributionManager(DataBaseTM * pDb)
{
	m_pDB = pDb;
	m_pLayerProperties = NULL;
	
	// check validity for all objects
	return IsAttributionManagerReady();
}



/***************************************************************************//**
 @brief Uninit attribution manager
 @details Mark the attribution manager as not valid.
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
void tmAttributionManager::UnInitAttributionManager()
{
	m_pDB = NULL;
	m_pLayerProperties = NULL;
}



/***************************************************************************//**
 @brief Checks all #tmAttributionManager object for validity
 @return  true if all objects are valid, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
bool tmAttributionManager::IsAttributionManagerReady()
{
	if (!m_Parent)
		return false;
	
	if (!m_Panel)
		return false;
	
	if (!m_SelData)
		return false;
	
	if(!m_TOC)
		return false;
	
	if (!m_pDB)
		return false;
	
	return true;
}



/***************************************************************************//**
 @brief Called when selection was made
 @details This is called by the #tmRenderer when a selection (either by point or
 rectangle) was made. This event is also used by #tmLayerManager::OnSelection
 @param event Contain data about the status of the shift key and the rectangle
 size. 
 - Use event.GetInt() for getting the status of the shift key
 - Use event.GetClientObject() for getting the wxRect.
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
void tmAttributionManager::OnSelection (wxCommandEvent & event)
{
	int iSelFeatureCount = m_SelData->GetCount();
	
	// some verifications :
	//	A layer must be selected
	//	A construction layer must be selected (< TOC_NAME_FRAME)
	m_pLayerProperties = m_TOC->GetSelectionLayer();
	if (!m_pLayerProperties)
		return;
	
	if(m_pLayerProperties->m_LayerType >= TOC_NAME_FRAME)
		return;
	
	m_Panel->SetAttributeBtn(iSelFeatureCount);
	TOC_GENERIC_NAME mySelType = static_cast<TOC_GENERIC_NAME> (m_pLayerProperties->m_LayerType);
	m_Panel->SetVisibleNotebook(mySelType);
	
	
}



/***************************************************************************//**
 @brief Called when user press attribute
 @details This event function is called by the #AttribObjType_PANEL when user
 press the attribute Button.
 @param event 
 @author Lucien Schreiber (c) CREALP 2008
 @date 05 November 2008
 *******************************************************************************/
void tmAttributionManager::OnAttributeBtn (wxCommandEvent & event)
{
	// verification : 
	//		Selected notebook page is same type as
	//		selected layer in TOC
	wxASSERT(m_pLayerProperties);
	if (m_pLayerProperties->m_LayerType != m_Panel->GetVisibleNotebook())
	{
		wxLogDebug(_("Something incorrect, selected panel doesn't correspond to the selected layer"));
		return;
	}
	
	wxArrayLong  * mySelObjArray = m_SelData->GetSelectedValues();
	int myLayerIndex = m_SelData->GetSelectedLayer();
	
	// create attribution object based on type
	tmAttributionData * myAttrib = NULL;
	switch (m_pLayerProperties->m_LayerType)
	{
		case TOC_NAME_LINES:
			myAttrib = new tmAttributionDataLine();
			break;
			
		default:
			myAttrib = new tmAttributionData();
			break;
	}
	myAttrib->Create(mySelObjArray, m_pDB);
	if(!myAttrib->SetAttributeBasic(m_Panel))
	{
		wxLogMessage(_("Unable to attribute those data"));
	}
	
	delete myAttrib;
		
	
	
}


