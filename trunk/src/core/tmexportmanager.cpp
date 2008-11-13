/***************************************************************************
								tmexportmanager.cpp
                    Main class for dealing with export process
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

#include "tmexportmanager.h"



/***************************************************************************//**
 @brief Init default member values
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
void tmExportManager::InitMemberValues()
{
	m_pDB = NULL;
	m_Parent = NULL;
}



/***************************************************************************//**
 @brief Default constructor
 @details Call  tmExportManager::Create() function if you use this constructor
 in order to initialization to take place.
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
tmExportManager::tmExportManager()
{
	InitMemberValues();
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
tmExportManager::~tmExportManager()
{

}



/***************************************************************************//**
 @brief Create function for two step initialization
 @details If default constructor was used, need to call this function for
 initialization to take place
 @param database a valid #DataBaseTM object adress
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
void tmExportManager::Create(wxWindow * parent, DataBaseTM * database)
{
	wxASSERT (database);
	wxASSERT (parent);
	m_pDB = database;
	m_Parent = parent;
}


/***************************************************************************//**
 @brief Full constructor
 @param database a valid #DataBaseTM object adress
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
tmExportManager::tmExportManager(wxWindow * parent, DataBaseTM * database)
{
	InitMemberValues();
	Create(parent, database);
}



/***************************************************************************//**
 @brief Ask user which layer to export
 @return  true if the export doesn't encounter a fatal error (warnings may be
 treated in a report), false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
bool tmExportManager::ExportSelected ()
{
	wxASSERT(m_pDB);
	wxASSERT(m_Parent);
	
	// get all layers from DB
	PrjMemLayersArray * myLayers = GetAllLayers();
	if (!myLayers)
	{
		return false;
	}
	
	// choose layer(s) to export
	wxArrayString myNames;
	for (unsigned int i = 0; i<myLayers->GetCount();i++)
		myNames.Add(myLayers->Item(i).m_LayerName);
	wxString sMsg = _("Select layer(s) to export");
	
	wxMultiChoiceDialog myDlg (m_Parent,sMsg, _T("Export layer"),myNames);
	if (myDlg.ShowModal() != wxID_OK)
	// select layers dialog canceled
	{
		myLayers->Clear();
		delete myLayers;
		return false;
	}
	
	wxArrayInt mySelectedLayersIndex = myDlg.GetSelections();
	if (mySelectedLayersIndex.IsEmpty())
		return false; // no layer selected
	
	// keep only selected layers in array
	bool bRemove = true;
	for(int j = (signed) myLayers->GetCount()-1; j >= 0; j--)
	{
		if (!mySelectedLayersIndex.IsEmpty())
		{
			if (j == mySelectedLayersIndex.Last())
			{
				bRemove = false;
				mySelectedLayersIndex.RemoveAt(mySelectedLayersIndex.GetCount()-1);
			}
		}
		
		if (bRemove)
			myLayers->RemoveAt(j);
		
		bRemove = true;
	}
	
	/* logging
	for (unsigned int e = 0; e < myLayers->GetCount();e++)
		wxLogDebug(_T("Layers : %d - %s"), e, myLayers->Item(e).m_LayerName.c_str());*/
	
	
	myLayers->Clear();
	delete myLayers;

	return true;
}


/***************************************************************************//**
 @brief Get all layers from database
 @details This function return all layers in an Array of
 #ProjectDefMemoryLayers. nblayers indicating the numbers of layers returned.
 @param nblayers the number of layers returned in the array
 @return  An array of #ProjectDefMemoryLayers or NULL in case of error
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
PrjMemLayersArray * tmExportManager::GetAllLayers ()
{
	wxASSERT(m_pDB);
	
	
	ProjectDefMemoryLayers myLayer;
	
	// first loop, destroy remaining results
	m_pDB->DataBaseDestroyResults();
	if(m_pDB->GetNextLayer(&myLayer) != 0)
	{
		wxASSERT_MSG(0, _T("Error getting layers"));
		return NULL;
	}
	int nblayers = m_pDB->DatabaseGetCountResults();
	if (nblayers == 0)
	{
		wxASSERT_MSG (0, _T("No layers returned : Error, unable to export"));
		return NULL;
	}
	
	PrjMemLayersArray * myLayers = new PrjMemLayersArray;
	
	for (int i = 0 ; i<nblayers;i++)
	{
		if (m_pDB->GetNextLayer(&myLayer)==1)
			myLayers->Add(myLayer);
	}
	
	return myLayers;
}


