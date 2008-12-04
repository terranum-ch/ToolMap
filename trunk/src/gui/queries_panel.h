/***************************************************************************
 queries_panel.h
 Deals with the queries panel
 -------------------
 copyright            : (C) 2008 CREALP Lucien Schreiber 
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

#ifndef _TM_QUERIESEDITOR_H_
#define _TM_QUERIESEDITOR_H_


#include "wx/listctrl.h"
#include "wx/tglbtn.h"
#include "wx/statusbr.h"
#include "managed_aui_wnd.h"
#include "wxflatbutton.h"		// for flat button
#include "listgenreport_status.h"	// for list with  status support
#include "../database/database_tm.h"	// for database support
#include "queries_panel_dlg.h"			// for the queries edition dialog
#include "../gis/tmmanagerevent.h"		// for manager event


#define ID_QUERIESEDITOR 10049
#define ID_QUERIES_LIST 10051
#define ID_QUERIES_ADD 10052
#define ID_QUERIES_REMOVE 10064
#define ID_QUERIES_RUN 10236
#define ID_BUTTON 10000
#define ID_STATUSBAR1 10065
#define SYMBOL_QUERIES_PANEL_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_QUERIES_PANEL_TITLE _("Queries Editor")
#define SYMBOL_QUERIES_PANEL_IDNAME ID_QUERIESEDITOR
#define SYMBOL_QUERIES_PANEL_SIZE wxSize(200, 100)
#define SYMBOL_QUERIES_PANEL_POSITION wxDefaultPosition



class QueriesList : public ListGenReportWithDialog
	{
	private:
		DataBaseTM * m_pDB;
		
		virtual void BeforeAdding();
		virtual void AfterAdding (bool bRealyAddItem);
		virtual void BeforeDeleting ();
		virtual void BeforeEditing ();
		virtual void AfterEditing (bool bRealyEdited);
		
	protected:
	public:
		QueriesList (wxWindow * parent,
					 wxWindowID id,
					 wxArrayString * pColsName, 
					 wxArrayInt * pColsSize=NULL,
					 wxSize size = wxDefaultSize);
		~QueriesList();
		
		// setter
		void SetDataBase (DataBaseTM * database) {m_pDB = database;}
		
	};



/***************************************************************************//**
 @brief Display the Queries Panel
 @details Queries are immediatly added or removed from the database.
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 November 2008
 *******************************************************************************/
class Queries_PANEL: public ManagedAuiWnd
{
private:
	wxAuiPaneInfo m_PaneInfo;
	DataBaseTM * m_pDB;
	QueriesList* m_QueriesList;
	wxWindow * m_ParentEvt;
	bool m_IsProjectOpen;
	
	/// Initialises member variables
    void InitMemberValues();
	
	// event function
	void OnAddQueries (wxCommandEvent & event);
	void OnRemoveQueries (wxCommandEvent & event);
	void OnRunQueries (wxCommandEvent & event);
	
	
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    Queries_PANEL(  wxWindow* parent, wxWindowID id, wxAuiManager * auimanager);

    /// Creation
   // bool Create( wxWindow* parent, wxWindowID id, wxAuiManager * auimanager);

    /// Destructor
    ~Queries_PANEL();

  
    /// Creates the controls and sizers
    wxSizer * CreateControls(wxWindow * parent,
							 bool call_fit = true,
							 bool set_sizer = true);
	
	void SetDataBase (DataBaseTM * database); 
	
	bool LoadQueries (DataBaseTM * database);
	void EnableQueriesPanel (bool projectopen = true);
	
	
	

};





#endif


