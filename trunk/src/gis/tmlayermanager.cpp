/***************************************************************************
						tmlayermanager.cpp
                    Central point for layer management
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

#include "tmlayermanager.h"
#include <wx/stdpaths.h>




DEFINE_EVENT_TYPE(tmEVT_THREAD_GISDATALOADED);
DEFINE_EVENT_TYPE(tmEVT_SELECTION_DONE);
DEFINE_EVENT_TYPE(tmEVT_VIEW_REFRESHED);
DEFINE_EVENT_TYPE(tmEVT_LM_ANGLE_CHANGED);

DEFINE_EVENT_TYPE(tmEVT_LM_MOVE_TO_FEATURE);
DEFINE_EVENT_TYPE(tmEVT_LM_ZOOM_TO_FEATURE);

BEGIN_EVENT_TABLE(tmLayerManager, wxEvtHandler)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_REMOVE,tmLayerManager::RemoveLayer)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_ADD,  tmLayerManager::AddLayer)
	EVT_MENU (ID_MENU_UNLINK_SPATIAL_DATA, tmLayerManager::OnRemoveLayers)
	EVT_COMMAND(wxID_ANY,tmEVT_LM_SIZE_CHANGED,   tmLayerManager::OnSizeChange)
	EVT_COMMAND(wxID_ANY,tmEVT_LM_MOUSE_MOVED, tmLayerManager::OnUpdateCoordinates)
	EVT_COMMAND(wxID_ANY, tmEVT_THREAD_GISDATALOADED, tmLayerManager::OnReloadProjectLayersDone)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_UPDATE, tmLayerManager::OnShowLayer)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_ZOOM_RECTANGLE_IN, tmLayerManager::OnZoomRectangleIn)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_ZOOM_RECTANGLE_OUT, tmLayerManager::OnZoomRectangleOut)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_ZOOM_TO_FEATURE,  tmLayerManager::OnZoomToFeature)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_MOVE_TO_FEATURE,tmLayerManager::OnMoveToFeature)
	EVT_COMMAND(wxID_ANY, tmEVT_SCALE_USER_CHANGED,tmLayerManager::OnScaleChanged)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_PAN_ENDED, tmLayerManager::OnPanFinished)
	//EVT_COMMAND(wxID_ANY, tmEVT_LM_SCROLL_MOVED, tmLayerManager::OnScrolled)
	EVT_COMMAND (wxID_ANY, tmEVT_LM_SHOW_PROPERTIES, tmLayerManager::OnDisplayProperties)
	EVT_COMMAND (wxID_ANY, tmEVT_LM_SELECTION,  tmLayerManager::OnSelection)
	EVT_COMMAND (wxID_ANY, tmEVT_LM_ANGLE_CHANGED, tmLayerManager::OnUpdateAngle)

END_EVENT_TABLE()


bool tmLayerManager::m_LogOn = true;

/***************************************************************************//**
 @brief Constructor
 @details Init values to default and init members.
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
tmLayerManager::tmLayerManager(wxWindow * parent, tmTOCCtrl * tocctrl, 
							   tmRenderer * renderer,  wxStatusBar * status,
							   tmScaleCtrlCombo * scalectrl)
{
	InitMemberValue();
	
	m_TOCCtrl = tocctrl;
	m_GISRenderer = renderer;
	m_Parent = parent;
	m_StatusBar = status;
	m_ScaleCtrl = scalectrl;
	m_Parent->PushEventHandler(this);
	
	m_Shared_ThreadStatus = tmTHREAD_STOP;
}


/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
tmLayerManager::~tmLayerManager()
{
	UnInitLayerManager();
	m_Parent->PopEventHandler(false);
	m_Parent->SetEventHandler(m_Parent);
}


/***************************************************************************//**
 @brief Init members to default
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
void tmLayerManager::InitMemberValue()
{
	m_TOCCtrl = NULL;
	m_Parent = NULL;
	m_DB = NULL;
	m_GISRenderer = NULL;
	m_Bitmap = NULL;
	m_StatusBar = NULL;
	m_Thread = NULL;
	m_Progress = NULL;
	m_ThreadBitmap = NULL;
	// init selected data structure
	m_Drawer.SetSelectedData(&m_SelectedData);
	m_BlockRefresh = false;
	m_LastOpenedPath = wxEmptyString;
}



/***************************************************************************//**
 @brief Init the layer manager with a project
 @details Call this function when a project is opened, this allows the needed
 initialization to takes place.
 @param db Pointer to a valid #DataBaseTM object (ASSERT is done in debug mode)
 @return  Always TRUE for the moment
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
bool tmLayerManager::InitLayerManager(DataBaseTM * db)
{
	// check
	wxASSERT_MSG (m_TOCCtrl != NULL, _T("Toc Ctrl is null, error"));
	wxASSERT_MSG (db != NULL, _T("Database pointer is empty... error"));
	
	// 1) init Database
	m_DB = db;
	
	// clear selected data
	m_SelectedData.Clear();
	
	// 2) Init TOCCtrl
	m_TOCCtrl->InsertProjectName(m_DB->DataBaseGetName());
	FillTOCArray();
	
	// 3) Init scale from database
	InitScaleCtrlList();
	
	wxClientDC dc (m_GISRenderer);
	m_Scale.SetPPI(dc.GetPPI());
	
	// 4) Load Data (not threaded)
	if(!LoadProjectLayers())
	{
		wxLogDebug(_T("Layers don't contains any spatial informations yet."));
	}	
	
	
	return TRUE;
}



/***************************************************************************//**
 @brief Un-Init the layer manager
 @details Call this function when a project is closed
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
bool tmLayerManager::UnInitLayerManager()
{
	
	// saving TOC status
	if (m_TOCCtrl->IsTOCReady())
		SaveTOCStatus();
	
	wxLogDebug(_T("Clearing TOC"));
	m_DB = NULL;
	
	// delete bitmap if existing 
	if(m_Bitmap != NULL)
	{
		//wxASSERT_MSG(0, _T("Bitmap should be allready deleted ?"));
		wxLogDebug(_T("deleting bitmap"));
		delete m_Bitmap;
		m_Bitmap = NULL;
	}
	m_GISRenderer->SetBitmapStatus();
	
	// clear the scale too
	m_Scale.SetMaxLayersExtent(tmRealRect(0,0,0,0));
	
	
	// clear the ctrl
	m_TOCCtrl->ClearAllLayers();
	
	return TRUE;
}


/***************************************************************************//**
 @brief Fill TOC control with Database TOC values
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
void tmLayerManager::FillTOCArray()
{
	wxASSERT_MSG (m_DB != NULL, _T("Database pointer is empty... error"));
	
	
	tmLayerProperties * lyrproptemp = NULL;
	//int iNumberAdded = 0;
	
	while (1)
	{
		lyrproptemp = m_DB->GetNextTOCEntry();
		
		if(lyrproptemp ==NULL)
		{
			break;
		}
		
		  if(!m_TOCCtrl->InsertLayer(lyrproptemp))
			  break;
	}
	
	m_TOCCtrl->ExpandAllLayers();
	
	wxLogDebug(_T("%d items added to TOC array"), m_TOCCtrl->GetCountLayers());
}



/***************************************************************************//**
 @brief Save TOC status to database
 @details This function called when closing project saves the status of the TOC
 ctrl. Only visibility rank of layers and vertex flags are saved, other 
 informations are saved in real time when adding a new layer or removing one.
 @return  TRUE if data saved to database, FALSE otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
bool tmLayerManager::SaveTOCStatus()
{
	wxASSERT_MSG(m_TOCCtrl, _T("Error TOC ctrl not defined"));
	
	tmLayerProperties * itemProp = NULL;
	unsigned int iRank = m_TOCCtrl->GetCountLayers();
	
	wxString sSentence = _T("");
	
	while (1)
	{
		if (iRank == m_TOCCtrl->GetCountLayers())
		{
			itemProp = m_TOCCtrl->IterateLayers(TRUE);
		}
		else
		{
			itemProp = m_TOCCtrl->IterateLayers(FALSE);
		}
		
		if (!itemProp)
			break;
		
		// serialize symbology //
		tmSerialize out;
		itemProp->m_LayerSymbol->Serialize(out);
		
		m_DB->PrepareTOCStatusUpdate(sSentence, itemProp, iRank, out.GetString());
		iRank --;
		
		if (iRank < 0)
			wxLogDebug(_T("Getting rank for layers seem wrong : %d"), iRank);
		
	}
	
	// update the database with toc status
	if (m_DB->DataBaseQueryNoResults(sSentence)==false)
	{
		wxLogDebug(_T("Error updating DB with TOC status : %s"),sSentence.c_str());
		return false;
	}
	
	return true;
	
}



/***************************************************************************//**
 @brief Response to the event sent by the #tmTOCCtrl
 @details Item was alleready delleted from the tmTOCCtrl and is now removed from
 the DB and the display is refreshed.
 @param event Contain the layer database ID into the GetExtraLong() function
 @author Lucien Schreiber (c) CREALP 2008
 @date 11 July 2008
 *******************************************************************************/
void tmLayerManager::RemoveLayer (wxCommandEvent & event)
{
	// checks where allready done by the tmTOCCtrl
	// we can delete item here from the DB
	long litemID = event.GetExtraLong();
	
	if (!m_DB->RemoveTOCLayer(litemID))
		return;
	
	wxLogDebug(_T("tmLayerManager : removing layer %d"), litemID);
	
	
	LoadProjectLayers();
}



// removing multiple layers with a dialog
void tmLayerManager::OnRemoveLayers(wxCommandEvent & event){
	// list support layers
	wxASSERT(m_TOCCtrl);
	bool bStart = true;
	PrjMemLayersArray myLayers;
	wxArrayString myLayersName;
	while (1) {
		tmLayerProperties * myLayerProp = m_TOCCtrl->IterateLayers(bStart);
		bStart = false;
		if (myLayerProp == NULL) {
			break;
		}
		if (myLayerProp->m_LayerType > TOC_NAME_NOT_GENERIC) {
			ProjectDefMemoryLayers myLayer;
			myLayer.m_LayerID = myLayerProp->m_LayerID;
			myLayer.m_LayerName = myLayerProp->GetDisplayName();
			myLayer.m_LayerType = (PRJDEF_LAYERS_TYPE) myLayerProp->m_LayerType;
			myLayers.Insert(myLayer, 0);
			myLayersName.Insert(myLayerProp->GetDisplayName(), 0);
		}
	}
	
	// display dialog
	wxMultiChoiceDialog myChoiceDlg (m_Parent, _("Select Layer(s) to close"),
									 _("Unlink layer(s)"),
									 myLayersName);
	if (myChoiceDlg.ShowModal() != wxID_OK) {
		return;
	}
	
	wxArrayInt myLayerToRemoveIndex = myChoiceDlg.GetSelections();
	if (myLayerToRemoveIndex.IsEmpty()) {
		wxLogWarning(_("Nothing selected, no layer will be closed"));
		return;
	}
	
 	// removing
	wxTreeItemId myItemId;
	for (unsigned int i = 0; i<myLayerToRemoveIndex.GetCount(); i++) {
		if (m_TOCCtrl->GetItemByID(myItemId,
									myLayers.Item(myLayerToRemoveIndex.Item(i)).m_LayerID)==false){
			wxLogError(_("Item with layer id : %d not found in the TOC"),
					   myLayers.Item(myLayerToRemoveIndex.Item(i)).m_LayerID);
			continue;
		}
		m_TOCCtrl->RemoveLayer(myItemId, true);
		
		if (m_DB->RemoveTOCLayer(myLayers.Item(myLayerToRemoveIndex.Item(i)).m_LayerID)==false) {
			wxLogError(_("Unable to remove layer : '%s'"),
					   myLayers.Item(myLayerToRemoveIndex.Item(i)).m_LayerName.c_str());
		}
	}
	
	LoadProjectLayers();
}



/***************************************************************************//**
 @brief Response to the event sent by the "Add Gis Data" menu
 @details This function is doing following operations :
 - Checking that #tmTOCCtrl is ready : tmTOCCtrl::IsTOCReady()
 - Importing data 
 - Saving layer to the database and getting back his real ID
 - Adding layer to the tmTOCCtrl
 with his real ID.
 @author Lucien Schreiber (c) CREALP 2008
 @date 11 July 2008
 *******************************************************************************/
void tmLayerManager::AddLayer (wxCommandEvent & event)
{
	// check that the a project was opened !
	// normal project contain 4 layers minimum
	if (IsOK() == false){
		return;
	}

	// create All supported extension
	wxString myExt = _("All supported formats|"); 
	wxArrayString myExts = tmGISData::GetAllSupportedGISFormatsExtensions();
	for (unsigned int i = 0; i<myExts.GetCount(); i++) {
		myExt.Append(myExts.Item(i));
		myExt.Append(_T(";"));
	}
	myExt.RemoveLast();
	myExt.Append(_T("|"));
	myExt.Append(tmGISData::GetAllSupportedGISFormatsWildcards());
	
	if (m_LastOpenedPath == wxEmptyString) {
		m_LastOpenedPath = wxStandardPaths::Get().GetDocumentsDir();
	}
	
	wxFileDialog myDlg (m_Parent, _("Link Data"),m_LastOpenedPath, _T(""),myExt);
	if(myDlg.ShowModal() == wxID_CANCEL){
		return;
	}
	
	m_LastOpenedPath = myDlg.GetDirectory();
	wxFileName myFilename (myDlg.GetPath());
	tmLayerProperties * item = new tmLayerProperties();
	item->InitFromPathAndName(myFilename.GetPath(),
							  myFilename.GetFullName(),
							  tmGISData::GetAllSupportedGISFormatsExtensions());
	
	// try to open the file for getting the spatial type
	tmGISData * myLayer = tmGISData::LoadLayer(item);
	if (!myLayer)
	{
		wxLogError(_("Not able to open the layer : %s"), item->GetDisplayName().c_str());
		return;
	}
	item->m_LayerSpatialType = myLayer->GetSpatialType();
	wxDELETE( myLayer);
	if (item->m_LayerSpatialType == LAYER_ERR || 
		item->m_LayerSpatialType == LAYER_SPATIAL_UNKNOWN)
	{
		return;
	}
	
	// init the symbology
	item->InitSymbology(wxEmptyString);
		
	
	// saving to the database and getting the last ID
	long lastinsertedID = m_DB->AddTOCLayer(item);
	if (lastinsertedID < 0)
		return;
	
	item->m_LayerID = lastinsertedID;
	wxLogDebug(_T("Last inserted item id is : %d"),lastinsertedID);
	
	// adding entry to TOC
	if(!m_TOCCtrl->InsertLayer(item))
		return;
	
	// re-load project
	LoadProjectLayers();


}


void tmLayerManager::ZoomToSelectedLayer(){
	
	tmLayerProperties * myLayerProp = m_TOCCtrl->GetSelectionLayer();
	if (myLayerProp == NULL) {
		wxLogError(_("No layer or incorrect layer selected"));
		return;
	}
	
	ZoomToLayer( myLayerProp->m_LayerID );
}



bool tmLayerManager::ZoomToLayer(long layerid){

	tmLayerProperties * myLayerProp = m_TOCCtrl->GetLayerById(layerid);
	if (myLayerProp == NULL) {
		wxLogError(_("Layer with specified id doesn't exists (ID = %d)"), layerid);
		return false;
	}
	
	tmGISData * myGISData = tmGISData::LoadLayer(myLayerProp);
	if (myGISData == NULL) {
		wxLogError(_("Unable to get data for layer %s"),
				   myLayerProp->m_LayerNameExt.c_str());
		return false;
	}
	
	wxASSERT(myGISData);
	tmRealRect myRealRect = myGISData->GetMinimalBoundingRectangle();
	vrRealRect myRRect;
	myRRect.SetLeftTop(wxPoint2DDouble(myRealRect.x_min, myRealRect.y_max));
	myRRect.SetRightBottom(wxPoint2DDouble(myRealRect.x_max, myRealRect.y_min));
	wxDELETE(myGISData);
	
	// layer extent seems correct
	if (myRRect.IsOk() == false) {
		wxLogError(_T("Layer extent isn't correct : %.3f, *.3f | %.3f, %.3f"),
				   myRRect.GetLeft(), myRRect.GetTop(), 
				   myRRect.GetRight(), myRRect.GetBottom());
		return false;
	}
	
	
	if (m_Scale.ZoomViewTo(myRRect)==false) {
		wxLogError(_T("Zooming to layer n\u00B0%d failed"), layerid);
		return false;
	}
	
	ReloadProjectLayersThreadStart(FALSE);
	_ZoomChanged();

	return true;
}



/***************************************************************************//**
 @brief Called when windows size change
 @details This function is called by the renderer (#tmRenderer) when the size
 change. It call the rendering of all layers in a multi-threaded way.
 @param event Contain a tmArraySize with :
 - Item 0 containing the old Size (in pixels).
 - Item 1 containing the new size (in pixels).
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 October 2008
 *******************************************************************************/
void tmLayerManager::OnSizeChange (wxCommandEvent & event)
{
	// pass size to scale object but don't make 
	// any computation if no project are opened
	tmArraySize * mySizes = (tmArraySize *) event.GetClientData();
	wxASSERT(mySizes);
	wxSize myOldSize = mySizes->Item(0);
	wxSize myNewSize = mySizes->Item(1);
	//bool mybSmaller = (bool) event.GetInt();
	delete mySizes;
	
	// actual wnd extent
	m_Scale.SetWindowExtent(wxRect(0,0,myNewSize.GetWidth(), myNewSize.GetHeight()));
	
	
	
	
	// compute reel size in MM
	wxClientDC dc (m_GISRenderer);
	m_Scale.SetWindowExtentMM(dc.GetSizeMM());
	
	
	// ensure that a project is opened
	if (!IsOK())
		return;
	// compute new extend in map unit
	
	// compute new pixel size
	//if (m_Scale.ComputeNewPixelSize(myOldSize, myNewSize))
	if (m_Scale.ComptuteNewWindowSize(myOldSize, myNewSize))
		ReloadProjectLayersThreadStart(false, false);
}



/***************************************************************************//**
 @brief Respond to mouse mouve inside renderer area
 @details This function is called automatically by the tmRenderer class when the
 mouse is moved inside the display area
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
void tmLayerManager::OnUpdateCoordinates (wxCommandEvent &event)
{
	wxPoint * pmousepoint = (wxPoint*) event.GetClientData();
	wxPoint mousepoint (*pmousepoint);
	wxDELETE(pmousepoint);
	
	if (!m_StatusBar){
		return;
	}
	
	// ensure that a project is opened
	if (!IsOK()){
		return;
	}
	
	if (!m_Scale.IsLayerExtentValid()){
		m_StatusBar->SetStatusText(_T(""), 1);
		return;
	}
	// clear status bar
	m_StatusBar->SetStatusText(_T(""), 0);
	
	
	wxRealPoint mouserealpoint = m_Scale.PixelToReal(mousepoint);
	wxString sCoord = wxString::Format(_T("x: %.*f -- y: %.*f"),
									   4, mouserealpoint.x, 4, mouserealpoint.y);
	m_StatusBar->SetStatusText(sCoord, 1);
}



void tmLayerManager::OnUpdateAngle(wxCommandEvent & event)
{
	if (!m_StatusBar)
		return;
	
	int myAngle = event.GetInt();
	
	if (myAngle == wxNOT_FOUND)
	{
		m_StatusBar->SetStatusText(_T(""), 3);
		return;
	}
	
	
	wxString myAngleText = wxString::Format(_T("Angle : %d\u00B0"), myAngle);
	m_StatusBar->SetStatusText(myAngleText, 3);
}


/***************************************************************************//**
 @brief Called when showing or hiding a layer
 @details This function is called by the TOC (#tmTOCCtrl) when a layers is
 displayed or hidden
 @param event empty.
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 October 2008
 *******************************************************************************/
void tmLayerManager::OnShowLayer (wxCommandEvent & event)
{
	ReloadProjectLayersThreadStart(FALSE);
}



/***************************************************************************//**
 @brief Called when user change scale
 @details This function is called by the Scale control (#tmScaleCtrlCombo) when
 user specify a scale.
 @param event empty.
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 October 2008
 *******************************************************************************/
void tmLayerManager::OnScaleChanged (wxCommandEvent & event)
{
	// checking that project is open and contain GIS data
	if (!IsOK() && m_Scale.GetMaxLayersExtent() == tmRealRect(0,0,0,0))
	{
		wxLogMessage(_T("Not able to process scale if no project open\n")
					 _T("or if not GIS data"));
		return;
	}
	
	// computing new real extent based on scale
	m_Scale.ComputeNewScaleExtent(event.GetExtraLong());
	
	//double dNewScaleDist = m_Scale.DistanceFromScale(event.GetExtraLong());
	ReloadProjectLayersThreadStart(FALSE);
	
	_ZoomChanged();
}



/***************************************************************************//**
 @brief Show properties dialog
 @param event Contain the #tmLayerProperties info. Use GetClientInfo() for
 getting info out.
 @author Lucien Schreiber (c) CREALP 2008
 @date 22 October 2008
 *******************************************************************************/
void tmLayerManager::OnDisplayProperties (wxCommandEvent & event)
{
	tmLayerProperties * itemProp = (tmLayerProperties*) event.GetClientData();
	if (!itemProp)
	{
		wxLogError(_("Problem during data transfert"));
		return;
	}
	
	
	tmGISData * myData = tmGISData::LoadLayer(itemProp);
	if (!myData)
	{
		wxLogError(_("Error loading GIS data for metadata"));
		return;
	}
	wxString myMetaData = myData->GetMetaDataAsHtml();
	wxDELETE(myData);
	
	if (itemProp->m_LayerSymbol->ShowSymbologyDialog(m_TOCCtrl,
													 myMetaData,
													 wxGetMousePosition())==wxID_OK)
	{
		ReloadProjectLayersThreadStart(FALSE);
	}
	
}


/***************************************************************************//**
 @brief Called when user press Zoom to fit
 @details This functions does the following :
 - Loading all visible layers (computing maximum extent)
 - Drawing
 This function is threaded, one can call
 this multiple time, if a thread allready exists it will be destroyed and a new
 one will be started
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
void tmLayerManager::OnZoomToFit ()
{
	ReloadProjectLayersThreadStart();
	
	_ZoomChanged();
}


void tmLayerManager::OnZoomRectangle()
{
	m_GISRenderer->SetTool(tmTOOL_ZOOM_RECTANGLE);
}



void tmLayerManager::OnPan ()
{
	m_GISRenderer->SetTool(tmTOOL_PAN);
}


void tmLayerManager::OnSelect()
{
	
	m_GISRenderer->SetTool(tmTOOL_SELECT);
}



/***************************************************************************//**
 @brief Searching data for selection
 @details This function search and update the #tmSelectedDataMemory structure
 with searched data. If this function return true we should redraw all layers
 @param rect area we sould search for selecting data
 @param shiftdown status of the shift key. If down we mainly add data to the
 selection
 @return  true if function succeed and we should reload layers, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
bool tmLayerManager::SelectedSearch (const wxRect & rect, bool shiftdown)
{
	// is a project open
	if (!m_TOCCtrl->IsTOCReady())
	{
		if (IsLoggingEnabled())
			wxLogMessage(_("Open a project first"));
		return false;
	}
	
	// is a layer selected and wich one
	tmLayerProperties * layerprop = m_TOCCtrl->GetSelectionLayer();
	if (!layerprop)
	{
		if (IsLoggingEnabled())
			wxLogMessage(_("Select a layer first in the TOC"));
		return false;
	}
	
	// converting wxRect to RealRect
	tmRealRect mySelReal = m_Scale.PixelsToReal(rect);
	tmGISData * myLayerData = tmGISData::LoadLayer(layerprop);
	if (!myLayerData)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Unable to get data for the layer"));
		return false;
	}
	
	//TODO: Remove, temp code for drawing rectangle
	//wxClientDC dc(m_GISRenderer);
	//dc.DrawLine(m_Scale.RealToPixel(wxRealPoint(mySelReal.x_min, mySelReal.y_max)),
	//			m_Scale.RealToPixel(wxRealPoint(mySelReal.x_max, mySelReal.y_min)));
	
	
	// searching for data
	wxArrayLong * myArray = myLayerData->SearchData(mySelReal, layerprop->m_LayerType);
	if (myArray == NULL){
		myArray = new wxArrayLong();
	}
	int myArrayCount = myArray->GetCount();
		
	// are we in the same layer ?
	m_SelectedData.SetLayerID(layerprop->m_LayerID);
	
	if (IsLoggingEnabled()){
		wxLogDebug(_T("Number of features selected : %d"), myArrayCount);
	
		for (int i = 0; i < myArrayCount; i++)
		{
			if (i == 10)
			{
				wxLogDebug(_T("- More selected features selected .... "));
				break;
			}
			wxLogMessage(_T(" - Selected Features ID : %d"), myArray->Item(i));
		}
	}
		
	
	// add, remove or clear selection depending on :
	// - number of items selected
	// - Status of shift key.
	if (myArrayCount >= 1)
	{
		if (shiftdown == false)
			m_SelectedData.Clear();
		m_SelectedData.AddSelected(myArray);
	}
	else
	{
		if (shiftdown == false)
			m_SelectedData.Clear();
	}
	
	myArray->Clear();
	wxDELETE(myArray);
	wxDELETE(myLayerData);
	//wxLogDebug(_T("Number of features flaged as selected : %d"),
	//		   m_SelectedData.GetCount());
	
	return true;
}



/***************************************************************************//**
 @brief Clear all selected objects
 @details This function clear all selected objects
 @return  true if they where selected items, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 31 March 2009
 *******************************************************************************/
bool tmLayerManager::SelectedClear ()
{
	bool bReturn = false;
	if (m_SelectedData.GetCount() > 0)
		bReturn = true;
	
	m_SelectedData.Clear();
	ReloadProjectLayersThreadStart(false, true);
	
	wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
	m_Parent->GetEventHandler()->AddPendingEvent(evt);
	return bReturn;
}



/***************************************************************************//**
 @brief Invert selected values
 @return true if sucess, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 31 March 2009
 *******************************************************************************/
bool tmLayerManager::SelectedInvert ()
{
	// selection have value ?
	if (m_SelectedData.GetCount() == 0)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("No value selected, select object(s) first"));
		return false;
	}
	
	
	// get all selected values
	tmLayerProperties * layerprop = m_TOCCtrl->GetSelectionLayer();
	if (!layerprop)
	{
		if (IsLoggingEnabled())
			wxLogMessage(_("Select a layer first in the TOC"));
		return false;
	}

	tmGISData * myLayerData = tmGISData::LoadLayer(layerprop);
	if (!myLayerData)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Unable to get data for the layer"));
		return false;
	}
	
	wxArrayLong * myAllData = myLayerData->GetAllData();
	if (!myAllData)
		return false;
	
		
	// actual values
	wxArrayLong * myActualSelVal = m_SelectedData.GetSelectedValues();
	wxASSERT (myActualSelVal);
	
	m_SelectedData.Clear();
	m_SelectedData.AddSelected(myAllData);
	m_SelectedData.Remove(myActualSelVal);

	delete myAllData;
	delete myActualSelVal;
	
	ReloadProjectLayersThreadStart(false, true);
	
	wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
	m_Parent->GetEventHandler()->AddPendingEvent(evt);
	return true;
}


bool tmLayerManager::SelectByOid (){
	//	
	// Some checks for 
	// - Project open
	// - Layer selected
	//
	if (!m_TOCCtrl->IsTOCReady()){
		wxLogError(_("No project opened, open a project first"));
		return false;
	}
	tmLayerProperties * layerprop = m_TOCCtrl->GetSelectionLayer();
	if (!layerprop){
		wxLogError(_("No layer selected, select a layer in the Table of content"));
		return false;
	}
	
	wxASSERT(m_Parent);
	long myOid = wxGetNumberFromUser (wxEmptyString,
						 _("Feature ID: "),
						 _("Select by feature ID"),
						 1, 0, 1000000,
						 m_Parent);
	
	if (myOid == wxNOT_FOUND) {
		return false;
	}
	
	wxArrayLong mySel;
	mySel.Add(myOid);
	m_SelectedData.Clear();
	m_SelectedData.AddSelected(&mySel);
	
	ReloadProjectLayersThreadStart(false, true);
	
	wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
	m_Parent->GetEventHandler()->AddPendingEvent(evt);
	return true;
}



void tmLayerManager::OnZoomRectangleIn (wxCommandEvent & event)
{
	// computing best ratio between actuall windows size 
	// and selection size
	wxRect * mySelectedRect = (wxRect *) event.GetClientData();
	double dSmaller = m_Scale.GetBestDivFactor(*mySelectedRect);
	
	// calcul the new reduced window size (px)
	wxRect myActualExtent = m_Scale.GetWindowExtent();
	wxRect myNewWndExtent (0,0,0,0);
	double dwidth = ( 1.0 * myActualExtent.GetWidth()) / dSmaller;
	double dheight = (1.0 * myActualExtent.GetHeight()) / dSmaller;
	
	myNewWndExtent.SetWidth(wxRound(dwidth));
	myNewWndExtent.SetHeight(wxRound(dheight));
	
	// modify the real window extent
	m_Scale.ComputeNewRealZoomExtent(myNewWndExtent, 
								 wxPoint(mySelectedRect->GetX(),
										 mySelectedRect->GetY()));
	
	// reload data
	ReloadProjectLayersThreadStart(FALSE);
	delete mySelectedRect;
	
	_ZoomChanged();
}



void tmLayerManager::OnZoomRectangleOut (wxCommandEvent & event)
{
	wxRect * mySelectedRect = (wxRect *) event.GetClientData();
	
	// compute ratio between actual window size and selection
	double dBigger = m_Scale.GetBestDivFactor(*mySelectedRect);
	
	// calcul the new bigger window size (px)
	wxRect myActualExtent = m_Scale.GetWindowExtent();
	wxRect myNewWndExtent (0,0,0,0);
	double dwidth = ((double) myActualExtent.GetWidth()) * dBigger;
	double dheight = ((double) myActualExtent.GetHeight()) * dBigger;
	
	myNewWndExtent.SetWidth(wxRound(dwidth));
	myNewWndExtent.SetHeight(wxRound(dheight));
	
	// modify the real window extent
	m_Scale.ComputeNewRealZoomExtent(myNewWndExtent, 
								 wxPoint(-mySelectedRect->GetX(),
										 -mySelectedRect->GetY()));
	
	// reload data
	ReloadProjectLayersThreadStart(FALSE);
	delete mySelectedRect;
	_ZoomChanged();
}



void tmLayerManager::OnZoomToFeature (wxCommandEvent & event){
	long myOid = event.GetExtraLong();
	
	// getting geometry
	long myLayerID = m_SelectedData.GetSelectedLayer();
	tmLayerProperties * myLayerProp = m_TOCCtrl->GetLayerById(myLayerID);
	
	tmGISDataVector * myVectorData = (tmGISDataVector*) tmGISData::LoadLayer(myLayerProp);
	wxASSERT(myVectorData);
	
	OGRFeature * myFeature = myVectorData->GetFeatureByOID(myOid);
	wxASSERT(myFeature);
	delete myVectorData;
	
	OGREnvelope myEnveloppe;
	myFeature->GetGeometryRef()->getEnvelope(&myEnveloppe);
	OGRFeature::DestroyFeature(myFeature);
	
	//zooming
	vrRealRect myRect;
	myRect.SetLeftTop(wxPoint2DDouble(myEnveloppe.MinX, myEnveloppe.MaxY));
	myRect.SetRightBottom(wxPoint2DDouble(myEnveloppe.MaxX, myEnveloppe.MinY));
	if (m_Scale.ZoomViewTo(myRect) == false){
		wxLogError(_T("Error zooming to feature n\u00B0%d"), myOid);
		return;
	}
	
	ReloadProjectLayersThreadStart(FALSE);
	_ZoomChanged();
}


void tmLayerManager::OnMoveToFeature (wxCommandEvent & event){
	long myOid = event.GetExtraLong();
	
	// getting geometry
	long myLayerID = m_SelectedData.GetSelectedLayer();
	tmLayerProperties * myLayerProp = m_TOCCtrl->GetLayerById(myLayerID);
	
	tmGISDataVector * myVectorData = (tmGISDataVector*) tmGISData::LoadLayer(myLayerProp);
	wxASSERT(myVectorData);
	
	OGRFeature * myFeature = myVectorData->GetFeatureByOID(myOid);
	wxASSERT(myFeature);
	delete myVectorData;
	
	OGREnvelope myEnveloppe;
	myFeature->GetGeometryRef()->getEnvelope(&myEnveloppe);
	OGRFeature::DestroyFeature(myFeature);
	
	//moving
	vrRealRect myRect;
	myRect.SetLeftTop(wxPoint2DDouble(myEnveloppe.MinX, myEnveloppe.MaxY));
	myRect.SetRightBottom(wxPoint2DDouble(myEnveloppe.MaxX, myEnveloppe.MinY));
	if (m_Scale.MoveViewTo(myRect) == false){
		wxLogError(_T("Error moving to feature %d"), myOid);
		return;
	}
	
	ReloadProjectLayersThreadStart(FALSE);
}



void tmLayerManager::OnPanFinished (wxCommandEvent & event)
{
	wxPoint * myNewPosPx = (wxPoint*) event.GetClientData();
	
	// change coordinates
	m_Scale.ComputeNewRealPanExtent(*myNewPosPx);
	ReloadProjectLayersThreadStart(FALSE);
	
	
	delete myNewPosPx;
	
	_ZoomChanged();
}



/***************************************************************************//**
 @brief Called when selection was made
 @details This is called by the #tmRenderer when a selection (either by point or
 rectangle) was made
 @param event Contain data about the status of the shift key and the rectangle
 size. 
 - Use event.GetInt() for getting the status of the shift key
 - Use event.GetClientObject() for getting the wxRect.
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
void tmLayerManager::OnSelection (wxCommandEvent & event)
{
	bool IsShiftDown = static_cast<bool> (event.GetInt());
	wxRect * mySelectedRect = (wxRect *) event.GetClientData();
	
	if (SelectedSearch(*mySelectedRect, IsShiftDown))
	{
		//if (IsLoggingEnabled())
		//	wxLogDebug(_T("Selection done"));
		
		ReloadProjectLayersThreadStart(false, false);
	}
	
	
	wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
	m_Parent->GetEventHandler()->AddPendingEvent(evt);
	
	delete mySelectedRect;
	
}



/***************************************************************************//**
 @brief Is the layer manager ready with some project
 @return  TRUE if some layers are loaded into the project, FALSE otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
bool tmLayerManager::IsOK()
{
	// ensure that TOC ctrl isn't empty
	if (m_TOCCtrl->GetCountLayers() == 0)
	{
		//wxLogDebug(_T("No data loaded into the TOC ctrl, load data into the TOC first"));
		return FALSE;
	}
	return TRUE;
}


/***************************************************************************//**
 @brief Load all layers (non threaded)
 @details This functions does closely the same that the
 #ReloadProjectLayersThreadStart() one but isn't threaded and is called during
 project opening.
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
bool tmLayerManager::LoadProjectLayers()
{
	// enable logging
	tmGISData::EnableLogging(true);
	tmDrawer::EnableLogging(true);
	
	if (!IsOK())
		return FALSE; 
	
	// invalidate bitmap
	m_GISRenderer->SetBitmapStatus();
	CreateEmptyBitmap(wxSize (m_Scale.GetWindowExtent().GetWidth(),
							  m_Scale.GetWindowExtent().GetHeight()));

		
	int iRead = ReadLayerExtent(true);
	wxLogDebug(_T("%d layer(s) read"),iRead);
	
	
	m_Scale.ComputeMaxExtent();
	// test validity of layers extent. If no extent is 
	// specified (like no data displayed) return 
	if (!m_Scale.IsLayerExtentValid())
		return FALSE;
	
	
	m_Drawer.InitDrawer(m_Bitmap, m_Scale, m_Scale.GetWindowExtentReal());
	
	
	// iterate for drawing layers
	ReadLayerDraw();
	
	
	// update scale
	m_ScaleCtrl->SetValueScale(m_Scale.GetActualScale());
	
	m_Drawer.DrawExtentIntoBitmap(2,*wxRED);
	
	// send view updated message 
	//ViewUpdated();
	
	// set active bitmap	
	m_GISRenderer->SetBitmapStatus(m_Bitmap);
	if (m_Bitmap)
	{
		delete m_Bitmap;
		m_Bitmap = NULL;
	}
		
	m_GISRenderer->Refresh();
	
	// previous zoom
	m_ZoomManager.Clear();
	_ZoomChanged();
	
	// send view updated message 
	ViewUpdated();

	
	return TRUE;
}

/*void myTestFunc()
{
	char * myTest = new char[10000000];
	//delete [] myTest;
}*/


// non threaded version for tests
//FIXME: Convert to threaded version when stable
bool tmLayerManager::ReloadProjectLayersThreadStart(bool bFullExtent, bool bInvalidateFullExt)
{
	// invalidate bitmap
	m_GISRenderer->SetBitmapStatus();
	CreateEmptyBitmap(wxSize (m_Scale.GetWindowExtent().GetWidth(),
							  m_Scale.GetWindowExtent().GetHeight()));
	
	// invalidate max_extent
	if (bInvalidateFullExt)
		m_Scale.SetMaxLayersExtent(tmRealRect(0,0,0,0));
	

	
	int iRead = ReadLayerExtent(true);
	//wxLogDebug(_T("%d layer(s) read"),iRead);
	if (iRead == -1)
		return false;
	
		
	if (m_Scale.IsLayerExtentValid()==false)
	{
		wxLogError(_T("Extend not valid"));
		//wxASSERT_MSG(0,_T("Failed computing"));
		return false;
	}
		

	// compute max extent if required by option
	if (bFullExtent)
		m_Scale.ComputeMaxExtent();
	
	
	// read layers for drawing
	m_Drawer.InitDrawer(m_Bitmap, m_Scale, m_Scale.GetWindowExtentReal());
	int iNbLayersDraw = ReadLayerDraw();
	if (iNbLayersDraw == -1)
		return false;
	
	// update scale
	m_ScaleCtrl->SetValueScale(m_Scale.GetActualScale());
	
	// update scrollbars
	//UpdateScrollBars();
	
	// set active bitmap	
	m_GISRenderer->SetBitmapStatus(m_Bitmap);
	if (m_Bitmap)
	{
		delete m_Bitmap;
		m_Bitmap = NULL;
	}
	
	m_GISRenderer->Refresh();
	m_GISRenderer->Update();
	
	ViewUpdated();
	return true;
}

#if (0)
bool tmLayerManager::ReloadProjectLayersThreadStart(bool bFullExtent, bool bInvalidateFullExt)
{
	if (m_BlockRefresh)
		return false;
	
	// init values
	m_computeFullExtent = bFullExtent;
	
	// invalidate bitmap
	//m_GISRenderer->SetBitmapStatus();
	//CreateEmptyBitmap(wxSize (m_Scale.GetWindowExtent().GetWidth(),
	//						  m_Scale.GetWindowExtent().GetHeight()));
	
	// prepare loading of mySQL data in tmGISData
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_DB);
	
	// invalidate max_extent
	if (bInvalidateFullExt)
		m_Scale.SetMaxLayersExtent(tmRealRect(0,0,0,0));
	

	// stop an existing thread if needed.
	if (s_SharedDataCritical.TryLock() == wxMUTEX_NO_ERROR)
	{
		if (m_Shared_ThreadStatus == tmTHREAD_RUN)
		{
			m_Shared_ThreadStatus = tmTHREAD_STOP;
			s_SharedDataCritical.Unlock();
			m_Thread->Delete();
		}
		else
			s_SharedDataCritical.Unlock();
		
		// disable logging (Mac OS thread problem)
		tmGISData::EnableLogging(false);
		tmDrawer::EnableLogging(false);
		tmLayerManager::EnableLogging(false);
		
		// display a progress thread
		// only if nothing is displayed
		if (!m_Progress)
		{
			m_Progress = new tmProgressIndicator(m_Parent, m_StatusBar);
			m_Progress->SetMessage(TMPROGRESS_DRAW_DATA);
			m_Progress->DisplayProgress();
		}
		
		// create and launch thread
		wxSize myScreenSize (m_Scale.GetWindowExtent().GetWidth(),
							 m_Scale.GetWindowExtent().GetHeight());
		m_Thread = new tmGISLoadingDataThread(m_Parent, m_TOCCtrl, &m_Scale, m_DB, 
											  &m_Drawer, &m_Shared_ThreadStatus,
											  &m_ThreadBitmap,
											  bFullExtent,
											  myScreenSize);
		if (m_Thread->Create() != wxTHREAD_NO_ERROR)
		{
			wxLogError(_T("Can't create thread for GIS data loading"));
			return FALSE;
		}
		wxLogDebug(_T("Gis thread started..."));
		m_Thread->Run();
		return TRUE;
	}
	wxLogDebug(_T("Unable to lock mutex during thread start"));
	return FALSE;
}	
#endif


void tmLayerManager::OnReloadProjectLayersDone (wxCommandEvent & event)
{
	wxFAIL;
	// enable logging
	tmGISData::EnableLogging(true);
	tmDrawer::EnableLogging(true);
	tmLayerManager::EnableLogging (true);
	
	wxLogDebug(_T("GIS thread finished without interuption"));
	m_Thread = NULL; // thread finished
	
	// stoping progress display
	if (m_Progress)
	{
		m_Progress->StopProgress();
		m_Progress = NULL;
	}
	
	
	// test validity of layers extent. If no extent is 
	// specified (like no data displayed)  
	//if (m_Scale.IsLayerExtentValid())
	//{
	// compute max extent if required by option
	//	if (m_computeFullExtent)
	//		m_Scale.ComputeMaxExtent();
	
	if (s_SharedDataCritical.TryLock() == wxMUTEX_NO_ERROR)
	{
		if (m_ThreadBitmap && m_ThreadBitmap->IsOk())
		{
			wxSize myWndpxSize (m_Scale.GetWindowExtent().GetWidth(), m_Scale.GetWindowExtent().GetHeight());
			CreateEmptyBitmap(myWndpxSize);
			
			wxMemoryDC dc;
			dc.SelectObject(*m_Bitmap);
			dc.DrawBitmap(*m_ThreadBitmap, 0, 0, TRUE);
			dc.SelectObject(wxNullBitmap);
			
		}
		else
		{
			wxLogDebug(_T("Error with bitmap."));
		}
		s_SharedDataCritical.Unlock();
		
		// update scale
		m_ScaleCtrl->SetValueScale(m_Scale.GetActualScale());
		
		// update scrollbars
		//UpdateScrollBars();
		
		
		
	}
	
	// set active bitmap	
	m_GISRenderer->SetBitmapStatus(m_Bitmap);
	m_GISRenderer->Refresh();
	
	ViewUpdated();
	
	return;
	
}



/***************************************************************************//**
 @brief Sent a message when view is updated
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmLayerManager::ViewUpdated()
{
	// send view updated message 
	wxCommandEvent evt(tmEVT_VIEW_REFRESHED, wxID_ANY);
	m_Parent->GetEventHandler()->AddPendingEvent(evt);
}




/***************************************************************************//**
 @brief Creating an empty white bitmap
 @param size the size (in pixels) of the new bitmap to create
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
void tmLayerManager::CreateEmptyBitmap (const wxSize & size)
{
	if (m_Bitmap != NULL)
	{
		//if (m_Bitmap->IsOk())
		//{
		//wxLogDebug(_T("deleting bitmap"));
		delete m_Bitmap;
		m_Bitmap = NULL;
		//}
	}
	
	m_Bitmap = new wxBitmap(size.GetWidth(), size.GetHeight());
				   //wxLogDebug(_T("Creating bitmap"));

	// ensure that the bitmap is white (win and linux)
	wxMemoryDC dc;
	dc.SelectObject (*m_Bitmap);
	dc.SetBackground(wxBrush(*wxWHITE_BRUSH));
	dc.Clear();
}



/***************************************************************************//**
 @brief Iterate all layers for extent
 @details This function iterates through all layers, loading layers and their
 extent.
 @param loginfo TRUE if the function should log some info about what is done.
 <B>When used in thread, this parameter has to be set to false</B>
 @return  the number of layers really read, a. k. a. number of visible layers
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 September 2008
 *******************************************************************************/
int tmLayerManager::ReadLayerExtent(bool loginfo)
{
	// iterate throught all layers
	int iRank = 0;
	int iReaded = 0;
	
	tmLayerProperties * pLayerProp = NULL;
	tmRealRect myExtent (0,0,0,0);
	
	// prepare loading of MySQL data
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_DB);
	while (1)
	{
		if (iRank == 0)
		{
			pLayerProp = m_TOCCtrl->IterateLayers(TRUE);
		}
		else
		{
			pLayerProp = m_TOCCtrl->IterateLayers(FALSE);
		}
		
		if (!pLayerProp)
			break;
		
		
		if (pLayerProp->m_LayerVisible == true) {
			
			// loading data
			tmGISData * layerData = tmGISData::LoadLayer(pLayerProp);
			
			// processing and deleting data
			if (layerData)
			{
				myExtent = layerData->GetMinimalBoundingRectangle();
				m_Scale.SetMaxLayersExtentAsExisting(myExtent);
				iReaded ++;
				
				// show some logging info, not working
				// in thread mode
				/*if (loginfo)
				{
					wxLogDebug(_T("Minimum rectangle is : %.*f - %.*f, %.*f - %.*f"),
							   2,myExtent.x_min, 2, myExtent.y_min,
							   2, myExtent.x_max, 2, myExtent.y_max);
				}*/
			}
			
			if (layerData != NULL)
				delete layerData;
		}
		
		
		iRank ++;
	}
	return iReaded;
}



/***************************************************************************//**
 @brief Iterate all visible layers for drawing them
 @return  Number of layers really drawn (visible layers)
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 September 2008
 *******************************************************************************/
int tmLayerManager::ReadLayerDraw ()
{
	// iterate throught all layers
	int iRank = 0;
	int iReaded = 0;
	
	tmLayerProperties * pLayerProp = NULL;
	tmRealRect myExtent (0,0,0,0);
	
	// prepare loading of MySQL data
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_DB);
	while (1)
	{
		if (iRank == 0)
		{
			pLayerProp = m_TOCCtrl->IterateLayers(TRUE);
		}
		else
		{
			pLayerProp = m_TOCCtrl->IterateLayers(FALSE);
		}
		
		if (!pLayerProp)
			break;
		
		if (pLayerProp->m_LayerVisible == true) {
			// loading data
			tmGISData * layerData = tmGISData::LoadLayer(pLayerProp);
			
			// processing and deleting data
			if (layerData && pLayerProp->m_LayerVisible)
			{
				// draw layer data
				m_Drawer.Draw(pLayerProp, layerData);
				iReaded ++;
			}
			
			if (layerData != NULL)
				delete layerData;
		}
		
		iRank ++;
	}
	return iReaded;
}



void tmLayerManager::InitScaleCtrlList ()
{
	wxASSERT (m_DB);
	wxArrayLong  myValues;
	
	long myScale = 0;
	long myCount = 0;
	long myDBIndex = -1;
	
	myScale = m_DB->GetNextScaleValue(myDBIndex, TRUE);
	
	while (1)
	{
		if (myScale != -1)
		{
			// adding scale in the list
			myValues.Add(myScale);
			myCount++;
		}
		else
			break;
		
		myScale = m_DB->GetNextScaleValue(myDBIndex, FALSE);
	}
	
	// send message 
	m_ScaleCtrl->InitScaleFromDatabase(myValues);
	
}



void tmLayerManager::_ZoomChanged()
{
	
	tmZoomExtent myActualExtent;
	myActualExtent.m_ZoomFactor = m_Scale.GetPixelSize();
	myActualExtent.m_TopLeftPosition = m_Scale.GetTopLeftValue();
	if (myActualExtent.IsOk()==false)
	{
		wxLogError(_("Incorrect zoom extent set, unable to pass this value to the zoom manager"));
		return; 
	}
	m_ZoomManager.Add(myActualExtent);
}




bool tmLayerManager::ZoomPrevious()
{
	tmZoomExtent myPrevExtent;
	if (m_ZoomManager.GetPrevious(myPrevExtent)==false)
	{
		return false;
	}
	
	wxASSERT(myPrevExtent.IsOk());
	
	
	m_Scale.ComputePrevZoomExtent(myPrevExtent.m_ZoomFactor, myPrevExtent.m_TopLeftPosition);
	ReloadProjectLayersThreadStart(false, false);	
	return true;
}


bool tmLayerManager::HasZoomPrevious(){
	bool bReturn = false;
	if (m_ZoomManager.GetCount() > 0) {
		bReturn = true;
	}
	return bReturn;
}





/****************************** THREAD CLASS FUNCTION BELOW ***********************/

/***************************************************************************//**
 @brief Thread constructor
 @param parent A valid pointer to a wxWindow object (object must be able to deal
 with event such as wxFrame, wxControls and so on)
 @param toc a valid pointer to the tmTOCCtrl
 @param scale 
 @param database 
 @param drawer
 @param threadstatus
 @param threadbitmap
 @param computefullextent Should we compute the full extent
 @param wndpxsize Size of the windows in pixels
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
tmGISLoadingDataThread::tmGISLoadingDataThread(wxWindow * parent, tmTOCCtrl * toc,
											   tmGISScale * scale,
											   DataBaseTM * database,
											   tmDrawer * drawer,
											   tmTHREAD_STATUS * threadstatus,
											   wxBitmap ** threadbitmap,
											   bool computefullextent,
											   const wxSize & wndpxsize)
{
	m_Parent = parent;
	m_TOC = toc;
	m_Scale = scale;
	m_DB = database;
	m_Stop = FALSE;
	m_Drawer = drawer;
	m_ThreadStatus = threadstatus;
	m_ThreadBmp = threadbitmap;
	m_computeFullExtent = computefullextent;
	
	// creating img for drawing
	CreateEmptyBitmap(wndpxsize.GetWidth(), wndpxsize.GetHeight());
	
}

tmGISLoadingDataThread::~tmGISLoadingDataThread()
{
	
}




/***************************************************************************//**
 @brief Entry point for thread
 @details This function is loading the layers using thread. Allowing user to
 stop the loading process if for exemple the image size has changed
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
void * tmGISLoadingDataThread::Entry()
{	
	// indicate to main that thread is running
	if (!s_SharedDataCritical.TryLock() == wxMUTEX_NO_ERROR)
		return NULL;
		
	* m_ThreadStatus = tmTHREAD_RUN;
	s_SharedDataCritical.Unlock();
	

	
	//if (!CreateEmptyBitmap(m_Scale->GetWindowExtent().GetWidth(),
	//					  m_Scale->GetWindowExtent().GetHeight()))
	//	return NULL;
	
	
	// read layers extent
	int iNbLayers = ReadLayerExtentThread();
	if (iNbLayers == -1)
		return NULL;
	
	
	if (m_Scale->IsLayerExtentValid())
	{
		// compute max extent if required by option
		wxMutexGuiEnter();
		if (m_computeFullExtent)
			m_Scale->ComputeMaxExtent();
		
		
		// read layers for drawing
		
		m_Drawer->InitDrawer(*m_ThreadBmp, *m_Scale, m_Scale->GetWindowExtentReal());
		int iNbLayersDraw = ReadLayerDraw();
		wxMutexGuiLeave();
		if (iNbLayersDraw == -1)
			return NULL;
		
		
		// temp loosing time code
		/*for (int i = 0;i<5; i++)
		{
			if (TestDestroy())
				return NULL;
			
			wxThread::Sleep(200);
		}*/
		if (TestDestroy())
			return NULL;
	}
	
	// if thread finished correctly, send a message to
	// the layer manager to display the new image
	if(!s_SharedDataCritical.TryLock() == wxMUTEX_NO_ERROR)
		return NULL;
	
	* m_ThreadStatus = tmTHREAD_STOP;
	s_SharedDataCritical.Unlock();

	wxCommandEvent evt(tmEVT_THREAD_GISDATALOADED, wxID_ANY);
	m_Parent->GetEventHandler()->AddPendingEvent(evt);
		
	return NULL;
}



/***************************************************************************//**
 @brief Iterate all layers for extent (threaded version
 @details This function iterates through all layers, loading layers and their
 extent.
 @return  the number of layers read or -1 if the thread sould be stopped
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 September 2008
 *******************************************************************************/
int tmGISLoadingDataThread::ReadLayerExtentThread()
{
	/*
	// iterate throught all layers
	int iRank = 0;
	int iReaded = 0;
	bool bInteruptedThread = false;
	
	tmLayerProperties * pLayerProp = NULL;
	tmRealRect myExtent (0,0,0,0);
	
	// Init new thread
	m_DB->DataBaseThreadInit();
	
	while (1)
	{
		if (iRank == 0)
		{
			pLayerProp = m_TOC->IterateLayers(TRUE);
		}
		else
		{
			pLayerProp = m_TOC->IterateLayers(FALSE);
		}
		
		if (!pLayerProp)
			break;
		
		// check for thread stopped.
		if (TestDestroy())
		{
			bInteruptedThread = true;
			break;
		}
		
		// loading data
		tmGISData * layerData = tmGISData::LoadLayer(pLayerProp);
		
		// processing and deleting data
		if (layerData && pLayerProp->m_LayerVisible)
		{
			myExtent = layerData->GetMinimalBoundingRectangle();
			m_Scale->SetMaxLayersExtentAsExisting(myExtent);
			iReaded ++;
			
			delete layerData;
		}
		iRank ++;
	}
	
	// uninit thread variables
	m_DB->DataBaseThreadEnd();
	
	// if thread need to be stopped, then 
	// return -1.
	if (bInteruptedThread)
		iReaded = -1;
	
	return iReaded;	*/
	return -1;
}



/***************************************************************************//**
 @brief Draw layers into bitmap [THREADED]
 @return  number of layers drawed, or -1 if thread was stopped and need to
 return immediatelly
 @author Lucien Schreiber (c) CREALP 2008
 @date 11 October 2008
 *******************************************************************************/
int tmGISLoadingDataThread::ReadLayerDraw ()
{
	/*
	// iterate throught all layers
	int iRank = 0;
	int iReaded = 0;
	bool bInteruptedThread = false;
	
	tmLayerProperties * pLayerProp = NULL;
	tmRealRect myExtent (0,0,0,0);
	
	// Init new thread
	m_DB->DataBaseThreadInit();
	
	while (1)
	{
		if (iRank == 0)
		{
			pLayerProp = m_TOC->IterateLayers(TRUE);
		}
		else
		{
			pLayerProp = m_TOC->IterateLayers(FALSE);
		}
		
		if (!pLayerProp)
			break;
		
		
		if (TestDestroy())
		{
			bInteruptedThread = true;
			break;			
		}
		
		// loading data
		tmGISData * layerData = tmGISData::LoadLayer(pLayerProp);
		
		// processing and deleting data
		if (layerData && pLayerProp->m_LayerVisible)
		{
			// draw layer data
			m_Drawer->Draw(pLayerProp, layerData);
			iReaded ++;
			delete layerData;
		}
		iRank ++;
	}
	
	// uninit thread variables
	m_DB->DataBaseThreadEnd();
	
	// if thread need to be stopped, then 
	// return -1.
	if (bInteruptedThread)
		iReaded = -1;
	
	return iReaded;*/
	return -1;
}






/***************************************************************************//**
 @brief Create a tempory bitmap for drawing in threads
 @param width width of the bitmap to create
 @param height height of the bitmap to create
 @return  true if bitmap was created succesfully
 @author Lucien Schreiber (c) CREALP 2008
 @date 11 October 2008
 *******************************************************************************/
bool tmGISLoadingDataThread::CreateEmptyBitmap (int width, int height)
{
	// I don't wont to share my data...
	wxMutexLocker lock (s_SharedDataCritical);
	if(!lock.IsOk())
		return false;

	
	if (*m_ThreadBmp)
		delete *m_ThreadBmp;
	
	*m_ThreadBmp = new wxBitmap (width, height);
	if ((*m_ThreadBmp)->IsOk())
	{
		wxMemoryDC dc;
		dc.SelectObject(**m_ThreadBmp);
		dc.SetBackground(wxBrush(*wxWHITE_BRUSH));
		dc.Clear();
		dc.SelectObject(wxNullBitmap);
		return true;
	}
	return false;
}



bool tmGISLoadingDataThread::TestDestroyThread()
{
	/*wxMutexLocker lock (s_SharedDataCritical);
	if (*m_ThreadStatus == tmTHREAD_STOP)
	{	
		return true;
	}*/
	return false;
}
