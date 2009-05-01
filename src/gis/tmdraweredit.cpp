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
	m_Index = wxNOT_FOUND;
	m_EditPen = *wxBLACK_PEN;
	
}


tmDrawerEditLine::~tmDrawerEditLine()
{
	if(m_Pt != NULL)
	{
		DLVertexDelete();
	}
	
	wxASSERT(m_LeftPT == NULL);
	wxASSERT(m_RightPT == NULL);
}



bool tmDrawerEditLine::IsOK()
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
	
	m_Index = wxNOT_FOUND;
}



int tmDrawerEditLine::DLGetCountPoints()
{
	wxASSERT(m_Pt != NULL);
	if (IsEndVertex()==true)
		return 2;
	
	return 3;
}


wxPoint * tmDrawerEditLine::DLGetPoints ()
{
	wxASSERT(m_Pt != NULL);
	wxPoint * pPts = new wxPoint[DLGetCountPoints()];
	
	int i = 0;
	
	if (m_LeftPT != NULL)
	{
		pPts[i].x = m_LeftPT->x;
		pPts[i].y = m_LeftPT->y;
		i++;
	}
	
	pPts[i].x = m_Pt->x;
	pPts[i].y = m_Pt->y;
	i++;
	
	if (m_RightPT != NULL)
	{
		pPts[i].x = m_RightPT->x;
		pPts[i].y = m_RightPT->y;
	}
	
	return pPts;
}


bool tmDrawerEditLine::CreateVertex(const wxArrayPoints & pts, int index)
{
	wxASSERT(index >= 0);
	if (IsOK()==true)
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
		return false;
	}
		
	wxASSERT(m_Pt == NULL);
	m_Pt = new wxPoint(pts.Item(index));
	m_Index = index;
	
	if (index - 1 >= 0)
		m_LeftPT = new wxPoint(pts.Item(index -1));
	
	if (index + 1 < (signed) pts.GetCount())
		m_RightPT = new wxPoint(pts.Item(index+1));
		
	return true;
}




bool tmDrawerEditLine::CreateVertex(const wxPoint & pt, wxPoint * left, wxPoint * right, int index)
{
	if (IsOK()==true)
		DLVertexDelete();
	
	if (left == NULL && right == NULL)
	{
		wxLogDebug(_("Left or right should not be NULL"));
		return false;
	}
	
	
	wxASSERT(m_Pt == NULL);
	m_Pt = new wxPoint(pt);
	m_Index = index;
	
	if (left != NULL)
		m_LeftPT = new wxPoint(*left);
	
	if (right != NULL)
		m_RightPT = new wxPoint(*right);

	return true;
}



bool tmDrawerEditLine::IsEndVertex ()
{
	if (m_LeftPT == NULL || m_RightPT == NULL)
		return true;
	
	return false;
}



bool tmDrawerEditLine::SetVertex(const wxPoint & pt)
{
	if (IsOK()==false)
	{
		wxLogDebug(_T("Error setting vertex, no existing vertex ! Use")
				   _T(" CreateVertex() function first"));
		return false;
	}
		
	wxASSERT(m_Pt);
	m_Pt->x = pt.x;
	m_Pt->y = pt.y;
	return true;
}


void tmDrawerEditLine::SetSymbology (wxColour col, int width)
{
	m_EditPen.SetColour(col);
	m_EditPen.SetWidth(width);
}


bool tmDrawerEditLine::DrawEditPart(wxClientDC * pdc)
{
	if (IsOK()==false)
	{
		wxLogDebug(_T("Error CreateVertex() first"));
		return false;
	}
	
	wxASSERT(pdc != NULL);
	
	pdc->SetLogicalFunction(wxINVERT);
	pdc->SetPen(m_EditPen);
	
	int iNumPts = DLGetCountPoints();
	wxASSERT (iNumPts >= 2 && iNumPts <=3);
	wxPoint * myPts = DLGetPoints();
	wxASSERT (myPts);
	pdc->DrawLines(iNumPts, myPts);
	
	
	return true;
}


/*bool tmDrawerEditLine::DrawEditedLine (wxClientDC * pdc, tmGISDataVectorMemory * memory)
{
	if (IsOK()==false)
	{
		wxLogDebug(_T("Error CreateVertex() first"));
		return false;
	}
	wxASSERT(pdc != NULL);
	wxASSERT(memory != NULL);
	wxASSERT(memory->GetVertexCount() > 1);
	
	
	return true;
}*/


/*
bool tmDrawerEditLine::DrawFinishPart(wxClientDC * pdc, tmSymbolVectorLine & symbology)
{
	if (IsOK()==false)
	{
		wxLogDebug(_T("Error CreateVertex() first"));
		return false;
	}
	
	wxASSERT(pdc != NULL);
	
	//TODO: Set Pen here
	int iNumPts = DLGetCountPoints();
	wxASSERT (iNumPts >= 2 && iNumPts <=3);
	wxPoint * myPts = DLGetPoints();
	wxASSERT (myPts);
	pdc->DrawLines(iNumPts, myPts);
	return true;
}*/


