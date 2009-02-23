/***************************************************************************
								editvertex_dlg.h
                    Dialog for manuel editing vertex position
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


#ifndef _TM_EDIT_VERTEX_DLG_H_
#define _TM_EDIT_VERTEX_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/statline.h>	// for staticline
#include <wx/grid.h>		// for grid definition
#include "wxflatbutton.h"	// for flat buttons. 


#define ID_BTN_ADD_VERTEX 22100
#define ID_BTN_REMOVE_VERTEX 22101
#define ID_BTN_DISPLAYVERTEX 22102
const wxString EditVertexDLGTitle = _("Edit Vertex Position (ID : ");




class EditVertexDLG : public wxDialog 
	{
	private:
		// member controls
		wxGrid* m_VertexGrid;
		wxFlatButton* m_DisplayVertexPosBtn;
		
		// member functions
		void CreateControls();
		
	public:
		// ctor and dtor
		EditVertexDLG( wxWindow* parent, wxWindowID id = wxID_ANY,
					  const wxString& title = EditVertexDLGTitle,
					  const wxPoint& pos = wxDefaultPosition,
					  const wxSize& size = wxDefaultSize,
					  long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~EditVertexDLG();
		
	};




#endif
