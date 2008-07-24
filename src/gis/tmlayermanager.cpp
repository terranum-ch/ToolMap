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
END_EVENT_TABLE()



/***************************************************************************//**
 @brief Constructor
 @details Init values to default and init members.
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
tmLayerManager::tmLayerManager(wxWindow * parent, tmTOCCtrl * tocctrl, 
							   tmRenderer * renderer,  wxStatusBar * status)
{
	InitMemberValue();
	
	m_TOCCtrl = tocctrl;
	m_GISRenderer = renderer;
	m_Parent = parent;
	m_StatusBar = status;
	m_Parent->PushEventHandler(this);
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
	
	// 3) Load Data (not threaded)
	if(!LoadProjectLayers())
	{
		wxLogDebug(_T("Loading layers not completed succesfully"));
		return FALSE;
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
	int iRank = 0;
	
	wxString sSentence = _T("");
	
	while (1)
	{
		if (iRank == 0)
		{
			itemProp = m_TOCCtrl->IterateLayers(TRUE);
		}
		else
		{
			itemProp = m_TOCCtrl->IterateLayers(FALSE);
		}
		
		if (!itemProp)
			break;
		
		m_DB->PrepareTOCStatusUpdate(sSentence, itemProp, iRank);
		iRank ++;
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
		
	//if (!m_TOCCtrl->IsTOCReady())
	//	return;
	
	
	
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
	item->m_LayerNameExt = myFilename.GetFullName();
	item->m_LayerPathOnly = myFilename.GetPath();
	delete m_dlg;
	
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
		
	// create new bitmap based on size

	
	// ensure that a project is opened
	//if (!m_TOCCtrl->IsTOCReady())
	//	return;
	if (!IsOK())
		return;
	
	 
	
	// TODO: Do computation and reload project in a thread
	
	wxLogDebug(_("Size message received : %d, %d"), mySize->GetWidth(), mySize->GetHeight());
	
	delete mySize;
	
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
	//if (!m_TOCCtrl->IsTOCReady())
	//	return;
	
	wxPoint * mousepoint = (wxPoint*) event.GetClientData();
	wxRealPoint mouserealpoint = m_Scale.PixelToReal(*mousepoint);
	delete mousepoint;
	
	wxString sCoord = wxString::Format(_T("x: %.*f -- y: %.*f"),
									   4, mouserealpoint.x, 4, mouserealpoint.y);
	m_StatusBar->SetStatusText(sCoord, 1);
}



void tmLayerManager::OnZoomToFit ()
{
	
	ReloadProjectLayersThreadStart();
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
		wxLogDebug(_T("No data loaded into the TOC ctrl, load data into the TOC first"));
		return FALSE;
	}
	return TRUE;
}



bool tmLayerManager::LoadProjectLayers()
{
	if (!IsOK())
		return FALSE; 
	
	// invalidate bitmap
	m_GISRenderer->SetBitmapStatus();
	CreateEmptyBitmap(wxSize (m_Scale.GetWindowExtent().GetWidth(),
							  m_Scale.GetWindowExtent().GetHeight()));

		
	// iterate throught all layers
	int iRank = 0;
	tmLayerProperties * itemProp = NULL;
	tmRealRect myExtent (0,0,0,0);
	
	
	// prepare loading of MySQL data
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_DB);
	while (1)
	{
		if (iRank == 0)
		{
			itemProp = m_TOCCtrl->IterateLayers(TRUE);
		}
		else
		{
			itemProp = m_TOCCtrl->IterateLayers(FALSE);
		}
		
		if (!itemProp)
			break;
		
		// loading data
		tmGISData * layerData = LoadLayer(itemProp);
		
		// processing and deleting data
		if (layerData && itemProp->m_LayerVisible)
		{
			//wxFileName myfilename (itemProp->m_LayerPathOnly, itemProp->m_LayerNameExt);
			//wxLogDebug(myfilename.GetFullPath() + _T(" - Opened"));
			
			// computing extend 
			
			// TODO: Remove this later, this is temp code
			myExtent = layerData->GetMinimalBoundingRectangle();
			wxLogDebug(_T("Minimum rectangle is : %.*f - %.*f, %.*f - %.*f"),
					   2,myExtent.x_min, 2, myExtent.y_min,
					   2, myExtent.x_max, 2, myExtent.y_max);
			//
			
			m_Scale.SetMaxLayersExtentAsExisting(myExtent);
			delete layerData;
		}
		iRank ++;
	}
	
	//tmRealRect r = m_Scale.GetMaxLayersExtent();
	
	//wxLogDebug(_T("Max visible extent is : %.*f, %.*f -- %.*f, %.*f"),
	//		   2, r.x_min, 2, r.y_min, 2, r.x_max, 2, r.y_max );
	
	//wxLogDebug(_T("Computed factor is : %.*f"), 3,
	//		   m_Scale.ComputeDivFactor());
	
	
	// draw into bitmap
	m_Scale.ComputeMaxExtent();
	

	m_Drawer.DrawExtentIntoBitmap(m_Bitmap, m_Scale);
	
	// set active bitmap	
	m_GISRenderer->SetBitmapStatus(m_Bitmap);
	m_GISRenderer->Refresh();
	return TRUE;
}



bool tmLayerManager::ReloadProjectLayersThreadStart()
{
	// invalidate bitmap
	m_GISRenderer->SetBitmapStatus();
	CreateEmptyBitmap(wxSize (m_Scale.GetWindowExtent().GetWidth(),
							  m_Scale.GetWindowExtent().GetHeight()));
	
		
	// start a thread if not existing,
	// stop the existing otherwise.
	if (m_Thread != NULL)
	{
		m_Thread->StopThread();
		m_Thread = NULL;
	}
	
		
	m_Thread = new tmGISLoadingDataThread(m_Parent, m_TOCCtrl, &m_Scale, m_DB);
	if (m_Thread->Create() != wxTHREAD_NO_ERROR)
	{
		wxLogError(_T("Can't create thread for GIS data loading"));
		return FALSE;
	}
	m_Thread->Run();
	
	

	return TRUE;
}	



void tmLayerManager::OnReloadProjectLayersDone (wxCommandEvent & event)
{
	wxLogDebug(_T("GIS thread finished"));
	m_Thread = NULL; // thread finished
	
	// draw into bitmap
	//m_Scale.ComputeMaxExtent();
	
	
	//m_Drawer.DrawExtentIntoBitmap(m_Bitmap, m_Scale);
	
	// set active bitmap	
	//m_GISRenderer->SetBitmapStatus(m_Bitmap);
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
	
	switch (layerProp->m_LayerIsGeneric)
	{
		case TOC_NAME_LINES:
		case TOC_NAME_POINTS:
		case TOC_NAME_ANNOTATIONS:
		case TOC_NAME_LABELS:
		case TOC_NAME_FRAME:
			m_Data = tmGISData::CreateGISBasedOnType(tmGIS_VECTOR_MYSQL);
			myFileName = TABLE_NAME_GIS_GENERIC[layerProp->m_LayerIsGeneric];
			myErrMsg = layerProp->m_LayerNameExt;
			break;
	
		case TOC_NAME_NOT_GENERIC:
			m_Data = tmGISData::CreateGISBasedOnExt(layerProp->GetFileExtension());
			myFileName = layerfullname.GetFullPath();
			myErrMsg = myFileName;
			break;
			
		default:
			wxLogDebug(_T("%s file format not supported yet"),layerProp->m_LayerNameExt.c_str() );
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
	dc.SetBrush (wxBrush(*wxWHITE_BRUSH));
	dc.SetPen (wxPen(*wxWHITE_PEN));
	dc.DrawRectangle (0,0,size.GetWidth(), size.GetHeight());
	
}






/****************************** THREAD CLASS FUNCTION BELOW ***********************/

tmGISLoadingDataThread::tmGISLoadingDataThread(wxWindow * parent, tmTOCCtrl * toc,
											   tmGISScale * scale,
											   DataBaseTM * database)
{
	m_Parent = parent;
	m_TOC = toc;
	m_Scale = scale;
	m_DB = database;
	m_Stop = FALSE;
	
	// init new thread support
	m_DB->DataBaseNewThreadInit();
	
}

tmGISLoadingDataThread::~tmGISLoadingDataThread()
{
	m_DB->DataBaseNewThreadUnInit();
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
	// iterate throught all layers
	int iRank = 0;
	tmLayerProperties * itemProp = NULL;
	tmRealRect myExtent (0,0,0,0);
	
	
	// prepare loading of MySQL data
	
	//m_DB->DataBaseIsOpen();
	
	m_DB->DataBaseListTables();
	
	/*
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_DB);
	while (1)
	{
		if (iRank == 0)
		{
			itemProp = m_TOC->IterateLayers(TRUE);
		}
		else
		{
			itemProp = m_TOC->IterateLayers(FALSE);
		}
		
		if (!itemProp)
			break;
		
		// exit thread
		if (m_Stop == TRUE)
		{
			wxLogDebug(_T("GIS thread stoped"));
			return NULL;
		}
		
		
		// loading data
		tmGISData * layerData = tmLayerManager::LoadLayer(itemProp);
		
		// processing and deleting data
		if (layerData && itemProp->m_LayerVisible)
		{
			
			
			
			//wxFileName myfilename (itemProp->m_LayerPathOnly, itemProp->m_LayerNameExt);
			//wxLogDebug(myfilename.GetFullPath() + _T(" - Opened"));
			
			// computing extend 
			
			// TODO: Remove this later, this is temp code
			myExtent = layerData->GetMinimalBoundingRectangle();
			wxLogDebug(_T("Minimum rectangle is : %.*f - %.*f, %.*f - %.*f"),
					   2,myExtent.x_min, 2, myExtent.y_min,
					   2, myExtent.x_max, 2, myExtent.y_max);
			//
			
			m_Scale->SetMaxLayersExtentAsExisting(myExtent);
			delete layerData;
		}
		iRank ++;
		
		// exit thread
		if (m_Stop == TRUE)
		{
			wxLogDebug(_T("GIS thread stoped"));
			return NULL;
		}
	}*/
	
	
	wxCommandEvent evt(tmEVT_THREAD_GISDATALOADED, wxID_ANY);
	//sProgress.Append(TMPROGRESS_INDICATOR_CHAR); 
	//evt.SetString(m_Message + sProgress);
	m_Parent->GetEventHandler()->AddPendingEvent(evt);
	return NULL;
}





