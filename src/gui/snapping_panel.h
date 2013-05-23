/***************************************************************************
								snapping_panel.h
				Deals with the snapping panel, used for setting the
							snapping for layers
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


#ifndef _SNAPPING_PANEL_H_
#define _SNAPPING_PANEL_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/spinctrl.h>


#include "managed_aui_wnd.h"
#include "wxflatbutton.h"				// for flat button
#include "listgenreport.h"				// listgenreport support
#include "../core/tmsnappingmemory.h"	// store snapping status into memory

class DataBaseTM;
class DataListReportCtrl;

#define ID_SNAP_TOLERENCE_TXT 22000
#define ID_SNAP_LIST 22001
#define ID_SNAP_ADD 22002
#define ID_SNAP_REMOVE 22003
#define ID_SNAP_CLEAR 22004
#define SNAPPING_PANEL_TITLE _("Snapping")
#define SNAPPING_PANEL_SIZE wxSize(230, 150)



/***************************************************************************//**
 @brief The list used for setting the snapping
 @details This list consist in 3 columns, the layer name, and the snapping
 status for vertex or begin end
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
class SnappingList : public ListGenReportWithDialog
{
private:
    DataBaseTM * m_pDB;
    wxArrayLong m_LayersID;
    wxArrayString m_LayersName;
    wxWindow * m_Parent;
    tmSnappingMemory * m_SnappingMemory;
    
    virtual void BeforeAdding();
    virtual void AfterAdding (bool bRealyAddItem);
    virtual void BeforeDeleting ();
    
    // very different editing process, here we don't use
    // a dialog for editing
    virtual void OnDoubleClickItem (wxListEvent & event);
    
protected:
public:
    SnappingList (wxWindow * parent, wxWindowID id, wxArrayString * pColsName,  wxArrayInt * pColsSize=NULL, wxSize size = wxDefaultSize);
    ~SnappingList();
    
    // setter
    void SetDataBase (DataBaseTM * database) {m_pDB = database;}
    tmSnappingMemory * GetSnappingMemoryRef(){return m_SnappingMemory;}
    
    void SetSnappingStatus (int snapStatus, int iRow, bool clearbefore = true);
    int GetSnappingStatus (int iRow);
    
    void ClearSnappingStatus ();
};



class tmRenderer;
/***************************************************************************//**
 @brief The panel used for setting the snapping
 @author Lucien Schreiber (c) CREALP 2009
 @date 19 January 2009
 *******************************************************************************/
class Snapping_PANEL : public ManagedAuiWnd
{
private:
    wxSpinCtrl* m_TolerenceCtrl;
    DataListReportCtrl * m_ListCtrl;
    DataBaseTM * m_pDB;
    tmRenderer * m_Renderer;
    wxWindow * m_ParentEvt;
    wxAuiPaneInfo m_PaneInfo;
    tmSnappingMemory m_SnappingMemory;

    
    wxPanel * _CreateControls();
    
    void OnUpdateTolerence( wxSpinEvent & event );
    void OnContextualMenu (wxContextMenuEvent & event);
    DECLARE_EVENT_TABLE()
    
public:
    Snapping_PANEL( wxWindow* parent, wxWindowID id, wxAuiManager * auimanager);
    ~Snapping_PANEL();
    
    void SetDataBase (DataBaseTM * database){m_pDB = database;}
    void SetRendererRef(tmRenderer * renderer) {m_Renderer = renderer;}
    
    void LoadSnappingStatus ();
    bool SaveSnappingStatus ();
    
    void OnAddSnapping( wxCommandEvent& event );
    void OnRemoveSnapping( wxCommandEvent& event );
    void OnUpdateUIContextualMenuAdd (wxUpdateUIEvent & event);
    void OnUpdateUIContextualMenuRemove (wxUpdateUIEvent & event);

    tmSnappingMemory * GetSnappingMemoryRef() {return &m_SnappingMemory;}
};







#endif
