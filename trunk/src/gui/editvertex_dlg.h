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

#include <wx/statline.h>			// for staticline
#include <wx/grid.h>				// for grid definition
#include "wxflatbutton.h"			// for flat buttons. 
#include "../core/tmarraysize.h"	// for array of real


#define ID_BTN_ADD_VERTEX 22100
#define ID_BTN_REMOVE_VERTEX 22101
#define ID_BTN_DISPLAYVERTEX 22102

const int tmVERTEX_PRECISION = 10;

// margin for systems
//#ifdef __WXMSW__
const int gSizeMargin = 10;
//#else
//const int gSizeMargin = 0;
//#endif


class EditVertexDLG : public wxDialog 
	{
	private:
		// member controls
		wxGrid* m_VertexGrid;
		wxFlatButton* m_DisplayVertexPosBtn;
		wxStatusBar * m_Status;
		
		// member functions
		void CreateControls();
		
		//status function
		void SetStatusSelectedID (long oid);
		void SetStatusNumberVertex (int ivertexnumber);
		
		// Grid functions
		bool GridInsertLine (int iIndex = -1, wxRealPoint * pt = NULL);
		bool GridClear ();
		int GridGetSelection ();
		
		//event function
		void OnVertexAdd (wxCommandEvent & event);
		void OnVertexRemove (wxCommandEvent & event);
		void OnVertexHighlight (wxCommandEvent & event);
		
		DECLARE_EVENT_TABLE();
		
		
	public:
		// ctor and dtor
		EditVertexDLG( wxWindow* parent, wxWindowID id = wxID_ANY,
					  const wxString& title = _("Edit Vertex Position"),
					  const wxPoint& pos = wxDefaultPosition,
					  const wxSize& size = wxDefaultSize,
					  long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~EditVertexDLG();
		
		// member for data exchange
		wxArrayRealPoints m_VertexPts;
		long m_SelectedOID;
		
		// data transfert, called automatically
		virtual bool TransferDataToWindow();
		virtual bool TransferDataFromWindow();
		
	};




#endif
