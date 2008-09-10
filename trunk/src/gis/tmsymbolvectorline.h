/***************************************************************************
								tmsymbolvectorline.h
				Deals with GIS line vector symbology and associed dialog
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


#ifndef _TM_SYMBOLVECTORLINE_H_
#define _TM_SYMBOLVECTORLINE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmsymbolvector.h"	// tmSymbol class definition
#include "tmsymboldlgline.h" 

class tmSymbolVectorLine : public tmSymbolVector
	{
	private:
		tmSymbolDataLineUnique m_lSymUnique;
		
		virtual tmSymbolDLG * GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos);
		virtual bool GetDialogData(tmSymbolDLG * dlg);
	protected:
	public:
		tmSymbolVectorLine();
		~tmSymbolVectorLine();
		
		virtual bool Serialize(tmSerialize &s);
		wxColour GetColour(){return m_lSymUnique.m_Colour;}
		int GetWidth() {return m_lSymUnique.m_Width;}
		int GetShape() {return tmSYMBOLPENSYLES[m_lSymUnique.m_Shape];}
	};




#endif
