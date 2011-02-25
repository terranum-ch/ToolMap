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


#ifndef _TM_SYMBOLDLGLINE_MULTIPLE_H_
#define _TM_SYMBOLDLGLINE_MULTIPLE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "wx/notebook.h"
#include "wx/spinctrl.h"
#include "tmsymboldlg.h" // tmSymbolDlg declaration


/*struct tmSymbolDataLineUnique
{
	int m_PanelNo;
	wxColour m_Colour;
	int m_Shape;
	int m_Width;
	int m_GlobalTransparency;
};*/



class tmSymbolDLGLineMultiple : public tmSymbolDLG
	{    
	private:
		wxColourPickerCtrl * m_LineColourCtrl;
		wxSpinCtrl* m_LineWidthCtrl;
		wxChoice* m_LinePatternCtrl;
		tmSliderWithText * m_TransparencySlider;
		
		wxNotebook* m_SymbologyTypeCtrl;
		wxChoice* m_MQueryCtrl;
		wxColourPickerCtrl* m_MSelColourCtrl;
		wxChoice* m_MSelSymbolCtrl;
		wxSpinCtrl* m_MSelWidthCtrl;
		wxColourPickerCtrl* m_MUnSelColourCtrl;
		wxChoice* m_MUnSelSymbolCtrl;
		wxSpinCtrl* m_MUnSelWidthCtrl;
		
		
		//tmSymbolDataLineUnique m_DlgData;
		
		void _Init();
		void CreateControlsLine();
		
		virtual bool TransferDataToWindow();
		virtual bool TransferDataFromWindow();
		
		DECLARE_DYNAMIC_CLASS( tmSymbolDLGLine )		
	public:
		/// Constructors
		tmSymbolDLGLineMultiple();
		~tmSymbolDLGLineMultiple();
		tmSymbolDLGLineMultiple( wxWindow* parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
					const wxString& caption = SYMBOL_TMSYMBOLDLG_TITLE, 
					const wxPoint& pos = SYMBOL_TMSYMBOLDLG_POSITION,
					const wxSize& size = SYMBOL_TMSYMBOLDLG_SIZE,
					long style = SYMBOL_TMSYMBOLDLG_STYLE );
		bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
					const wxString& caption = SYMBOL_TMSYMBOLDLG_TITLE,
					const wxPoint& pos = SYMBOL_TMSYMBOLDLG_POSITION,
					const wxSize& size = SYMBOL_TMSYMBOLDLG_SIZE,
					long style = SYMBOL_TMSYMBOLDLG_STYLE );
		
		//void SetDialogData ( const tmSymbolDataLineUnique & data) {m_DlgData = data;}
		//tmSymbolDataLineUnique GetDialogData () {return m_DlgData;}

	};








#endif