/***************************************************************************
								tmsymbolvectorpoint.h
				Deals with GIS point vector symbology and associed dialog
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


#ifndef _TM_SYMBOLVECTORPOINT_H_
#define _TM_SYMBOLVECTORPOINT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmsymbolvector.h"	// tmSymbol class definition
#include "tmsymboldlgpoint.h" 

class tmSymbolVectorPoint : public tmSymbolVector
	{
	private:
		tmSymbolDataPointUnique m_ptUniqueSymbol;
		
		virtual tmSymbolDLG * GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos);
		virtual bool GetDialogData(tmSymbolDLG * dlg);
	protected:
	public:
		tmSymbolVectorPoint();
		tmSymbolVectorPoint(const tmSymbolVectorPoint & origin);
		~tmSymbolVectorPoint();
		
		virtual wxColour GetColour();
		virtual int GetRadius(){return m_ptUniqueSymbol.m_Radius;}
		virtual int GetTransparency(){return m_ptUniqueSymbol.m_GlobalTransparency;}
		
		
		virtual bool Serialize(tmSerialize &s);
	};




#endif
