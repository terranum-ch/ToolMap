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
#include "tmdataintegrity.h"



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
	//wxASSERT (parent);
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
	
	
	// integrity check
	_CorrectIntegrity(myLayers);
	
	
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
	//wxASSERT(m_Parent);
	wxASSERT (localprojdef);
	m_ProjMem = localprojdef;
	
	PrjMemLayersArray * myLayers = m_ProjMem->m_PrjLayerArray ;
	if (!myLayers)
	{
		return false;
	}
	
	// correcting integrity
	_CorrectIntegrity(myLayers);
	
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
	
	
	// get frame
	int iFrameVertex = 0;
	wxRealPoint * pFrame = GetFrame(iFrameVertex);
	if (!pFrame)
		return false;
	
	CreateProgress(layers->GetCount(),
				   layers->Item(0).m_LayerName);
	
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
	if (GetAvailableFileName(layer) == false){
		return false;
	}
	
	// EXPORT
	m_ExportData = CreateExportData();
	wxASSERT(m_ExportData);
	m_ExportData->SetFrame(frame, framevertex);
	
	
	if (_CreateExportLayer(layer) == false){
		wxDELETE(m_ExportData);
		return false;
	}
	
	// in simple case we could do the job in one pass
	// but for polygons two pass are needed 
	switch (layer->m_LayerType) {
		case LAYER_LINE:
		case LAYER_POINT:
			if(_ExportSimple(layer)==false){
				wxLogError(_("Error exporting layer '%s'"), layer->m_LayerName.c_str());
				wxDELETE(m_ExportData);
				return false;
			}
			break;
			
			
		case LAYER_POLYGON:
			if(_ExportPolyGIS(layer)==false){
				wxLogError(_("Error exporting layer '%s'"), layer->m_LayerName.c_str());
				wxDELETE(m_ExportData);
				return false;
			}
			if (_ExportSimple(layer)==false) {
				wxLogError(_("Error exporting labels to polygon layer '%s'"),
						   layer->m_LayerName.c_str());
				wxDELETE(m_ExportData);
				return false;
			}
			break;

			
		default:
			wxLogError(_("Layer type not supported for export (%d)"),
					   layer->m_LayerType);
			wxDELETE(m_ExportData);
			return false;
			break;
	}
	
	wxDELETE(m_ExportData);
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
	
	// to allow unit testing without GUI
	if (m_Parent == NULL) {
		wxLogMessage(_T("No parent window specified, running console ?"));
		return;
	}
	

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
	// if no parent specified.
	if (m_Parent == NULL) {
		return false;
	}
	
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
	if (m_ProgressDlg){
		delete m_ProgressDlg;
	}
	
	if (m_ProgressBusy){
		delete m_ProgressBusy;
	}
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
bool tmExportManager::_CreateExportLayer (ProjectDefMemoryLayers * layer)
{
	wxASSERT(layer);
	wxASSERT (m_ExportData);

	
	// get size of object_description
	int iSizeOfObjCol = m_ExportData->GetSizeOfObjDesc(layer->m_LayerID);
	
	
	// create SIG layer
	if (m_ExportData->
		CreateEmptyExportFile(layer,
							  m_ExportPath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR))==false){
			return false;
		}

	// add toolmap's id field
	if (m_ExportData->AddFIDField()==false){
		return false;
	}
		
	// add obligatory (basic) fields
	if (m_ExportData->AddGenericFields(iSizeOfObjCol)==false){
		return false;
	}
		
	// add optionnal fields
	PrjMemFieldArray * myFields = layer->m_pLayerFieldArray;
	if (myFields){ // ok we have advanced fields
		m_ExportData->AddOptFields(myFields);
	}
		
	return true;
}



bool tmExportManager::_ExportSimple (ProjectDefMemoryLayers * layer){
	wxASSERT (m_ExportData);
	wxASSERT (layer->m_LayerType == LAYER_LINE || 
			  layer->m_LayerType == LAYER_POINT||
			  layer->m_LayerType == LAYER_POLYGON);
	
	//
	// build different query if layer has advanced fields or not
	//
	wxString myQuery = wxEmptyString;
	if (layer->m_pLayerFieldArray == NULL || layer->m_pLayerFieldArray->GetCount()== 0) {
		myQuery = wxString::Format(_T("SELECT l.OBJECT_ID, AsWKB(l.OBJECT_GEOMETRY),")
								   _T(" o.OBJECT_CD, o.OBJECT_DESC FROM %s")
								   _T(" l LEFT JOIN (%s la, %s o) ON (la.OBJECT_GEOM_ID")
								   _T(" = l.OBJECT_ID AND o.OBJECT_ID = la.OBJECT_VAL_ID)")
								   _T(" WHERE o.THEMATIC_LAYERS_LAYER_INDEX = %d ORDER BY l.OBJECT_ID"),
								   TABLE_NAME_GIS_GENERIC[layer->m_LayerType].c_str(),
								   TABLE_NAME_GIS_ATTRIBUTION[layer->m_LayerType].c_str(),
								   TABLE_NAME_OBJECTS.c_str(),
								   layer->m_LayerID);
	}
	else {
		myQuery = _T("SELECT l.OBJECT_ID, AsWKB(l.OBJECT_GEOMETRY), o.OBJECT_CD, o.OBJECT_DESC, ");
		PrjMemFieldArray * myFields = layer->m_pLayerFieldArray;
		wxASSERT(myFields);
		for (unsigned int i = 0; i< myFields->GetCount(); i++) {
			myQuery.Append(_T("laa."));
			myQuery.Append(myFields->Item(i).m_Fieldname);
			myQuery.Append(_T(", "));
		}
		myQuery.RemoveLast(2);
		myQuery.Append(wxString::Format(_T(" FROM (%s l  LEFT OUTER JOIN (layer_at%d laa)")
										_T(" ON (l.OBJECT_ID = laa.OBJECT_ID)) LEFT OUTER")
										_T(" JOIN (%s la, %s o) ON (l.OBJECT_ID = la.OBJECT_GEOM_ID")
										_T(" AND la.OBJECT_VAL_ID = o.OBJECT_ID) WHERE")
										_T(" o.THEMATIC_LAYERS_LAYER_INDEX = %d ORDER BY l.OBJECT_ID"),
										TABLE_NAME_GIS_GENERIC[layer->m_LayerType].c_str(),
										layer->m_LayerID,
										TABLE_NAME_GIS_ATTRIBUTION[layer->m_LayerType].c_str(),
										TABLE_NAME_OBJECTS.c_str(),
										layer->m_LayerID));
	}
	
	wxASSERT(m_pDB);
	if (m_pDB->DataBaseQuery(myQuery)==false) {
		return false;
	}
	
	// 
	// Message if some layers are exported empty
	//
	if (m_pDB->DataBaseHasResults() == false) {
		wxLogWarning(_("Layer '%s' exported but is empty"), layer->m_LayerName.c_str());
		return true;
	}

	switch (layer->m_LayerType)
	{
		case LAYER_LINE:
			if (m_ExportData->WriteLines(layer)==false){
				return false;	
			}
			break;
			
		case LAYER_POINT:
			if (m_ExportData->WritePoints(layer)==false){
				return false;
			}
			break;
			
		case LAYER_POLYGON:
			if (m_ExportData->WriteLabels(layer)==false){
				return false;
			}
			break;
			
		default:
			wxLogError(_("This type of data isn't supported for now"));
			break;
	}
	return true;
}




bool tmExportManager::_ExportPolyGIS (ProjectDefMemoryLayers * layer){
	wxASSERT(layer);
	wxASSERT(layer->m_LayerType == LAYER_POLYGON);
	
	//
	// get all lines constructing polygon layer 
	//
	wxString myLQuery = wxString::Format(_T("SELECT l.OBJECT_ID, AsWKB(l.OBJECT_GEOMETRY) FROM")
										 _T(" %s l LEFT JOIN (%s la, %s o) ON ")
										 _T("(la.OBJECT_GEOM_ID = l.OBJECT_ID AND")
										 _T(" o.OBJECT_ID = la.OBJECT_VAL_ID) WHERE")
										 _T(" o.THEMATIC_LAYERS_LAYER_INDEX = %d"),
										 TABLE_NAME_GIS_GENERIC[LAYER_LINE].c_str(),
										 TABLE_NAME_GIS_ATTRIBUTION[LAYER_LINE].c_str(),
										 TABLE_NAME_OBJECTS.c_str(),
										 layer->m_LayerID);
	wxASSERT(m_pDB);
	if (m_pDB->DataBaseQuery(myLQuery)==false) {
		return false;
	}
	
	// 
	// Message if some layers are exported empty
	//
	if (m_pDB->DataBaseHasResults() == false) {
		wxLogWarning(_("Layer '%s' exported but is empty"), layer->m_LayerName.c_str());
		return true;
	}
	
	if (m_ExportData->WritePolygons(layer)==false) {
		return false;
	}
	return true;
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
	
	
	wxLogError( _("%d files searched for export name, no name available remove files"),
			   tmFILENAME_MAX_SEARCH);
	return false;
}



void tmExportManager::_CorrectIntegrity(PrjMemLayersArray * layers){
	wxASSERT(layers);
	wxASSERT(m_pDB);
	
	
	if (layers->GetCount() == 0) {
		wxLogError(_("Correcting integrity isn't possible without layers"));
		return;
	}
	
	
	// integrity type checks
	
	tmDataIntegrity di(m_pDB);
	if (layers->GetCount() == 1) {
		di.CorrectType(layers->Item(0).m_LayerType);
	}
	else {
		di.CorrectType(LAYER_LINE);
		di.CorrectType(LAYER_POINT);
		di.CorrectType(LAYER_POLYGON);
	}
	
		
	// integrity advanced attribution checks
	for (unsigned int i = 0; i< layers->GetCount(); i++) {
		if (layers->Item(i).m_pLayerFieldArray->GetCount() > 0) {
			di.CorrectAAttrib(layers->Item(i).m_LayerID,
							  layers->Item(i).m_LayerType);
		}
	}
	
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
 @brief Get the frame
 @details Array of wxRealPoints returned must be destroyed by caller
 @param nbvertex the nb of vertex contained in the wxRealPoint array returned
 @return  An array of nbvertex item containing all vertex creating the frame
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
wxRealPoint *  tmExportManager::GetFrame (int & nbvertex)
{
	wxString myErr0 =	_("No Frame present, please create a frame for exporting data.\n");
	myErr0.Append(_("Note : Frame must be one closed line."));
	
	wxString myErr2 = _("Frame is incorrect. More than one line detected\n");
	myErr2.Append(_T("Note : Frame must be one closed line."));
	
	wxString myErr3 = _("Frame isn't closed. Please close the frame");
		
	wxString myCaptErr = _T("Frame error");
	
	
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
	
	bool bGetRowSize = m_pDB->DataBaseGetResultSize(NULL, &myRows);
	wxASSERT(bGetRowSize);
	
	wxString myMessage = wxEmptyString;
	// only one frame present
	if (myRows != 1)
	{
		if (myRows == 0)
			myMessage = myErr0;
		else
			myMessage = myErr2;
	
		wxMessageBox(myMessage, myCaptErr, wxICON_ERROR | wxOK  | wxCENTRE, m_Parent);
		m_pDB->DataBaseClearResults();
		nbvertex = 0;
		return NULL;
	}
	
	
	wxRealPoint * myPt = myFrameDB.GetNextDataLine(nbvertex, loid);
	
	// check for closed frame
	wxASSERT(myPt);
	if ( !wxIsSameDouble(myPt[0].x,myPt[nbvertex-1].x) || !wxIsSameDouble( myPt[0].y, myPt[nbvertex-1].y))
	{
		wxMessageBox(myErr3, myCaptErr, wxICON_ERROR | wxOK | wxCENTRE, m_Parent);
		m_pDB->DataBaseClearResults();
		return NULL;
	}
	
	
	m_pDB->DataBaseClearResults();
	return myPt;
}


