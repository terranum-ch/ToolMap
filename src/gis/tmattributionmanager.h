/***************************************************************************
 tmattributionmanager.h
 Manage the attribution
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

#ifndef _TM_ATTIBUTION_MANAGER_H_
#define _TM_ATTIBUTION_MANAGER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../core/tmshortcutmemory.h"     // for shortcuts in memory
#include "../gui/attribution_obj_type.h"  // for attribution PANEL
#include "tmaattribwindow.h"              // for advanced attribution
#include "tmattributiondatalabel.h"       // for label attributions
#include "tmattributiondataline.h"        // for line attribution
#include "tmattributiondatapoint.h"       // for point attribution
#include "tmmanagerevent.h"               // for shared event with other manager
#include "tmselecteddatamemory.h"         // for selection data
#include "tocctrl.h"                      // for TOC ctrl

class InformationDLG;

class DataBaseTM;

DECLARE_EVENT_TYPE(tmEVT_SHORTCUT_ATTRIBUTION_DONE, -1);

/***************************************************************************/ /**
  @brief Central point for object attribution
  @details This class deals with the attribution process and should know
  following object :
  - The TOC, to get the actual layer type
  - The Attibution Panel
  - The Selected data
  - The database
  @author Lucien Schreiber (c) CREALP 2008
  @date 04 November 2008
  *******************************************************************************/
class tmAttributionManager : public wxEvtHandler {
  private:
    // member data
    wxWindow* m_Parent;
    AttribObjType_PANEL* m_Panel;
    tmSelectedDataMemory* m_SelData;
    TocCtrl* m_TocCtrl;
    DataBaseTM* m_pDB;
    bool m_ShortcutLoaded;
    PrjDefMemManage* m_pPrjMem;
    InformationDLG* m_InfoDLG;

    tmLayerProperties* m_pLayerProperties;
    tmShortcutMemory m_ShortcutMem;

    // event function
    void OnSelection(wxCommandEvent& event);

    // void OnAttributeBtn (wxCommandEvent & event);
    void OnAddBtn(wxCommandEvent& event);

    void OnRemoveBtn(wxCommandEvent& event);

    void OnInfoBtn(wxCommandEvent& event);

    // message event functions
    void OnLayerChanged(wxCommandEvent& event);

    void OnSelectionChanged(wxCommandEvent& event);

    // shortcut function
    void OnRefreshShortcut(wxCommandEvent& event);

    int LoadShortcutIntoMemory();

    void ConnectShortcutEvent();

    void DisconnectShortcutEvent();

    void OnShortcutPressed(wxCommandEvent& event);

    bool ShortcutAttributionChecking(int iCount, int shortcutlayer_type);

    // query function
    void OnRunQuery(wxCommandEvent& event);

    // copy attribution function
    void OnCopyAttribution(wxCommandEvent& event);

    // advanced attribution
    int DisplayAAttributionWindow(wxArrayString* values, PrjMemLayersArray* layers,
                                  const tmLayerValueArray& arrayidname);

    // verification
    bool IsAttributionManagerReady();

    bool IsOnlyOneObjSelected();

    DECLARE_EVENT_TABLE()

  protected:
  public:
    // ctor and dtor
    tmAttributionManager(wxWindow* parent, TocCtrl* toc, AttribObjType_PANEL* panel, tmSelectedDataMemory* selection);

    ~tmAttributionManager();

    // Init attribution
    bool InitAttributionManager(DataBaseTM* pDb, PrjDefMemManage* memprojdef);

    void UnInitAttributionManager();

    // selection informations
    int GetSelectionCount() {
        return m_SelData->GetCount();
    }

    // advanced attribution
    bool AAttributionButtonShow();

    void AAttributionBatchShow();

    // informations
    void DisplayInformationsWnd();

    // Attribution data creation
    static tmAttributionData* CreateAttributionData(int type);
    // orientedpoints
    // void OrientedPointEdit();
    // void OrientedPointDraw();
};

#endif
