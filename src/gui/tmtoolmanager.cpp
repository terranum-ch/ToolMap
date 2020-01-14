/***************************************************************************
 tmtoolmanager.cpp
 Take control of all tools (Dangling nodes, etc.)
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "tmtoolmanager.h"

void tmToolManager::InitMemberValues()
{
    m_Parent = NULL;
    m_TOC = NULL;
    m_Selected = NULL;
    m_Renderer = NULL;
    m_Scale = NULL;
    m_pDB = NULL;
}


tmToolManager::tmToolManager(wxWindow *parent, tmTOCCtrl *toc, tmSelectedDataMemory *seldata, tmRenderer *renderer,
                             tmGISScale *scale)
{
    InitMemberValues();
    m_Parent = parent;
    m_TOC = toc;
    m_Selected = seldata;
    m_Renderer = renderer;
    m_Scale = scale;
}


tmToolManager::~tmToolManager()
{
}


bool tmToolManager::_SearchDanglingNodes(int selectedlayer, const wxArrayString &layersname)
{
    wxASSERT(selectedlayer != wxNOT_FOUND);
    if (selectedlayer < 0)
        return false;

    wxASSERT(m_pDB);
    _ClearDangling();

    // get real layer ids
    wxArrayLong myLayersID;
    wxArrayString mySearchedLayers(layersname);
    mySearchedLayers.RemoveAt(0);

    ProjectDefMemoryLayers myActualLayer;
    int iFirst = m_pDB->GetNextLayer(&myActualLayer);
    wxASSERT(iFirst == 0);
    while (m_pDB->GetNextLayer(&myActualLayer) != -1) {
        for (unsigned int i = 0; i < mySearchedLayers.GetCount(); i++)
            if (mySearchedLayers.Item(i) == myActualLayer.m_LayerName) {
                myLayersID.Add(myActualLayer.m_LayerID);
                break;
            }
    }

    wxASSERT(myLayersID.GetCount() == mySearchedLayers.GetCount());
    if (selectedlayer != 0) {
        long myLayer = myLayersID.Item(selectedlayer - 1);
        myLayersID.Clear();
        myLayersID.Add(myLayer);
    }

    unsigned int iLayersCount = myLayersID.GetCount();
    wxLogDebug(_T("%d layers to check"), iLayersCount);

    wxString smsg = wxString::Format(_("Searching dangling nodes on %d layer(s)"), iLayersCount);
    wxProgressDialog myDlg(_("Searching dangling nodes"), smsg, iLayersCount * 100, m_Parent,
                           wxPD_AUTO_HIDE | wxPD_CAN_ABORT);

    // search dangling nodes
    ToolDanglingNodes myTool(m_pDB);
    bool bStoped = false;
    unsigned int iLoop = 0;
    for (unsigned int i = 0; i < iLayersCount; i++) {
        wxString myLayerName = layersname.Item(i + 1);
        if (iLayersCount == 1) {
            myLayerName = layersname.Item(selectedlayer);
        }
        if (myTool.SearchInit(myLayersID.Item(i), myLayerName) == false) {
            continue;
        }
        if (myTool.SearchRun(&myDlg) == false) {
            bStoped = true;
            break;
        }
        wxLogDebug(_T("Dangling nodes searched for layers : %d"), i);
        myTool.GetDanglingNodes(m_DanglingPts);
        iLoop++;
    }

    if (bStoped == true) {
        _ClearDangling();
        return false;
    }

    // all searched layers are empty !! no message
    if (iLoop == 0) {
        return true;
    }

    wxString myMsg = wxString::Format(_T("%ld dangling nodes found"), m_DanglingPts.GetCount());
    wxLogDebug(myMsg);
    wxMessageBox(myMsg, _("Searching dangling nodes done"), wxICON_INFORMATION | wxOK, m_Parent);

    return true;
}


bool tmToolManager::FindDanglingNodes()
{
    // check DB pointer
    if (_IsOk() == false) {
        return false;
    }

    // get polygons layers
    wxArrayString myLayers;
    myLayers = m_pDB->GetLayerNameByType(LAYER_SPATIAL_POLYGON);
    if (myLayers.GetCount() == 0) {
        wxLogWarning(_T("No polygons layer defined"));
        return false;
    }
    myLayers.Insert(_("All polygon layers"), 0);

    DanglingNode_DLG myDlg(m_Parent);
    myDlg.SetLayers(myLayers, wxNOT_FOUND);
    int iReturn = myDlg.ShowModal();

    wxCommandEvent myUnusedEvent;

    bool bReturn = true;
    switch (iReturn) {
        case wxID_OK:
            bReturn = _SearchDanglingNodes(myDlg.GetSelectedLayer(), myLayers);
            break;

        case ID_DLGDN_CLEAR:
            _ClearDangling();
            break;

        default:
            bReturn = false;
            break;
    }

    m_Renderer->Refresh();
    m_Renderer->Update();
    return bReturn;
}


bool tmToolManager::_IsOk()
{
    if (m_pDB == NULL) {
        wxLogError(_T("Database not inited, init database first"));
        return false;
    }
    return true;
}


long tmToolManager::DrawDanglingNodes(wxGCDC *dc)
{
    if (m_DanglingPts.GetCount() == 0) {
        return 0;
    }

    wxASSERT(dc);
    dc->SetPen(wxPen(tmDANGLING_NODE_DRAW_COLOUR));
    dc->SetBrush(*wxTRANSPARENT_BRUSH);
    for (unsigned int i = 0; i < m_DanglingPts.GetCount(); i++) {
        wxPoint myPt = m_Scale->RealToPixel(m_DanglingPts.Item(i));
        dc->DrawCircle(myPt, tmDANGLING_NODE_DRAW_SIZE);
    }
    return m_DanglingPts.GetCount();
}

