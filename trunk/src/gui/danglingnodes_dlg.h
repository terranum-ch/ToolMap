/***************************************************************************
							danglingnodes_dlg.h
						Display Dangling nodes Dialog
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


#ifndef _DANGLING_NODES_DLG_H_
#define _DANGLING_NODES_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


const int ID_DLGDN_CLEAR = 22210;

class DanglingNode_DLG : public wxDialog 
	{
	private:
		wxChoice* m_LayerChoice;
		void CreateControls();

		int m_SelectedLayer;

	protected:
		
		
	public:
		DanglingNode_DLG( wxWindow* parent, wxWindowID id = wxID_ANY,
						 const wxString& title = _("Dangling nodes"),
						 const wxPoint& pos = wxDefaultPosition,
						 const wxSize& size = wxDefaultSize,
						 long style = wxDEFAULT_DIALOG_STYLE); //|wxRESIZE_BORDER );
		~DanglingNode_DLG();
		
		void InitDialog (const wxArrayString & layers, int selected = wxNOT_FOUND);
		virtual bool TransferDataFromWindow();
		int GetSelectedLayer(){return m_SelectedLayer;}	
		
		
	};






#endif
