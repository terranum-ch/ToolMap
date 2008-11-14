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
	m_ExportType = EXPORT_SHAPEFILE;
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
 @param parent Adress of parent, maily used for parenting dialogs
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
 @param parent a valid wxWindow. Mainly used for parent of dialogs
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
	bool bReturn = ExportLayers(myLayers);
	
	myLayers->Clear();
	delete myLayers;

	return bReturn;
}


/***************************************************************************//**
 @brief Get all layers from database
 @details This function return all layers in an Array of
 #ProjectDefMemoryLayers.
 @return  An array of PrjMeLayersArray or NULL in case of error
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



/***************************************************************************//**
 @brief Export layers
 @details This function is called either by ExportSelected() or ExportAll()
 @param layers An array of PrjMemLayersArray
 @return  true if export was successfull, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
bool tmExportManager::ExportLayers (PrjMemLayersArray * layers)
{
	wxASSERT(layers);
	if (layers->GetCount() == 0)
		return false;
	
	// check and init path and export type
	wxString szErr = _("Error, export directory isn't specified or isn't valid.");
	if (!IsExportPathValid())
	{
		
		wxMessageBox(szErr, _("Error exporting project"),
					 wxOK | wxICON_ERROR ,m_Parent);
		return false;
	}
	
	

	// for each layer
	for (unsigned int i = 0; i<layers->GetCount();i++)
	{
		// create SIG layer
		if (CreateExportLayer(&(layers->Item(i))))
			// export data to layer
			wxLogDebug(_T("Exporting layers : %s - OK-"),
					   layers->Item(i).m_LayerName.c_str());
			
	}
	
	return true;
}


/***************************************************************************//**
 @brief First export step
 @details During this step, we try to create the file and attach needed files
 @param layer a valid #ProjectDefMemoryLayers object
 @return  true if layers was created successfully, false otherwise and info is
 added to the rapport
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
bool tmExportManager::CreateExportLayer (ProjectDefMemoryLayers * layer)
{
	wxASSERT(layer);
	bool bReturn = true;
	
	// create SIG layer
	tmExportData * myExportData = CreateExportData();
	if (myExportData->
		CreateEmptyExportFile(layer,
							  m_ExportPath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR)))
	{
				
		// add optionnal fields
		PrjMemFieldArray * myFields = GetAllFieldsForLayer(layer);
		if (myFields) // ok we have advanced fields
		{
			
			
			delete myFields;
		}
		
		
		
	}
	else
		bReturn = false;
			
	
	delete myExportData;
	return bReturn;
}



/***************************************************************************//**
 @brief Get all fields
 @param layer a valid #ProjectDefMemoryLayers for the layer we want to get fields
 @return  an adress containing Array of fields or null if an error occur. Returned
 object must be destroyed by caller.
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
PrjMemFieldArray * tmExportManager::GetAllFieldsForLayer(ProjectDefMemoryLayers * layer)
{
	wxASSERT(layer);
	m_pDB->DataBaseDestroyResults();
	
	PrjMemFieldArray * myFieldArray = new PrjMemFieldArray();
	if(m_pDB->GetFields(*myFieldArray, layer))
		return myFieldArray;
	
	return NULL;
}



/***************************************************************************//**
 @brief Get and check the export path
 @details We verify that an export path was defined and is valid
 @return  true if the export path is defined and valid, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
bool tmExportManager::IsExportPathValid()
{
	wxASSERT(m_pDB);
	wxString myExportPath = wxEmptyString;
	int myExportType = -1;
	if (m_pDB->GetProjectExportData(myExportType, myExportPath) != PATH_OK)
		return false;
	
	m_ExportType = static_cast<PRJDEF_EXPORT_TYPE> (myExportType);
	m_ExportPath = wxFileName (myExportPath, _T(""));
	
	if (m_ExportPath.IsDirWritable())
		return true;
	
	
	return false;
}



/***************************************************************************//**
 @brief Create object
 @details depanding of export type, an object of type #tmExportData is created
 and allow us to support more export format in the future without too much work
 @return  A valid object of type #tmExportData
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportData * tmExportManager::CreateExportData ()
{
	wxASSERT (m_pDB);
	switch (m_ExportType)
	{
		case EXPORT_SHAPEFILE:
			return new tmExportDataSHP(m_pDB);
			break;
			
			
		default:
			return new tmExportData(m_pDB);
			break;
	} 
	
	return NULL;
}

