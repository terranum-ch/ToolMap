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

#include "managed_aui_wnd.h"
#include "wxflatbutton.h"				// for flat button
#include "../database/database_tm.h"	// for database support
#include "listgenreport.h"				// listgenreport support


#define ID_SNAP_TOLERENCE_TXT 22000
#define ID_SNAP_LIST 22001
#define ID_SNAP_ADD 22002
#define ID_SNAP_REMOVE 22003
#define ID_SNAP_CLEAR 22004
#define SNAPPING_PANEL_TITLE _("Snapping")
#define SNAPPING_PANEL_SIZE wxSize(300, 200)



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
		
		virtual void BeforeAdding(){;}
		virtual void AfterAdding (bool bRealyAddItem){;}
		virtual void BeforeDeleting (){;}
		virtual void BeforeEditing (){;}
		virtual void AfterEditing (bool bRealyEdited){;}
		
	protected:
	public:
		SnappingList (wxWindow * parent,
					 wxWindowID id,
					 wxArrayString * pColsName, 
					 wxArrayInt * pColsSize=NULL,
					 wxSize size = wxDefaultSize);
		~SnappingList();
		
		// setter
		void SetDataBase (DataBaseTM * database) {m_pDB = database;}
		
	};




/***************************************************************************//**
 @brief The panel used for setting the snapping
 @author Lucien Schreiber (c) CREALP 2009
 @date 19 January 2009
 *******************************************************************************/
class Snapping_PANEL : public ManagedAuiWnd  
	{
	private:
		wxTextCtrl* m_Tolerence;
		SnappingList * m_SnappingList;
		wxWindow * m_ParentEvt;
		wxAuiPaneInfo m_PaneInfo;
		
		// init private functions
		void InitMembersValue();
		wxSizer * CreateControls(wxWindow * parent,
								 bool call_fit = true,
								 bool set_sizer = true);
		
		
		//  event handlers
		virtual void OnUpdateTolerence( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAddSnapping( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRemoveSnapping( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnClearSnapping( wxCommandEvent& event ){ event.Skip(); }
		DECLARE_EVENT_TABLE()
		
	public:
		Snapping_PANEL( wxWindow* parent, wxWindowID id, 
					   wxAuiManager * auimanager);
		~Snapping_PANEL();
		
	};







#endif
