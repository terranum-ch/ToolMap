/***************************************************************************
								tmsymbol.h
				Deals with GIS symbology and associed dialog
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


#ifndef _TM_SYMBOL_H_
#define _TM_SYMBOL_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmsymboldlg.h"
#include "../wxserialize/tmserialize.h"	// for object serialization
#include "tmlayerpropertiesdef.h"	// for tmLayerProperties definition


class tmSymbol : public wxObject
	{
	private:
		virtual tmSymbolDLG * GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos); 
		virtual bool GetDialogData(tmSymbolDLG * dlg);
		
	protected:
		int m_LayerTransparency;
		char ConvertTransparency (int itransparency);
		
	public:
		tmSymbol();
		~tmSymbol();
		
		static tmSymbol * CreateSymbolBasedOnType (TM_GIS_SPATIAL_TYPES  spattype);
		
		int ShowSymbologyDialog (wxWindow * parent, 
								 const wxPoint & dlgpos = wxDefaultPosition);
		virtual bool Serialize(tmSerialize &s);
		virtual int GetTransparency(){return 0;}
		
	};





#endif
