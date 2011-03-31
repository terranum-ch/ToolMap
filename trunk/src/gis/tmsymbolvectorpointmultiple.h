/***************************************************************************
 tmsymbolvectorpointmultiple.h
 Deals with GIS point vector symbology and associed dialog
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber 
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


#ifndef _TM_SYMBOLVECTORPOINT_MULTIPLE_H_
#define _TM_SYMBOLVECTORPOINT_MULTIPLE_H_

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "tmSymbolVectorpoint.h"	// tmSymbol class definition
#include "tmsymboldlgpoint.h" 

class tmSymbolVectorPointMultiple : public tmSymbolVectorPoint
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
		
		wxColour GetColour();
		int GetRadius(){return m_ptUniqueSymbol.m_Radius;}
		virtual int GetTransparency(){return m_ptUniqueSymbol.m_GlobalTransparency;}
		
		
		virtual bool Serialize(tmSerialize &s);
	};




#endif
