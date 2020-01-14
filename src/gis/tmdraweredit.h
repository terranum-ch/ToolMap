/***************************************************************************
 tmdraweredit.h
                    Drawing data during editing process
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
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


#ifndef _TM_DRAWER_EDIT_H_
#define _TM_DRAWER_EDIT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "wx/overlay.h"


#include "../core/tmarraysize.h"

class tmDrawerEditLine : public wxObject
{
private:
    wxPoint *m_LeftPT;
    wxPoint *m_Pt;
    wxPoint *m_RightPT;
    int m_Index;

    wxPen m_EditPen;
    wxOverlay m_Overlay;


    void DLVertexDelete();

    int DLGetCountPoints();

    wxPoint *DLGetPoints();

    void DLReorderVertex();

protected:

public:
    tmDrawerEditLine();

    ~tmDrawerEditLine();

    // creating and validating
    bool CreateVertex(const wxArrayPoints &pts, int index);

    bool CreateVertex(const wxPoint &pt, wxPoint *left, wxPoint *right, int index);

    bool CreateVertex(const wxPoint &pt);

    bool IsEndVertex();

    bool IsOK();

    bool ClearVertex();

    bool SetVertex(const wxPoint &pt);

    int GetVertexIndex()
    { return m_Index; }

    // Drawing functions
    void SetSymbology(wxColour col, int width);

    bool DrawEditPart(wxClientDC *pdc);

    void DrawEditReset(wxClientDC *pdc);

};


#endif
