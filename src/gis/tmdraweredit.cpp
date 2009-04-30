/***************************************************************************
								tmdraweredit.cpp
                    Drawing data during editing process
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
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

#include "tmdraweredit.h"


tmDrawerEditLine::tmDrawerEditLine()
{
	m_LeftPT = NULL;
	m_Pt = NULL;
	m_RightPT = NULL;
	
}


tmDrawerEditLine::~tmDrawerEditLine()
{
	
}





bool tmDrawerEditLine::DLIsVertexCreated()
{
	if (m_Pt != NULL)
		return true;
	
	return false;
}


void tmDrawerEditLine::DLVertexDelete()
{
	wxASSERT(m_Pt != NULL);
	delete m_Pt;
	m_Pt = NULL;
	
	if (m_LeftPT != NULL)
	{
		delete m_LeftPT;
		m_LeftPT = NULL;
	}
	
	if (m_RightPT != NULL)
	{
		delete m_RightPT;
		m_RightPT = NULL;
	}
}



bool tmDrawerEditLine::CreateVertex(const wxArrayRealPoints & pts, int index)
{
	wxASSERT(index >= 0);
	if (DLIsVertexCreated()==true)
		DLVertexDelete();
	
	if (pts.GetCount() <= 1)
	{
		wxLogError(_("Not enough vertex"));
		return false;
	}
	
	if (index >= (signed) pts.GetCount())
	{
		wxString myErr = _("Trying to get a vertex outside of array");
		wxLogError(myErr);
		//wxASSERT_MSG (0, myErr);
		return false;
	}
		
	wxASSERT(m_Pt == NULL);
	m_Pt = new wxRealPoint(pts.Item(index));
	
	
	if (index - 1 >= 0)
		m_LeftPT = new wxRealPoint(pts.Item(index -1));
	
	if (index + 1 < (signed) pts.GetCount())
		m_RightPT = new wxRealPoint(pts.Item(index+1));
		
	return true;
}



bool tmDrawerEditLine::IsEndVertex ()
{
	if (m_LeftPT == NULL || m_RightPT == NULL)
		return true;
	
	return false;
}