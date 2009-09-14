/***************************************************************************
								tmgisimport_dlg.h
                    Dialog for importing gis data into project
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
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


#ifndef _TM_GIS_IMPORT_DLG_H_
#define _TM_GIS_IMPORT_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/statline.h>			// static line
#include <wx/filepicker.h>			// file picker
#include "../gis/tmgisimport.h"		// import engine


#define ID_DLGIG_IMPORT_BTN 22400


class tmGISImport_DLG : public wxDialog 
	{
	private:
		wxStaticText* m_Type_Value;
		wxStaticText* m_FeatureCount_Type;
		wxChoice* m_ImportChoice;
		wxButton* m_Btn_Import;
		
		tmGISImport * m_Import;
		
		void CreateCtrls (wxWindow * parent);
		void GI_Enable(bool enable=true);
		void GI_SetInfo (TM_GIS_SPATIAL_TYPES type);
		void GI_SetRedColor (bool red = true);
		void GI_SetChoice (bool clear = false);
		
		// event function
		void OnChangeFile (wxFileDirPickerEvent & event);
		
		DECLARE_EVENT_TABLE();
		
	protected:
		
	public:
		tmGISImport_DLG( wxWindow* parent,
						tmGISImport * import, wxWindowID id = wxID_ANY,
						const wxString& title = _("Import GIS data"),
						const wxPoint& pos = wxDefaultPosition,
						const wxSize& size = wxDefaultSize,
						long style = wxDEFAULT_DIALOG_STYLE );
		~tmGISImport_DLG();
		TOC_GENERIC_NAME GetImportLayer();

	};




#endif
