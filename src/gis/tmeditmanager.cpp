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
END_EVENT_TABLE()



/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
tmEditManager::tmEditManager(wxWindow * parent,tmTOCCtrl * toc,
							 tmSelectedDataMemory * seldata,
							 tmRenderer * renderer)
{
	InitMemberValues();
	m_ParentEvt = parent;
	m_TOC = toc;
	m_SelectedData = seldata;
	m_Renderer = renderer;

	m_ParentEvt->PushEventHandler(this);

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
	m_Renderer->SetTool(tmTOOL_EDIT);
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
}


