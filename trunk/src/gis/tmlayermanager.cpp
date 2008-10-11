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


DEFINE_EVENT_TYPE(tmEVT_THREAD_GISDATALOADED)

BEGIN_EVENT_TABLE(tmLayerManager, wxEvtHandler)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_REMOVE,tmLayerManager::RemoveLayer)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_ADD,  tmLayerManager::AddLayer)
	EVT_COMMAND(wxID_ANY,tmEVT_LM_SIZE_CHANGED,   tmLayerManager::OnSizeChange)
	EVT_COMMAND(wxID_ANY,tmEVT_LM_MOUSE_MOVED, tmLayerManager::OnUpdateCoordinates)
	EVT_COMMAND(wxID_ANY, tmEVT_THREAD_GISDATALOADED, tmLayerManager::OnReloadProjectLayersDone)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_SHOW_HIDE, tmLayerManager::OnShowLayer)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_ZOOM_RECTANGLE_IN, tmLayerManager::OnZoomRectangleIn)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_ZOOM_RECTANGLE_OUT, tmLayerManager::OnZoomRectangleOut)
	EVT_COMMAND(wxID_ANY, tmEVT_SCALE_USER_CHANGED,tmLayerManager::OnScaleChanged)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_PAN_ENDED, tmLayerManager::OnPanFinished)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_SCROLL_MOVED, tmLayerManager::OnScrolled)
END_EVENT_TABLE()



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
	m_Parent->PopEventHandler(FALSE);
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
	
	
	// 2) Init TOCCtrl
	m_TOCCtrl->InsertProjectName(m_DB->DataBaseGetName());
	FillTOCArray();
	
	// 3) Init scale from database
	InitScaleCtrlList();
	
	
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
	if(m_Bitmap)
	{
		wxLogDebug(_T("deleting bitmap"));
		delete m_Bitmap;
		m_Bitmap = NULL;
	}
	
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
	
	wxLogDebug(_T("%d items added to TOC array"), m_TOCCtrl->GetCountLayers());
}



/***************************************************************************//**
 @brief Save TOC status to database
 @details This function called when closing project saves the status of the TOC
 ctrl. Only visibility and rank of layers are saved, other informations are
 saved in real time when adding a new layer or removing one.
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
	if (!m_DB->DataBaseQueryNoResult(sSentence))
	{
		wxLogDebug(_T("Error updating DB with TOC status : %s"),sSentence.c_str());
		return FALSE;
	}
	
	return TRUE;
	
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
	if (!IsOK())
		return;

	
	wxFileDialog * m_dlg = new wxFileDialog(m_Parent, _("Add GIS layer to the project"),
											_T(""), _T(""),
											tmGISData::GetAllSupportedGISFormatsWildcards());
	if(m_dlg->ShowModal() == wxID_CANCEL)
	{
		delete m_dlg;
		return;
	}
	
	wxFileName myFilename (m_dlg->GetPath());
	tmLayerProperties * item = new tmLayerProperties();
	item->InitFromPathAndName(myFilename.GetPath(),
							  myFilename.GetFullName(),
							  tmGISData::GetAllSupportedGISFormatsExtensions());
	delete m_dlg;
	
	// try to open the file for getting the spatial type
	tmGISData * myLayer = LoadLayer(item);
	if (!myLayer)
	{
		wxLogDebug(_T("Not able to open the layer : %s"), item->GetDisplayName().c_str());
		return;
	}
	item->m_LayerSpatialType = myLayer->GetSpatialType();
	delete myLayer;
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



void tmLayerManager::OnSizeChange (wxCommandEvent & event)
{
	// pass size to scale object but don't make 
	// any computation if no project are opened
	wxSize * mySize = (wxSize *) event.GetClientData();
	m_Scale.SetWindowExtent(wxRect(0,0,mySize->GetWidth(), mySize->GetHeight()));
	
	// compute reel size in MM
	wxClientDC dc (m_GISRenderer);
	m_Scale.SetWindowExtentMM(dc.GetSizeMM());
	delete mySize;
	
	// ensure that a project is opened
	if (!IsOK())
		return;
	
	// compute new pixel size (display area remain the same)
	m_Scale.ComputeNewPixelSize();
	 
	ReloadProjectLayersThreadStart(FALSE, FALSE);
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
	if (!m_StatusBar)
		return;
	
	// ensure that a project is opened
	if (!IsOK())
		return;
	
	if (!m_Scale.IsLayerExtentValid())
	{
		m_StatusBar->SetStatusText(_T(""), 1);
		return;
	}
	
	wxPoint * mousepoint = (wxPoint*) event.GetClientData();
	wxRealPoint mouserealpoint = m_Scale.PixelToReal(*mousepoint);
	delete mousepoint;
	
	wxString sCoord = wxString::Format(_T("x: %.*f -- y: %.*f"),
									   4, mouserealpoint.x, 4, mouserealpoint.y);
	m_StatusBar->SetStatusText(sCoord, 1);
}



void tmLayerManager::OnShowLayer (wxCommandEvent & event)
{
	ReloadProjectLayersThreadStart(FALSE);
}



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



void tmLayerManager::OnZoomRectangleIn (wxCommandEvent & event)
{
	// computing best ratio between actuall windows size 
	// and selection size
	wxRect * mySelectedRect = (wxRect *) event.GetClientData();
	double dSmaller = m_Scale.GetBestDivFactor(*mySelectedRect);
	
	// calcul the new reduced window size (px)
	wxRect myActualExtent = m_Scale.GetWindowExtent();
	wxRect myNewWndExtent (0,0,0,0);
	double dwidth = ((double) myActualExtent.GetWidth()) / dSmaller;
	double dheight = ((double) myActualExtent.GetHeight()) / dSmaller;
	
	myNewWndExtent.SetWidth((int) dwidth);
	myNewWndExtent.SetHeight((int) dheight);
	
	// modify the real window extent
	m_Scale.ComputeNewRealZoomExtent(myNewWndExtent, 
								 wxPoint(mySelectedRect->GetX(),
										 mySelectedRect->GetY()));
	
	// reload data
	ReloadProjectLayersThreadStart(FALSE);
	delete mySelectedRect;
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
	
	myNewWndExtent.SetWidth((int) dwidth);
	myNewWndExtent.SetHeight((int) dheight);
	
	// modify the real window extent
	m_Scale.ComputeNewRealZoomExtent(myNewWndExtent, 
								 wxPoint(-mySelectedRect->GetX(),
										 -mySelectedRect->GetY()));
	
	// reload data
	ReloadProjectLayersThreadStart(FALSE);
	delete mySelectedRect;
}



void tmLayerManager::OnPanFinished (wxCommandEvent & event)
{
	wxPoint * myNewPosPx = (wxPoint*) event.GetClientData();
	
	// change coordinates
	m_Scale.ComputeNewRealPanExtent(*myNewPosPx);
	ReloadProjectLayersThreadStart(FALSE);
	
	
	delete myNewPosPx;
}



void tmLayerManager::OnScrolled (wxCommandEvent & event)
{
	wxScrollWinEvent * myScrollEvent = (wxScrollWinEvent*) event.GetClientData();
	m_Scale.ComputeScrollMoveReal(myScrollEvent->GetOrientation(),
								  myScrollEvent->GetPosition());
	delete myScrollEvent;
	ReloadProjectLayersThreadStart(FALSE, FALSE);
	
}



void tmLayerManager::UpdateScrollBars ()
{
	wxSize myVirtualLayerSize = m_Scale.GetVirtualPxSize();
	m_GISRenderer->SetVirtualSize(myVirtualLayerSize);
	
	wxSize myWndSize (m_Scale.GetWindowExtent().GetWidth(),
					  m_Scale.GetWindowExtent().GetHeight());
		
	wxSize myDiffSize = myVirtualLayerSize - myWndSize;
	int xscrollrate = myDiffSize.GetWidth() / tmSCROLLBARS_DIV;
	int yscrollrate = myDiffSize.GetHeight() / tmSCROLLBARS_DIV;
	
	m_GISRenderer->SetScrollRate(xscrollrate, yscrollrate);
	
	
	// avoid div by 0
	if (xscrollrate <= 0)
		xscrollrate = 1;
		
	if(yscrollrate <= 0)
		yscrollrate = 1;
		
	
	wxPoint myVirtPos = m_Scale.GetVirtualPxPosition();
	myVirtPos.x = myVirtPos.x / xscrollrate;
	myVirtPos.y = myVirtPos.y / yscrollrate;
	
	m_GISRenderer->SetScrollPos(wxHORIZONTAL, myVirtPos.x);
	m_GISRenderer->SetScrollPos(wxVERTICAL, myVirtPos.y);
	
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
	
	
	//TODO: iterate for drawing layers here
	ReadLayerDraw();
	
	
	// update scale
	m_ScaleCtrl->SetValueScale(m_Scale.GetActualScale());
	
	m_Drawer.DrawExtentIntoBitmap(2,*wxRED);
	
	// set active bitmap	
	m_GISRenderer->SetBitmapStatus(m_Bitmap);
	m_GISRenderer->Refresh();
	return TRUE;
}



bool tmLayerManager::ReloadProjectLayersThreadStart(bool bFullExtent, bool bInvalidateFullExt)
{
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
	s_SharedDataCritical.Enter();
	if (m_Shared_ThreadStatus == tmTHREAD_RUN)
	{
		m_Shared_ThreadStatus = tmTHREAD_STOP;
		s_SharedDataCritical.Leave();
		m_Thread->Delete();
	}
	else
		s_SharedDataCritical.Leave();
	

	// display a progress thread
	// only if nothing is displayed
	if (!m_Progress)
	{
		m_Progress = new tmProgressIndicator(m_Parent, m_StatusBar);
		m_Progress->SetMessage(TMPROGRESS_DRAW_DATA);
		m_Progress->DisplayProgress();
	}
	
		
	m_Thread = new tmGISLoadingDataThread(m_Parent, m_TOCCtrl, &m_Scale, m_DB, 
										  &m_Drawer, &m_Shared_ThreadStatus,
										  m_ThreadBitmap);
	if (m_Thread->Create() != wxTHREAD_NO_ERROR)
	{
		wxLogError(_T("Can't create thread for GIS data loading"));
		return FALSE;
	}
	wxLogDebug(_T("Gis thread started..."));
	m_Thread->Run();
	return TRUE;
}	



void tmLayerManager::OnReloadProjectLayersDone (wxCommandEvent & event)
{
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
	if (m_Scale.IsLayerExtentValid())
	{
		// compute max extent if required by option
		if (m_computeFullExtent)
			m_Scale.ComputeMaxExtent();
		
		//TODO: Here check validity and then copy m_ThreadBitmap into m_Bitmap
		wxCriticalSectionLocker lock (s_SharedDataCritical);
		if (m_ThreadBitmap->IsOk())
		{
			wxMemoryDC dc;
			dc.SelectObject(*m_Bitmap);
			dc.DrawBitmap(*m_ThreadBitmap, 0, 0, TRUE);
			dc.SelectObject(wxNullBitmap);
		}
		else
		{
			wxLogDebug(_T("Error with bitmap."));
		}
			
		
		//m_Drawer.InitDrawer(m_Bitmap, m_Scale, tmRealRect(0,0,0,0));
		//m_Drawer.DrawExtentIntoBitmap();//m_Bitmap, m_Scale);
	}
		
	// update scale
	m_ScaleCtrl->SetValueScale(m_Scale.GetActualScale());
	
	// update scrollbars
	UpdateScrollBars();
	
	
	// set active bitmap	
	m_GISRenderer->SetBitmapStatus(m_Bitmap);
	m_GISRenderer->Refresh();
	return;
	
}



/***************************************************************************//**
 @brief Load a layer
 @details This function load a layer based on his properties (see
 #tmLayerProperties) and return a pointer to a tmGISData object.
 @param layerProp properties of layer to load (#tmLayerProperties)
 @return  a valid pointer to a tmGISData or NULL if something goes wrong
 @author Lucien Schreiber (c) CREALP 2008
 @date 21 July 2008
 *******************************************************************************/
tmGISData * tmLayerManager::LoadLayer (tmLayerProperties * layerProp)
{
	wxASSERT(layerProp);
	tmGISData * m_Data = NULL;
	wxString myFileName = _T("");
	wxString myErrMsg = _T("");
	// only used if not generic layers
	wxFileName layerfullname (layerProp->m_LayerPathOnly, layerProp->m_LayerNameExt);
	
	switch (layerProp->m_LayerType)
	{
		case TOC_NAME_LINES:
		case TOC_NAME_POINTS:
		case TOC_NAME_ANNOTATIONS:
		case TOC_NAME_LABELS:
		case TOC_NAME_FRAME:
			m_Data = tmGISData::CreateGISBasedOnType(tmGIS_VECTOR_MYSQL);
			myFileName = TABLE_NAME_GIS_GENERIC[layerProp->m_LayerType];
			myErrMsg = layerProp->m_LayerNameExt;
			break;
	
		case TOC_NAME_TIFF:
		case TOC_NAME_EGRID:
		case TOC_NAME_SHP:
			m_Data = tmGISData::CreateGISBasedOnExt(layerProp->GetFileExtension());
			myFileName = layerfullname.GetFullPath();
			myErrMsg = myFileName;
			break;
			
		default:
			fprintf(stderr, "%s line %d : %s file format not supported yet \n ", __FUNCTION__, __LINE__,
					layerProp->m_LayerNameExt.c_str());
			return NULL;
			break;
	}
	
	// here load data
	if (!m_Data)
	{
		wxLogError(_("Error loading : %s"), myErrMsg.c_str());
		return NULL;
	}
	
	if (!m_Data->Open(myFileName, TRUE))
	{
		wxLogError(_("Error opening : %s"), myErrMsg.c_str());
		return NULL;
	}
		
	return m_Data;
}



/***************************************************************************//**
 @brief Creating an empty white bitmap
 @param size the size (in pixels) of the new bitmap to create
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
void tmLayerManager::CreateEmptyBitmap (const wxSize & size)
{
	if (m_Bitmap)
	{
		wxLogDebug(_T("deleting bitmap"));
		delete m_Bitmap;
		m_Bitmap = NULL;
	}
	
	m_Bitmap = new wxBitmap(size.GetWidth(), size.GetHeight());
				   wxLogDebug(_T("Creating bitmap"));

	// ensure that the bitmap is white (win and linux)
	wxMemoryDC dc;
	dc.SelectObject (*m_Bitmap);
	dc.SetBackground(wxBrush(*wxWHITE_BRUSH));
	dc.Clear();

	//dc.SetBrush (wxBrush(*wxWHITE_BRUSH));
	//dc.SetPen (wxPen(*wxWHITE_PEN));
	//dc.DrawRectangle (0,0,size.GetWidth(), size.GetHeight());
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
		
		// loading data
		tmGISData * layerData = LoadLayer(pLayerProp);
		
		// processing and deleting data
		if (layerData && pLayerProp->m_LayerVisible)
		{
			myExtent = layerData->GetMinimalBoundingRectangle();
			m_Scale.SetMaxLayersExtentAsExisting(myExtent);
			iReaded ++;
			
			// show some logging info, not working
			// in thread mode
			if (loginfo)
			{
				wxLogDebug(_T("Minimum rectangle is : %.*f - %.*f, %.*f - %.*f"),
						   2,myExtent.x_min, 2, myExtent.y_min,
						   2, myExtent.x_max, 2, myExtent.y_max);
			}
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
		
		// loading data
		tmGISData * layerData = LoadLayer(pLayerProp);
		
		// processing and deleting data
		if (layerData && pLayerProp->m_LayerVisible)
		{
			// draw layer data
			m_Drawer.Draw(pLayerProp, layerData);
			iReaded ++;
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
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
tmGISLoadingDataThread::tmGISLoadingDataThread(wxWindow * parent, tmTOCCtrl * toc,
											   tmGISScale * scale,
											   DataBaseTM * database,
											   tmDrawer * drawer,
											   tmTHREAD_STATUS * threadstatus,
											   wxBitmap * threadbitmap)
{
	m_Parent = parent;
	m_TOC = toc;
	m_Scale = scale;
	m_DB = database;
	m_Stop = FALSE;
	m_Drawer = drawer;
	m_ThreadStatus = threadstatus;
	m_ThreadBmp = threadbitmap;
	
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
	s_SharedDataCritical.Enter();
	* m_ThreadStatus = tmTHREAD_RUN;
	s_SharedDataCritical.Leave();
	
	// process loading of GIS data
	
	// if the thread was stopped
	//if (ReadLayerExtentThread()==false)	
	//	return NULL;
	
	if (!CreateEmptyBitmap(m_Scale->GetWindowExtent().GetWidth(),
						  m_Scale->GetWindowExtent().GetHeight()))
		return NULL;
	
	
	// read layers extent
	int iNbLayers = ReadLayerExtentThread();
	if (iNbLayers == -1)
		return NULL;
	
	// read layers for drawing
	m_Drawer->InitDrawer(m_ThreadBmp, *m_Scale, m_Scale->GetWindowExtentReal(),true);
	int iNbLayersDraw = ReadLayerDraw();
	if (iNbLayersDraw == -1)
		return NULL;
	
	
	for (int i = 0;i<10; i++)
	{
		if (TestDestroy())
			return NULL;
		
		 wxThread::Sleep(200);
	}
	
	
	// if thread finished correctly, send a message to
	// the layer manager to display the new image
	wxCriticalSectionLocker lock (s_SharedDataCritical);
	* m_ThreadStatus = tmTHREAD_STOP;

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
	// iterate throught all layers
	int iRank = 0;
	int iReaded = 0;
	bool bInteruptedThread = false;
	
	tmLayerProperties * pLayerProp = NULL;
	tmRealRect myExtent (0,0,0,0);
	
	// Init new thread
	m_DB->DataBaseNewThreadInit();

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
		tmGISData * layerData = tmLayerManager::LoadLayer(pLayerProp);
		
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
	m_DB->DataBaseNewThreadUnInit();
	
	// if thread need to be stopped, then 
	// return -1.
	if (bInteruptedThread)
		iReaded = -1;
	
	return iReaded;	
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
	// iterate throught all layers
	int iRank = 0;
	int iReaded = 0;
	bool bInteruptedThread = false;
	
	tmLayerProperties * pLayerProp = NULL;
	tmRealRect myExtent (0,0,0,0);
	
	// Init new thread
	m_DB->DataBaseNewThreadInit();
	
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
		tmGISData * layerData = tmLayerManager::LoadLayer(pLayerProp);
		
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
	m_DB->DataBaseNewThreadUnInit();
	
	// if thread need to be stopped, then 
	// return -1.
	if (bInteruptedThread)
		iReaded = -1;
	
	return iReaded;
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
	wxCriticalSectionLocker lock (s_SharedDataCritical);
	
	if (m_ThreadBmp)
		delete m_ThreadBmp;
	
	m_ThreadBmp = new wxBitmap (width, height);
	if (m_ThreadBmp->IsOk())
		return TRUE;
	return FALSE;
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


