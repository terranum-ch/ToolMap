/***************************************************************************
								tmsymboldlgline.h
							Display Line Symbology dialog
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


#ifndef _TM_SYMBOLDLGLINE_H_
#define _TM_SYMBOLDLGLINE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "wx/notebook.h"
#include "wx/spinctrl.h"
#include "tmsymboldlg.h"		// tmSymbolDlg declaration



const int ID_SYMDLGL_PANEL = 10216;
const int ID_SYMDLGL_NOTEBOOK  = 10080;
const int ID_SYMDLGL_PANEL2 = 10081;
const int ID_SYMDLGL_COLOR = 10082;
const int ID_CHOICE4 = 10083;
const int ID_SPINCTRL3 = 10084;
const int ID_PANEL12 = 10085;
const int ID_PANEL13 = 10088;
const int ID_SLIDER1 = 10086;
const int ID_TEXTCTRL10 = 10087;

const int ID_BUTTON1 = 10218;






class tmSymbolDLGLine : public tmSymbolDLG
	{    
	private:		
		void Init();
		void CreateControlsLine();
				
		DECLARE_DYNAMIC_CLASS( tmSymbolDLGLine )
		DECLARE_EVENT_TABLE()
		
	public:
		/// Constructors
		tmSymbolDLGLine();
		~tmSymbolDLGLine();
		tmSymbolDLGLine( wxWindow* parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
					const wxString& caption = SYMBOL_TMSYMBOLDLG_TITLE, 
					const wxPoint& pos = SYMBOL_TMSYMBOLDLG_POSITION,
					const wxSize& size = SYMBOL_TMSYMBOLDLG_SIZE,
					long style = SYMBOL_TMSYMBOLDLG_STYLE );
		bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
					const wxString& caption = SYMBOL_TMSYMBOLDLG_TITLE,
					const wxPoint& pos = SYMBOL_TMSYMBOLDLG_POSITION,
					const wxSize& size = SYMBOL_TMSYMBOLDLG_SIZE,
					long style = SYMBOL_TMSYMBOLDLG_STYLE );
		
				
		
				

	};








#endif
