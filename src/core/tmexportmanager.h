/***************************************************************************
								tmexportmanager.h
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


#ifndef _TM_EXPORTMANAGER_H_
#define _TM_EXPORTMANAGER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/wupdlock.h>

#include "../database/database_tm.h"	// for database access
#include "projectdefmemory.h"			// for PojectDefMemoryLayers definition

#include "tmexportdatashp.h"			// for exporting data in SHP
#include <wx/busyinfo.h>				// for busy info dialog, remove if no more needed
#include <wx/progdlg.h>					// the progress dialog


const int tmFILENAME_MAX_SEARCH = 500;	// number of search for a file name
const int ID_EXPORTDLG_ALLBTN = 10375;
const int ID_EXPORTDLG_NONEBTN = 10376;
const int ID_EXPORTDLG_INVERTBTN = 10377;

class tmTOCCtrl;


class tmExportManager : public wxObject
	{
	private:
		DataBaseTM * m_pDB;
		wxWindow * m_Parent;
		PRJDEF_EXPORT_TYPE m_ExportType;
		wxFileName m_ExportPath;
		tmExportData * m_ExportData;

		PrjDefMemManage * m_ProjMem;
		
		// init values
		void InitMemberValues();
		
		// export function
		bool ExportLayers (PrjMemLayersArray * layers);
		bool ExportLayer (ProjectDefMemoryLayers * layer, 
						  wxRealPoint * frame, const int & framevertex);
		bool _CreateExportLayer (ProjectDefMemoryLayers * layer);
		bool _ExportSimple (ProjectDefMemoryLayers * layer);
		bool _ExportPolyGIS (ProjectDefMemoryLayers * layer);
		
		wxRealPoint * GetFrame (int & nbvertex);
		
		// check path & file name
		bool IsExportPathValid();
		bool GetAvailableFileName (ProjectDefMemoryLayers * layer);
		
		// check integrity
		void _CorrectIntegrity(PrjMemLayersArray * layers);
		
		// create tmExportData object
		tmExportData * CreateExportData ();
		
		// progress function and variables
		wxProgressDialog * m_ProgressDlg;
		wxBusyInfo * m_ProgressBusy;
		wxString m_ProgressText;
		void CreateProgress(int iNbLayers, const wxString & layername);
		bool UpdateProgress(int iActualLayer, const wxString & layername);
		void DeleteProgress();
		
	protected:
	public:
		
		// construction and destruction
		tmExportManager();
		~tmExportManager();
		tmExportManager(wxWindow * parent, DataBaseTM * database);
		void Create (wxWindow * parent, DataBaseTM * database);
		
		// Export public function
		bool ExportAll (PrjDefMemManage * localprojdef);
		bool ExportSelected (PrjDefMemManage * localprojdef, tmTOCCtrl * toc);
};



/*************************************************************************************//**
@brief Export selected layer Dialog
@author Lucien Schreiber copyright CREALP
@date 30 aout 2012
 *****************************************************************************************/
class tmExportSelected_DLG : public wxDialog {
private:
    wxCheckListBox* m_ListLayersCtrl;
    wxCheckBox* m_LayersAddCtrl;
    wxCheckBox* m_LayersReplaceCtrl;
    
    void OnBtnAll(wxCommandEvent & event);    
    void OnBtnNone(wxCommandEvent & event);
    void OnBtnInvert(wxCommandEvent & event);
    void OnUpdateUIBtnNone(wxUpdateUIEvent & event);
    
    void _CreateControls(const wxArrayString & layers);
    DECLARE_EVENT_TABLE();
    
public:
    tmExportSelected_DLG(wxWindow * parent, const wxArrayString & layers,  wxWindowID id = wxID_ANY, const wxString & caption = _("Export Layer"), const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    virtual ~tmExportSelected_DLG();
    
    wxArrayInt GetSelectedLayersID();
    bool DoLayerAdd();
    bool DoLayerReplace();
};




#endif
