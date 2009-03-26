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
	m_ExportData = NULL;
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
	if (m_ExportData)
		delete m_ExportData;
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
 @brief Export all layers without futher informations from user
 @return  return true if all layers exported successfully, false otherwise.
 Warnings may be treated in a report
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 November 2008
 *******************************************************************************/
bool tmExportManager::ExportAll ()
{
	wxASSERT(m_pDB);
	wxASSERT(m_Parent);
	
	// get all layers from DB
	PrjMemLayersArray * myLayers = GetAllLayers();
	if (!myLayers)
	{
		return false;
	}
	
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
	
	CreateProgress(layers->GetCount(),
				   layers->Item(0).m_LayerName);
	
	
	
	// get frame
	int iFrameVertex = 0;
	wxRealPoint * pFrame = GetFrame(iFrameVertex);
	if (!pFrame)
		return false;
	

	// for each layer
	for (unsigned int i = 0; i<layers->GetCount();i++)
	{
		m_ExportData = CreateExportData();
		m_ExportData->SetFrame(pFrame, iFrameVertex);
		
		// create SIG layer
		if (CreateExportLayer(&(layers->Item(i))))
		{
			// export data to layer
			if (ExportGISData(&(layers->Item(i))))
				if (AddAttributionSimpleData(&(layers->Item(i))))
				wxLogDebug(_T("Exporting layers : %s - OK-"),
						   layers->Item(i).m_LayerName.c_str());
		}
		
		delete m_ExportData;
		m_ExportData = NULL;
		
		// update progress dialog
		if(UpdateProgress(i, layers->Item(i).m_LayerName))
		{
			wxLogMessage(_("Export cancelled by user."));
			break;
		}
			
		/*ProgDlg.Update(i,
					   mydlgtext + layers->Item(i).m_LayerName,
					   &bSkip);
		if (bSkip)
		{
			wxLogMessage(_("Export cancelled by user."));
			break;
		}*/
	}
	
	DeleteProgress();
	delete [] pFrame;
	return true;
}


/***************************************************************************//**
 @brief Create the progress dialog
 @details This function uses the wxProgressDialog class when more than one
 layers is to be exported and a wxBusyinfo when only one layers is
 exported.
 @param iNbLayers The number of layers to export
 @param layername The first layername
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 January 2009
 *******************************************************************************/
void tmExportManager::CreateProgress(int iNbLayers, const wxString & layername)
{
	m_ProgressDlg = NULL;
	m_ProgressBusy = NULL;
	m_ProgressText = _("Exporting  Layers    :    ");
	
	if (iNbLayers > 1)
	{
		m_ProgressDlg = new wxProgressDialog(_("Exporting Project in progress"),
											 m_ProgressText + layername,
											 iNbLayers,
											 m_Parent,
											 wxPD_CAN_ABORT |  wxPD_AUTO_HIDE | wxPD_APP_MODAL);
		
	}
	else
	{
		m_ProgressBusy = new wxBusyInfo(_("Please wait, exporting : ") + layername,
										m_Parent);
	}
	
}

/***************************************************************************//**
 @brief Update the progress dialog
 @details This function does nothing if only one layers is exported and
 otherwise it update the progress dialog
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 January 2009
 *******************************************************************************/
bool tmExportManager::UpdateProgress(int iActualLayer, const wxString & layername)
{
	bool bSkip = false; 
	if (m_ProgressDlg)
	{
		m_ProgressDlg->Update(iActualLayer, m_ProgressText + layername, &bSkip);
	}
	
	return bSkip;
}


/***************************************************************************//**
 @brief Delete the progress indicator
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 January 2009
 *******************************************************************************/
void tmExportManager::DeleteProgress()
{
	if (m_ProgressDlg)
		delete m_ProgressDlg;
	
	if (m_ProgressBusy)
		delete m_ProgressBusy;
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
	wxASSERT (m_ExportData);
	bool bReturn = true;
	
	
	
	// get size of object_description
	int iSizeOfObjCol = m_ExportData->GetSizeOfObjDesc(layer->m_LayerID);
	
	
	// create SIG layer
	if (m_ExportData->
		CreateEmptyExportFile(layer,
							  m_ExportPath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR)))
	{
				
		// add obligatory fields
		if (!m_ExportData->AddGenericFields(iSizeOfObjCol))
			bReturn = false;
		else
		{
		
			// add optionnal fields
			PrjMemFieldArray * myFields = GetAllFieldsForLayer(layer);
			if (myFields) // ok we have advanced fields
			{
				
				m_ExportData->AddOptFields(myFields);
				delete myFields;
			}
		
		}
		
	}
	else
		bReturn = false;
			
	
	
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




/***************************************************************************//**
 @brief Export GIS data to the created layer
 @param layer a valid object of type #ProjectDefMemoryLayers
 @return  true if GIS data added successfully to the layer
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
bool tmExportManager::ExportGISData (ProjectDefMemoryLayers * layer)
{
	wxASSERT (m_ExportData);
	
	// get gis data
	wxString sGeomTable = wxEmptyString;
	wxString sValTable = wxEmptyString;
	switch (layer->m_LayerType)
	{
		case LAYER_POLYGON:
		case LAYER_LINE:
			sGeomTable = TABLE_NAME_GIS_GENERIC[0]; // lines
			sValTable = TABLE_NAME_GIS_ATTRIBUTION[0];
			break;
			
		case LAYER_POINT:
			sGeomTable = TABLE_NAME_GIS_GENERIC[1];
			sValTable = TABLE_NAME_GIS_ATTRIBUTION[1];
			break;

		default:
			break;
	}
	
	//TODO: Order by object ID ??
	wxString sTemp = _T("SELECT %s.OBJECT_ID, %s.OBJECT_GEOMETRY, %s.OBJECT_CD,")
	_T(" %s.OBJECT_DESC FROM %s ")
	_T(" LEFT JOIN %s ON (%s.OBJECT_ID = %s.OBJECT_GEOM_ID) ") 
	_T(" LEFT JOIN %s ON %s.OBJECT_VAL_ID = %s.OBJECT_ID WHERE")
	_T(" %s.THEMATIC_LAYERS_LAYER_INDEX = %d");
	wxString sSentence = wxString::Format(sTemp,
										  sGeomTable.c_str(),
										  sGeomTable.c_str(),
										  TABLE_NAME_OBJECTS.c_str(),
										  TABLE_NAME_OBJECTS.c_str(),
										  sGeomTable.c_str(),
										  sValTable.c_str(),
										  sGeomTable.c_str(),
										  sValTable.c_str(),
										  TABLE_NAME_OBJECTS.c_str(),
										  sValTable.c_str(),
										  TABLE_NAME_OBJECTS.c_str(),
										  TABLE_NAME_OBJECTS.c_str(),
										  layer->m_LayerID);
	if (!m_pDB->DataBaseQuery(sSentence))
	{
		wxLogDebug(_T("Error getting data : %s"),
				   m_pDB->DataBaseGetLastError().c_str());
		return false;
	}
	
	
	switch (layer->m_LayerType)
	{
		case LAYER_LINE:
			if (m_ExportData->WriteLines(layer))
				return true;
			break;
			
		case LAYER_POINT:
			if (m_ExportData->WritePoints(layer))
				return true;
			break;
			
		case LAYER_POLYGON:
			if (m_ExportData->WritePolygons(layer))
				return true;
			break;
			
		default:
			break;
	}
	
	m_pDB->DataBaseDestroyResults();
	
	return false;
}



/***************************************************************************//**
 @brief Get the frame
 @details Array of wxRealPoints returned must be destroyed by caller
 @param nbvertex the nb of vertex contained in the wxRealPoint array returned
 @return  An array of nbvertex item containing all vertex creating the frame
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
wxRealPoint *  tmExportManager::GetFrame (int & nbvertex)
{
	wxASSERT(m_pDB);
	wxString sSentence = _T("SELECT * FROM ") + TABLE_NAME_GIS_GENERIC[4];
	
	if (!m_pDB->DataBaseQuery(sSentence))
	{
		nbvertex = 0;
		return NULL;
	}
	
	tmGISDataVectorMYSQL myFrameDB;
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
	long loid = 0;
	return myFrameDB.GetNextDataLine(nbvertex, loid);
	
}



/***************************************************************************//**
 @brief Fill columns OBJ_CD, OBJ_DESC
 @param layer informations about the current layer
 @return  true if value were passed successfully, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 November 2008
 *******************************************************************************/
bool tmExportManager::AddAttributionSimpleData (ProjectDefMemoryLayers * layer)
{
	switch (layer->m_LayerType)
	{
		case LAYER_LINE:
			if (m_ExportData->AddSimpleDataToLine(layer))
				return true;
			break;
			
		case LAYER_POINT:
			if (m_ExportData->AddSimpleDataToPoint(layer))
				return true;
			break;
			
		case LAYER_POLYGON:
			if (m_ExportData->AddSimpleDataToPolygon(layer))
				return true;
			break;
			
		default:
			break;
	}
	
	return false;
}



/***************************************************************************//**
 @brief Fill Advanced columns
 @param layer information object about the current layer
 @return  true if values where passed successfully
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 March 2009
 *******************************************************************************/
bool tmExportManager::AddAttributionAdvanced (ProjectDefMemoryLayers * layer)
{
	switch (layer->m_LayerType)
	{
		case LAYER_LINE:
			if (m_ExportData->AddAdvancedDataToLine(layer))
				return true;
			break;
			
		/*case LAYER_POINT:
			if (m_ExportData->AddSimpleDataToPoint(layer))
				return true;
			break;
			
		case LAYER_POLYGON:
			if (m_ExportData->AddSimpleDataToPolygon(layer))
				return true;
			break;*/
			
		default:
			wxLogDebug(_T("Layer type not supported now : %d"), layer->m_LayerType);
			break;
	}
	
	return false;
	
}
