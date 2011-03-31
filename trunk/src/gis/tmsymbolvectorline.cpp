/***************************************************************************
								tmsymbolvectorline.cpp
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

#include "tmsymbolvectorline.h"


tmSymbolVectorLine::tmSymbolVectorLine()
{	
}


tmSymbolVectorLine::tmSymbolVectorLine(const tmSymbolVectorLine & origin)
{
	m_lSymUnique.m_Colour = origin.m_lSymUnique.m_Colour;
	m_lSymUnique.m_Shape = origin.m_lSymUnique.m_Shape;
	m_lSymUnique.m_Width = origin.m_lSymUnique.m_Width;
	m_lSymUnique.m_GlobalTransparency = origin.m_lSymUnique.m_GlobalTransparency;
}


tmSymbolVectorLine::~tmSymbolVectorLine()
{
	
}


tmSymbolDLG * tmSymbolVectorLine::GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos)
{
	tmSymbolDLGLine * dlg = new tmSymbolDLGLine(parent,SYMBOL_TMSYMBOLDLG_IDNAME,
												SYMBOL_TMSYMBOLDLG_TITLE,
												dlgpos);
	dlg->SetDialogData(m_lSymUnique);
	return dlg;
}


bool tmSymbolVectorLine::GetDialogData(tmSymbolDLG * dlg)
{
	m_lSymUnique = ((tmSymbolDLGLine *) dlg)->GetDialogData();
	return TRUE;
}


bool tmSymbolVectorLine::Serialize(tmSerialize &s)
{
	s.EnterObject();
	if(s.IsStoring())
	{
		s << m_lSymUnique.m_Colour;
		s << m_lSymUnique.m_Shape;
		s << m_lSymUnique.m_Width;
		s << m_lSymUnique.m_GlobalTransparency;
	}
	else
	{
		s >> m_lSymUnique.m_Colour;
		s >> m_lSymUnique.m_Shape;
		s >> m_lSymUnique.m_Width;
		s >> m_lSymUnique.m_GlobalTransparency;
	}
	s.LeaveObject();
	
	// return false when the archive encountered an error
	return s.IsOk(); 
}



wxColour tmSymbolVectorLine::GetColour()
{
	return GetColourWithTransparency(m_lSymUnique.m_Colour,
									 m_lSymUnique.m_GlobalTransparency);
}

