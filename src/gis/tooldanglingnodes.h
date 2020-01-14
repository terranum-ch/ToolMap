/***************************************************************************
 tooldanglingnodes.h
                    Tool engine for searching dangling nodes
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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


#ifndef _TOOL_DANGLING_NODES_H_
#define _TOOL_DANGLING_NODES_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../core/tmarraysize.h"
#include "../database/database_tm.h"
#include <wx/progdlg.h>                // for progress dialog definition
#include "../gis/tmgisdatavectormysql.h"    // for mysql gis data manipulation


class DanglingPtsToCheck
{
private:
    void InitMemberValues();

public:
    wxRealPoint m_Pt;
    long m_LineOID;
    int m_Flaged;

    DanglingPtsToCheck()
    {
        m_Pt = wxRealPoint(-1, -1);
        m_LineOID = wxNOT_FOUND;
        m_Flaged = 0;
    }

    DanglingPtsToCheck(const wxRealPoint &pt, long oid)
    {
        m_Pt = pt;
        m_LineOID = oid;
        m_Flaged = 0;
    }

    ~DanglingPtsToCheck()
    { ; }
};

// Creating a list of MemoryFields
WX_DECLARE_OBJARRAY(DanglingPtsToCheck, tmArrayDanglingPtsToCheck);


class ToolDanglingNodes : public wxObject
{
private:
    tmArrayDanglingPtsToCheck m_PtsToCheck;
    DataBaseTM *m_pDB;
    bool m_bSearchInited;
    bool m_bSearchRun;

    long m_LayerID;

    int m_LoopNum;
    wxProgressDialog *m_pDlg;

    tmGISDataVectorMYSQL m_GisData;
    OGRPolygon *m_GeomFrame;


    void DNInitValues();

    // private check
    bool DNIsLayerCorrect(long layerid);

    bool DNIsSearchInitedOk();

    // private search part
    bool DNGetAllLines(long layerid);

    bool DNSearchValidVertex();

    bool DNFlagNodes();

    bool DNIsPointInside(OGRPoint *pt);

    void DNSearchCleanUp();

    bool DNGetFrameGeometry();

    void DNParseFlagedPts(wxArrayRealPoints &dpts);

    // update
    bool DNUpdateProgress(unsigned int ptstocheck, unsigned int iloop);

protected:
public:
    ToolDanglingNodes();

    ToolDanglingNodes(DataBaseTM *database);

    ~ToolDanglingNodes();

    void Create(DataBaseTM *database);

    bool IsOk();

    bool GetDanglingNodes(wxArrayRealPoints &pts);

    bool SearchInit(long layerid, const wxString &layername);

    bool SearchInfo(int &numberlines);

    bool SearchRun(wxProgressDialog *myProgDlg = NULL);

};


#endif
