/***************************************************************************
								tmsymboldlgpoint.h
							Display Point Symbology dialog
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


#ifndef _TM_SYMBOLDLGPOINT_H_
#define _TM_SYMBOLDLGPOINT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "wx/notebook.h"
#include "wx/spinctrl.h"
#include "tmsymboldlg.h"		// tmSymbolDlg declaration



const int ID_SYMDLGP_PANEL = 10216;
const int ID_SYMDLGP_NOTEBOOK  = 10080;
const int ID_SYMDLGP_PANEL2 = 10081;
const int ID_SYMDLGP_COLOR = 10082;
const int ID_SYMDLGP_WIDTH = 10084;
const int ID_SYMDLGP_PANEL3 = 10085;
const int ID_SYMDLGP_PANEL4 = 10088;
const int ID_SYMDLGP_TRANSPARENCY = 10086;


struct tmSymbolDataPointUnique
{
	int m_PanelNo;
	wxColour m_Colour;
	int m_Radius;
	int m_GlobalTransparency;
};


class tmSymbolDLGPoint : public tmSymbolDLG
	{    
	private:
		tmColourPickerCtrl * m_PointColourCtrl;
		wxSpinCtrl* m_PointWidthCtrl;
		tmSliderWithText * m_TransparencySlider;
		
		tmSymbolDataPointUnique m_DlgData;
		
		void Init();
		void CreateControlsLine();
		
		virtual bool TransferDataToWindow();
		virtual bool TransferDataFromWindow();
				
		DECLARE_DYNAMIC_CLASS( tmSymbolDLGPoint )
		DECLARE_EVENT_TABLE()
		
	public:
		/// Constructors
		tmSymbolDLGPoint();
		~tmSymbolDLGPoint();
		tmSymbolDLGPoint( wxWindow* parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
					const wxString& caption = SYMBOL_TMSYMBOLDLG_TITLE, 
					const wxPoint& pos = SYMBOL_TMSYMBOLDLG_POSITION,
					const wxSize& size = SYMBOL_TMSYMBOLDLG_SIZE,
					long style = SYMBOL_TMSYMBOLDLG_STYLE );
		bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
					const wxString& caption = SYMBOL_TMSYMBOLDLG_TITLE,
					const wxPoint& pos = SYMBOL_TMSYMBOLDLG_POSITION,
					const wxSize& size = SYMBOL_TMSYMBOLDLG_SIZE,
					long style = SYMBOL_TMSYMBOLDLG_STYLE );
		
				
		void SetDialogData ( const tmSymbolDataPointUnique & data) {m_DlgData = data;}
		tmSymbolDataPointUnique GetDialogData () {return m_DlgData;}

				

	};








#endif
