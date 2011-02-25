/***************************************************************************
 tmSymbolVectorLineMultiplemultiple.cpp
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


#include "tmsymbolvectorlinemultiple.h"


tmSymbolVectorLineMultiple::tmSymbolVectorLineMultiple()
{
	m_lSymUnique.m_PanelNo = 0;
	m_lSymUnique.m_Colour = wxColour(*wxBLACK);
	m_lSymUnique.m_Shape = 0;
	m_lSymUnique.m_Width = 1;
	m_lSymUnique.m_GlobalTransparency = 0;
	
}


tmSymbolVectorLineMultiple::tmSymbolVectorLineMultiple(const tmSymbolVectorLineMultiple & origin)
{
	m_lSymUnique.m_PanelNo = origin.m_lSymUnique.m_PanelNo;
	m_lSymUnique.m_Colour = origin.m_lSymUnique.m_Colour;
	m_lSymUnique.m_Shape = origin.m_lSymUnique.m_Shape;
	m_lSymUnique.m_Width = origin.m_lSymUnique.m_Width;
	m_lSymUnique.m_GlobalTransparency = origin.m_lSymUnique.m_GlobalTransparency;
}


tmSymbolVectorLineMultiple::~tmSymbolVectorLineMultiple()
{
	
}


tmSymbolDLG * tmSymbolVectorLineMultiple::GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos)
{
	wxASSERT(GetDatabase());
	tmSymbolDLGLineMultiple * dlg = new tmSymbolDLGLineMultiple(parent, GetDatabase(), SYMBOL_TMSYMBOLDLG_IDNAME,
																SYMBOL_TMSYMBOLDLG_TITLE,
																dlgpos);
	//dlg->SetDialogData(m_lSymUnique);
	return dlg;
}


bool tmSymbolVectorLineMultiple::GetDialogData(tmSymbolDLG * dlg)
{
	m_lSymUnique = ((tmSymbolDLGLine *) dlg)->GetDialogData();
	return TRUE;
}


bool tmSymbolVectorLineMultiple::Serialize(tmSerialize &s)
{
	s.EnterObject();
	if(s.IsStoring())
	{
		s << m_lSymUnique.m_PanelNo;
		s << m_lSymUnique.m_Colour;
		s << m_lSymUnique.m_Shape;
		s << m_lSymUnique.m_Width;
		s << m_lSymUnique.m_GlobalTransparency;
	}
	else
	{
		s >> m_lSymUnique.m_PanelNo;
		s >> m_lSymUnique.m_Colour;
		s >> m_lSymUnique.m_Shape;
		s >> m_lSymUnique.m_Width;
		s >> m_lSymUnique.m_GlobalTransparency;
	}
	s.LeaveObject();
	
	// return false when the archive encountered an error
	return s.IsOk(); 
}



wxColour tmSymbolVectorLineMultiple::GetColour()
{
	return GetColourWithTransparency(m_lSymUnique.m_Colour,
									 m_lSymUnique.m_GlobalTransparency);
}

