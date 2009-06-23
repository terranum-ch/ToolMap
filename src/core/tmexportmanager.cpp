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
	m_ProjMem = NULL;
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
bool tmExportManager::ExportSelected (PrjDefMemManage * localprojdef)
{
	wxASSERT(m_pDB);
	wxASSERT(m_Parent);
	wxASSERT (localprojdef);
	m_ProjMem = localprojdef;
	
	// get all layers from memory
	PrjMemLayersArray * myLayers = m_ProjMem->m_PrjLayerArray;
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
	
	bool bReturn = ExportLayers(myLayers);
	return bReturn;
}



/***************************************************************************//**
 @brief Export all layers without futher informations from user
 @return  return true if all layers exported successfully, false otherwise.
 Warnings may be treated in a report
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 November 2008
 *******************************************************************************/
bool tmExportManager::ExportAll (PrjDefMemManage * localprojdef)
{
	wxASSERT(m_pDB);
	wxASSERT(m_Parent);
	wxASSERT (localprojdef);
	m_ProjMem = localprojdef;
	
	PrjMemLayersArray * myLayers = m_ProjMem->m_PrjLayerArray ;
	if (!myLayers)
	{
		return false;
	}
	return ExportLayers(myLayers);
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
	
	bool bExportResult = true;
	// for each layer
	for (unsigned int i = 0; i<layers->GetCount();i++)
	{
		// update progress dialog
		ProjectDefMemoryLayers myLayer = layers->Item(i);
		if(UpdateProgress(i, myLayer.m_LayerName))
		{
			wxLogMessage(_("Export cancelled by user."));
			break;
		}
		
		if (!ExportLayer(&myLayer, pFrame, iFrameVertex))
			bExportResult = false;
		
		/*
		// check for file name 
		
		
		
		m_ExportData = CreateExportData();
		m_ExportData->SetFrame(pFrame, iFrameVertex);
		
		// create SIG layer
		ProjectDefMemoryLayers myLayer = layers->Item(i);
		
		if (CreateExportLayer(&myLayer))
		{
			// export data to layer
			if (ExportGISData(&myLayer))
				if (AddAttributionSimpleData(&myLayer))
					if (AddAttributionAdvanced(&myLayer))
						bExportResult = true;
		}
		
		// logging
		if (bExportResult == true)
		{
			wxLogDebug(_T("Exporting layers : %s - OK-"),
					   layers->Item(i).m_LayerName.c_str());
		}
		else
		{
			wxLogDebug(_T("Exporting layers : %s - FAILED -"),
					   layers->Item(i).m_LayerName.c_str());
			break;
		}
			
		delete m_ExportData;
		m_ExportData = NULL;*/
		

			
	}
	
	DeleteProgress();
	delete [] pFrame;
	return bExportResult;
}



/***************************************************************************//**
 @brief Export a layer, don't call directly
 @details This function is called through the ExportLayers() function, don't
 call directly
 @param layer information about the actual layer to export
 @return  true if export of selected layer success
 @author Lucien Schreiber (c) CREALP 2009
 @date 08 April 2009
 *******************************************************************************/
bool tmExportManager::ExportLayer (ProjectDefMemoryLayers * layer,
								   wxRealPoint * frame, const int & framevertex)
{
	
	// check for file name 
	if (GetAvailableFileName(layer) == false)
		return false;
	
	
	// EXPORT
	bool bReturn = false;
	m_ExportData = CreateExportData();
	m_ExportData->SetFrame(frame, framevertex);
	
	// create SIG layer
	//ProjectDefMemoryLayers myLayer = layers->Item(i);
	
	if (CreateExportLayer(layer))
	{
		// export data to layer
		if (ExportGISData(layer))
			if (AddAttributionSimpleData(layer))
				if (AddAttributionAdvanced(layer))
					bReturn = true;
	}
	
	// logging
	if (bReturn == true)
	{
		wxLogDebug(_T("Exporting layers : %s ... OK"),
				   layer->m_LayerName.c_str());
	}
	else
	{
		wxLogDebug(_T("Exporting layers : %s ... FAILED / NO DATA"),
				   layer->m_LayerName.c_str());
	}
	
	delete m_ExportData;
	m_ExportData = NULL;
	return bReturn;	
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
		
		// add toolmap field
		if (!m_ExportData->AddFIDField())
			bReturn = false;
		
		// add obligatory fields
		if (!m_ExportData->AddGenericFields(iSizeOfObjCol))
			bReturn = false;
		else
		{
		
			// add optionnal fields
			PrjMemFieldArray * myFields = layer->m_pLayerFieldArray;
			if (myFields) // ok we have advanced fields
			{
				
				m_ExportData->AddOptFields(myFields);
			}
		
		}
		
	}
	else
		bReturn = false;
			
	
	
	return bReturn;
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
 @brief Get the export file name
 @details Checks that the file doesn't exists and if the file exists, we try to
 add an increment
 @param layer layer information about what we are trying to export
 @param bool true if system was able to find a file name, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 08 April 2009
 *******************************************************************************/
bool tmExportManager::GetAvailableFileName (ProjectDefMemoryLayers * layer)
{
	// get the extension
	tmExportData * myExportFile = CreateExportData();
	wxASSERT (myExportFile);
	wxString myExtension = myExportFile->GetFileExtension();
	delete myExportFile;
	
	// does the actual file name exists ?
	wxFileName myFileName = wxFileName(m_ExportPath.GetPathWithSep(),
									   layer->m_LayerName, myExtension);
	if (myFileName.FileExists() == false)
	{
		return true;
	}
	
	
	// search for available name
	wxString myFileSearchedTemplate = _T("%s-%.*d");
	for (int i = 1; i <= tmFILENAME_MAX_SEARCH;i++)
	{
		myFileName.SetName(wxString::Format(myFileSearchedTemplate, 
											layer->m_LayerName.c_str(),
											3, i));
		wxLogDebug(_T("Searching for file : %s"), myFileName.GetFullName().c_str());
		if (myFileName.FileExists() == false)
		{
			layer->m_LayerName = myFileName.GetName();
			return true;
		}
	}
	
	
	wxASSERT_MSG(0, _T("500 files searched for export name...."));
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
	
	
	wxString sTemp = _T("SELECT g.OBJECT_ID, g.OBJECT_GEOMETRY, o.OBJECT_CD,")
	_T(" o.OBJECT_DESC FROM  %s g LEFT JOIN %s a ON (g.OBJECT_ID")
	_T(" = a.OBJECT_GEOM_ID) LEFT JOIN %s o ON a.OBJECT_VAL_ID =")
	_T(" o.OBJECT_ID WHERE o.THEMATIC_LAYERS_LAYER_INDEX =%d ORDER BY g.OBJECT_ID ");
	
	wxString sSentence = wxString::Format(sTemp,
										  sGeomTable.c_str(),
										  sValTable.c_str(),
										  TABLE_NAME_OBJECTS.c_str(),
										  layer->m_LayerID);
	if (m_pDB->DataBaseQuery(sSentence)==false)
	{
		wxLogError(_("Error getting data"));
		return false;
	}
	
	bool bReturn = false;
	switch (layer->m_LayerType)
	{
		case LAYER_LINE:
			if (m_ExportData->WriteLines(layer))
				bReturn = true;
			break;
			
		case LAYER_POINT:
			if (m_ExportData->WritePoints(layer))
				bReturn = true;
			break;
			
		case LAYER_POLYGON:
			if (m_ExportData->WritePolygons(layer))
				bReturn = true;
			break;
			
		default:
			break;
	}
	
	m_pDB->DataBaseClearResults();
	
	return bReturn;
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
	
	if (m_pDB->DataBaseQuery(sSentence)==false)
	{
		nbvertex = 0;
		return NULL;
	}
	
	tmGISDataVectorMYSQL myFrameDB;
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
	long loid = 0;
	
	long myRows = 0;
	if(m_pDB->DataBaseGetResultSize(NULL, &myRows)==true)
	{
		if (myRows > 1)
			wxFAIL_MSG(_T("Too many frame results"));
	}
	
	
	wxRealPoint * myPt = myFrameDB.GetNextDataLine(nbvertex, loid);
	m_pDB->DataBaseClearResults();
	return myPt;
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
			
		case LAYER_POINT:
			if (m_ExportData->AddAdvancedDataToPoint(layer))
				return true;
			break;
			
		case LAYER_POLYGON:
			if (m_ExportData->AddAdvancedDataToPolygon(layer))
				return true;
			break;
			
		default:
			wxLogDebug(_T("Layer type not supported now : %d"), layer->m_LayerType);
			break;
	}
	
	return false;
	
}
