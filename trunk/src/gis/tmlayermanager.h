/***************************************************************************
								tmlayermanager.h
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
#ifndef _TM_LAYERMANAGER_H_
#define _TM_LAYERMANAGER_H_

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/regex.h>

#include "tmdrawer.h"					// for drawing into bitmaps
#include "tmtocctrl.h"					// class TOC ctrl
#include "../database/database_tm.h"	// class database
#include "tmgisdata.h"					// for GISdata
#include "tmgisdatavectormysql.h"		// for direct access to GIS mysql
#include "tmrenderer.h"					// for GIS rendering
#include "tmscalectrl.h"				// for combo box scale control
#include "tmselecteddatamemory.h"		// for storing selected data into memory
#include "tmmanagerevent.h"				// for shared event with other manager
#include "tmmemoryzoom.h"				// for storing previous zoom in memory.

#define ID_MENU_UNLINK_SPATIAL_DATA 10023

// forward declaration
class tmGISLoadingDataThread;
class tmScaleCtrlCombo;

/***************************************************************************//**
 @brief GIS class for dealing with layers
 @details This is the super class for dealing with GIS. This class is the
 central point dealing with : 
 - Database (#DataBaseTM)
 - TOC controls (#tmTOCCtrl)
 - GIS progress indicator (#tmProgressIndicator)
 - ...
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
class tmLayerManager : public wxEvtHandler
{
private:
    tmTOCCtrl * m_TOCCtrl;
    tmRenderer * m_GISRenderer;
    wxWindow * m_Parent;
    DataBaseTM * m_DB;
    tmGISScale m_Scale;
    tmDrawer m_Drawer;
    tmGISLoadingDataThread * m_Thread;
    wxBitmap * m_Bitmap;
    wxStatusBar * m_StatusBar;
    tmScaleCtrlCombo * m_ScaleCtrl;
    static bool	m_LogOn;
    tmSelectedDataMemory m_SelectedData;
    bool m_BlockRefresh;
    tmMemoryZoomManager m_ZoomManager;
    
    // shared member data with thread
    //tmTHREAD_STATUS m_Shared_ThreadStatus;
    wxBitmap * m_ThreadBitmap;
    
    // values for thread
    bool m_computeFullExtent;
    
    // init member values to default
    void InitMemberValue();
    
    // TOC specific functions
    void FillTOCArray();
    bool SaveTOCStatus();
    bool IsOK ();
    
    // rotation warning function
    wxArrayString	m_RotationName;
    wxArrayShort	m_RotationStatus;
    wxArrayString	m_RotationLayerNames;
    
    // function called when view is updated
    void _ZoomChanged();
    bool _ReplaceLayer(const wxFileName & filename, const wxString & originalname);
    void _BuildOverviewsIfNeeded(tmGISData * layer, const wxString & displayname);
    
    // event functions
    void OnZoomRectangleIn (wxCommandEvent & event);
    void OnZoomRectangleOut (wxCommandEvent & event);
    void OnZoomToFeature (wxCommandEvent & event);
    void OnMoveToFeature (wxCommandEvent & event);
    void OnPanFinished (wxCommandEvent & event);
    void OnScrolled (wxCommandEvent & event);
    void OnSelection (wxCommandEvent & event);
    void OnRemoveLayers(wxCommandEvent & event);
    void OnRotationWarning (wxCommandEvent & event);
    
    // bitmap specific functions
    void CreateEmptyBitmap (const wxSize & size);
    
    // Reading all layers for extent or drawing
    int ReadLayerExtent(bool loginfo = false, bool buildpyramids=false);
    int ReadLayerDraw ();
    DECLARE_EVENT_TABLE()
    
public:
    // ctor / dtor
    tmLayerManager(wxWindow * parent, tmTOCCtrl * tocctrl, tmRenderer * renderer,
                   wxStatusBar * status, tmScaleCtrlCombo * scalectrl);
    ~tmLayerManager();
    
    // scale ctrl functions
    void InitScaleCtrlList ();
    
    // Init layermanager
    bool InitLayerManager(DataBaseTM * db);
    bool UnInitLayerManager();
    
    
    // layers operations
    void RemoveLayer (wxCommandEvent & event);
    void AddLayer (wxCommandEvent & event);
    bool OpenLayer(const wxFileName & filename, bool replace=false, const wxString & originalname = wxEmptyString);
    tmGISScale * GetScale (){return &m_Scale;}
    bool ZoomToLayer(long layerid);
    void ZoomToSelectedLayer();
    void ZoomToFrameLayer();
    
    // event operations
    void OnSizeChange (wxCommandEvent & event);
    void OnUpdateCoordinates (wxCommandEvent &event);
    void OnUpdateAngle(wxCommandEvent & event);
    void OnShowLayer (wxCommandEvent & event);
    void OnScaleChanged (wxCommandEvent & event);
    void OnDisplayProperties(wxCommandEvent & event);
    
    // for logging (only one thread may acces the GUI.
    static void EnableLogging (bool enable = true) {m_LogOn = enable;}
    static bool IsLoggingEnabled () {return m_LogOn;}
    
    // tool operations
    void OnZoomToFit ();
    void OnZoomRectangle();
    void OnPan ();
    void OnSelect();
    bool ZoomPrevious();
    bool HasZoomPrevious();
    
    // selection function
    bool SelectedSearch (const wxRect & rect, bool shiftdown = false);
    bool SelectedClear ();
    bool SelectedInvert ();
    bool SelectByOid ();
    
    void CheckGeometryValidity();
    void ExportSelectedGeometries(const wxFileName & file);
    
    bool LoadProjectLayers();
    bool ReloadProjectLayersThreadStart(bool bFullExtent = TRUE, bool bInvalidateFullExt = TRUE);
    
    tmSelectedDataMemory * GetSelectedDataMemory (){return &m_SelectedData;}
    
    // blocking, unblocking the refresh process
    void BlockRefreshProcess(bool block = true){m_BlockRefresh = block;}
    bool GetRefreshBlocked () {return m_BlockRefresh;}
    
    void SetSelectionColour(const wxColour & col){m_SelectedData.SetSelectionColour(col);}
    void SetSelectionHalo (bool hashalo) {m_SelectedData.SetSelectionHalo(hashalo);}
};

#endif
