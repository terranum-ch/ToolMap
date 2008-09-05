/***************************************************************************
								tmsymboldlgpolygon.h
							Display Polygon Symbology dialog
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


#ifndef _TM_SYMBOLDLGPOLYGON_H_
#define _TM_SYMBOLDLGPOLYGON_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "wx/notebook.h"
#include "wx/spinctrl.h"
#include "tmsymboldlg.h"		// tmSymbolDlg declaration



const int ID_SYMDLGPLG_PANEL = 10216;
const int ID_SYMDLGPLG_NOTEBOOK  = 10080;
const int ID_SYMDLGPLG_PANEL2 = 10081;
const int ID_SYMDLGPLG_BORDER_COLOR = 10082;
const int ID_SYMDLGPLG_FILL_PATTERN = 10083;
const int ID_SYMDLGPLG_BORDER_WIDTH = 10084;
const int ID_SYMDLGPLG_FILL_COLOR = 10087;
const int ID_SYMDLGPLG_PANEL3 = 10085;
const int ID_SYMDLGPLG_PANEL4 = 10088;
const int ID_SYMDLGPLG_TRANSPARENCY = 10086;


struct tmSymbolDataPolygonUnique
{
	int m_PanelNo;
	wxColour m_bColour;
	int m_bWidth;
	wxColour m_fColour;
	int m_fStyle;
	int m_GlobalTransparency;
};


class tmSymbolDLGPolygon : public tmSymbolDLG
	{    
	private:
		tmColourPickerCtrl * m_PolygonBorderColourCtrl;
		tmColourPickerCtrl * m_PolygonFillColourCtrl;
		wxSpinCtrl* m_PolygonBorderWidthCtrl;
		wxChoice* m_PolygonFillPattern;
		tmSliderWithText * m_TransparencySlider;
		
		tmSymbolDataPolygonUnique m_DlgData;
		
		void Init();
		void CreateControlsLine();
		
		virtual bool TransferDataToWindow();
		virtual bool TransferDataFromWindow();

				
		DECLARE_DYNAMIC_CLASS( tmSymbolDLGPolygon )
		DECLARE_EVENT_TABLE()
		
	public:
		/// Constructors
		tmSymbolDLGPolygon();
		~tmSymbolDLGPolygon();
		tmSymbolDLGPolygon( wxWindow* parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
					const wxString& caption = SYMBOL_TMSYMBOLDLG_TITLE, 
					const wxPoint& pos = SYMBOL_TMSYMBOLDLG_POSITION,
					const wxSize& size = SYMBOL_TMSYMBOLDLG_SIZE,
					long style = SYMBOL_TMSYMBOLDLG_STYLE );
		bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
					const wxString& caption = SYMBOL_TMSYMBOLDLG_TITLE,
					const wxPoint& pos = SYMBOL_TMSYMBOLDLG_POSITION,
					const wxSize& size = SYMBOL_TMSYMBOLDLG_SIZE,
					long style = SYMBOL_TMSYMBOLDLG_STYLE );
		
		void SetDialogData ( const tmSymbolDataPolygonUnique & data) {m_DlgData = data;}
		tmSymbolDataPolygonUnique GetDialogData () {return m_DlgData;}

		
				
		
				

	};








#endif
