/***************************************************************************
 tmlayermanager.cpp
 Central point for layer management
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
#include "tmlayermanager.h"

#include <wx/stdpaths.h>

#include "../gui/tmclosefile_dlg.h"
#include "tmgisdatarasterweb.h"
#include "tmgisdatavectorshp.h"
#include "tmsymbolvectorline.h"
#include "vrprogress.h"

DEFINE_EVENT_TYPE(tmEVT_SELECTION_DONE);
DEFINE_EVENT_TYPE(tmEVT_LM_ANGLE_CHANGED);
DEFINE_EVENT_TYPE(tmEVT_LM_MOVE_TO_FEATURE);
DEFINE_EVENT_TYPE(tmEVT_LM_ZOOM_TO_FEATURE);
DEFINE_EVENT_TYPE(tmEVT_TOGGLE_FREQUENT);
DEFINE_EVENT_TYPE(tmEVT_ENABLE_ACCELERATORS);
DEFINE_EVENT_TYPE(tmEVT_DISABLE_ACCELERATORS);

BEGIN_EVENT_TABLE(tmLayerManager, wxEvtHandler)
EVT_COMMAND(wxID_ANY, tmEVT_LM_REMOVE, tmLayerManager::RemoveLayer)
EVT_COMMAND(wxID_ANY, tmEVT_LM_ADD, tmLayerManager::AddLayer)
EVT_COMMAND(wxID_ANY, tmEVT_LM_GROUP_ADD, tmLayerManager::GroupAdd)
EVT_MENU(ID_MENU_UNLINK_SPATIAL_DATA, tmLayerManager::OnRemoveLayers)
EVT_COMMAND(wxID_ANY, tmEVT_LM_SIZE_CHANGED, tmLayerManager::OnSizeChange)
EVT_COMMAND(wxID_ANY, tmEVT_LM_MOUSE_MOVED, tmLayerManager::OnUpdateCoordinates)
EVT_COMMAND(wxID_ANY, tmEVT_LM_UPDATE, tmLayerManager::OnShowLayer)
EVT_COMMAND(wxID_ANY, tmEVT_LM_ZOOM_RECTANGLE_IN, tmLayerManager::OnZoomRectangleIn)
EVT_COMMAND(wxID_ANY, tmEVT_LM_ZOOM_RECTANGLE_OUT, tmLayerManager::OnZoomRectangleOut)
EVT_COMMAND(wxID_ANY, tmEVT_LM_ZOOM_TO_FEATURE, tmLayerManager::OnZoomToFeature)
EVT_COMMAND(wxID_ANY, tmEVT_LM_MOVE_TO_FEATURE, tmLayerManager::OnMoveToFeature)
EVT_COMMAND(wxID_ANY, tmEVT_SCALE_USER_CHANGED, tmLayerManager::OnScaleChanged)
EVT_COMMAND(wxID_ANY, tmEVT_LM_PAN_ENDED, tmLayerManager::OnPanFinished)
EVT_COMMAND(wxID_ANY, tmEVT_LM_SHOW_PROPERTIES, tmLayerManager::OnDisplayProperties)
EVT_COMMAND(wxID_ANY, tmEVT_LM_SHOW_LABELS, tmLayerManager::OnDisplayLabels)
EVT_COMMAND(wxID_ANY, tmEVT_LM_SELECTION, tmLayerManager::OnSelection)
EVT_COMMAND(wxID_ANY, tmEVT_LM_ANGLE_CHANGED, tmLayerManager::OnUpdateAngle)
EVT_COMMAND(wxID_ANY, tmEVT_LM_ROTATION_WARNING, tmLayerManager::OnRotationWarning)
EVT_COMMAND(wxID_ANY, tmEVT_LM_INCOMPATIBLE_WARNING, tmLayerManager::OnIncompatibleLayerWarning)
EVT_COMMAND(wxID_ANY, tmEVT_LM_TOC_EDITED, tmLayerManager::OnTocEdited)
END_EVENT_TABLE()

bool tmLayerManager::m_LogOn = true;

/***************************************************************************/ /**
  @brief Constructor
  @details Init values to default and init members.
  @author Lucien Schreiber (c) CREALP 2008
  @date 07 July 2008
  *******************************************************************************/
tmLayerManager::tmLayerManager(wxWindow* parent, TocCtrl* tocctrl, tmRenderer* renderer, wxStatusBar* status,
                               tmScaleCtrlCombo* scalectrl) {
    // default
    m_TocCtrl = nullptr;
    m_Parent = nullptr;
    m_DB = nullptr;
    m_GISRenderer = nullptr;
    m_Bitmap = nullptr;
    m_StatusBar = nullptr;
    m_Thread = nullptr;
    m_ThreadBitmap = nullptr;

    // init selected data structure
    m_Drawer.SetSelectedData(&m_SelectedData);
    m_BlockRefresh = false;
    m_MemoryPrjRef = nullptr;
    m_isUsingRAM = true;
    m_InternetRefreshTime = 250;

    // initing
    m_TocCtrl = tocctrl;
    m_GISRenderer = renderer;
    m_Parent = parent;
    m_StatusBar = status;
    m_ScaleCtrl = scalectrl;
    m_Parent->PushEventHandler(this);
}

/***************************************************************************/ /**
  @brief Destructor
  @author Lucien Schreiber (c) CREALP 2008
  @date 07 July 2008
  *******************************************************************************/
tmLayerManager::~tmLayerManager() {
    UnInitLayerManager();
    m_Parent->PopEventHandler(false);
}

/***************************************************************************/ /**
  @brief Init the layer manager with a project
  @details Call this function when a project is opened, this allows the needed
  initialization to takes place.
  @param db Pointer to a valid #DataBaseTM object (ASSERT is done in debug mode)
  @return  Always TRUE for the moment
  @author Lucien Schreiber (c) CREALP 2008
  @date 07 July 2008
  *******************************************************************************/
bool tmLayerManager::InitLayerManager(DataBaseTM* db) {
    // check
    wxASSERT_MSG(m_TocCtrl != nullptr, _T("Toc Ctrl is null, error"));
    wxASSERT_MSG(db != nullptr, _T("Database pointer is empty... error"));
    m_DB = db;

    // clear selected data
    m_SelectedData.Clear();

    // 2) Init TOCCtrl
    m_TocCtrl->SetProjectName(m_DB->DataBaseGetName());
    FillTOCArray();

    // 3) Init scale from database
    InitScaleCtrlList();

    wxClientDC dc(m_GISRenderer);
    m_Scale.SetPPI(dc.GetPPI());

    // 4) Load Data (not threaded)
    if (!LoadProjectLayers()) {
        wxLogDebug(_T("Layers don't contains any spatial informations yet."));
    }

    return true;
}

/***************************************************************************/ /**
  @brief Un-Init the layer manager
  @details Call this function when a project is closed
  @author Lucien Schreiber (c) CREALP 2008
  @date 07 July 2008
  *******************************************************************************/
bool tmLayerManager::UnInitLayerManager() {
    // saving TOC status
    if (IsOK()) {
        SaveTOCStatus(true);
    }

    m_DB = nullptr;

    wxDELETE(m_Bitmap);
    m_GISRenderer->SetBitmapStatus();

    // clear the scale too
    m_Scale.SetMaxLayersExtent(tmRealRect(0, 0, 0, 0));

    // clear the ctrl
    m_TocCtrl->GetTocModel()->Clear();

    return true;
}

void tmLayerManager::SetMemoryProject(PrjDefMemManage* memoryprj) {
    m_MemoryPrjRef = memoryprj;
    m_Scale.SetUnitAndProj(m_MemoryPrjRef->m_PrjProjType, m_MemoryPrjRef->m_PrjUnitType);
}

/***************************************************************************/ /**
  @brief Fill TOC control with Database TOC values
  @author Lucien Schreiber (c) CREALP 2008
  @date 07 July 2008
  *******************************************************************************/
void tmLayerManager::FillTOCArray() {
    wxASSERT_MSG(m_DB != nullptr, _T("Database pointer is empty... error"));

    // load preference
    wxConfigBase* myConfig = wxFileConfig::Get();
    wxASSERT(myConfig);
    bool myRelativePath = myConfig->ReadBool("GENERAL/relative_path", true);

    tmLayerProperties* lyrproptemp = nullptr;
    while (true) {
        lyrproptemp = m_DB->GetNextTOCEntry(myRelativePath);

        if (lyrproptemp == nullptr) {
            break;
        }

#ifndef USE_NOTES
        if (lyrproptemp->GetName().GetName() == "Notes") {
            wxLogDebug(_("Hiding the Notes layer"));
            continue;
        }
#endif

        if (!m_TocCtrl->InsertLayer(lyrproptemp)) {
            wxLogError(_("Adding layer: %s failed!"), lyrproptemp->GetName().GetName());
            continue;
        }
    }

    wxASSERT(!m_DB->DataBaseHasResults());
    m_TocCtrl->ExpandAll();
    wxLogDebug(_T("%d items added to TOC array"), m_TocCtrl->GetCountLayers());
}

/***************************************************************************/ /**
  @brief Save TOC status to database
  @details This function called when closing project saves the status of the TOC
  ctrl. Only visibility rank of layers and vertex flags are saved, other
  informations are saved in real time when adding a new layer or removing one.
  @return  TRUE if data saved to database, FALSE otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 14 July 2008
  *******************************************************************************/
bool tmLayerManager::SaveTOCStatus(bool isClosing) {
    wxASSERT_MSG(m_TocCtrl, _T("Error TOC ctrl not defined"));
    tmLayerProperties* itemProp = nullptr;
    int iRank = m_TocCtrl->GetCountLayers();

    bool myRelativePath = false;

    // load preference
    if (isClosing) {
        wxConfigBase* myConfig = wxFileConfig::Get();
        wxASSERT(myConfig);
        myRelativePath = myConfig->ReadBool("GENERAL/relative_path", true);
    }

    wxString sSentence = _T("");
    while (true) {
        if (iRank == m_TocCtrl->GetCountLayers()) {
            itemProp = m_TocCtrl->IterateLayers(TRUE);
        } else {
            itemProp = m_TocCtrl->IterateLayers(FALSE);
        }

        if (!itemProp) {
            break;
        }

        // make path relative if file is valid!
        // only for support files
        if (itemProp->GetType() > TOC_NAME_NOT_GENERIC) {
            wxFileName myLayerName(itemProp->GetName());
            if (myLayerName.Exists() && myRelativePath) {
                if (myLayerName.MakeRelativeTo(m_DB->DataBaseGetPath())) {
                    itemProp->SetName(myLayerName);
                }
            }
        }

        wxString symbology;
        if (itemProp->GetType() != TOC_NAME_GROUP) {
            // serialize symbology
            tmSerialize out;
            itemProp->GetSymbolRuleManagerRef()->Serialize(out);
            symbology = out.GetString();
        }

        m_DB->PrepareTOCStatusUpdate(sSentence, itemProp, iRank, symbology);
        iRank--;

        if (iRank < 0) {
            wxLogDebug(_T("Getting rank for layers seem wrong : %d"), iRank);
        }
    }

    // update the database with toc status
    if (!m_DB->DataBaseQueryNoResults(sSentence)) {
        wxLogDebug(_T("Error updating DB with TOC status : %s"), sSentence.c_str());
        return false;
    }

    return true;
}

/***************************************************************************/ /**
  @brief Response to the event sent by the #tmTOCCtrl
  @details Item was already deleted from the tmTOCCtrl and is now removed from
  the DB and the display is refreshed.
  @param event Contain the layer database ID into the GetExtraLong() function
  @author Lucien Schreiber (c) CREALP 2008
  @date 11 July 2008
  *******************************************************************************/
void tmLayerManager::RemoveLayer(wxCommandEvent& event) {
    // checks where allready done by the tmTOCCtrl
    // we can delete item here from the DB
    long litemID = event.GetExtraLong();

    if (!m_DB->RemoveTOCLayer(litemID)) return;

    wxLogDebug(_T("tmLayerManager : removing layer %ld"), litemID);

    LoadProjectLayers();
}

// removing multiple layers with a dialog
void tmLayerManager::OnRemoveLayers(wxCommandEvent& event) {
    // list support layers
    wxASSERT(m_TocCtrl);
    bool bStart = true;
    PrjMemLayersArray myLayers;
    wxArrayString myLayersName;
    while (true) {
        tmLayerProperties* myLayerProp = m_TocCtrl->IterateLayers(bStart);
        bStart = false;
        if (myLayerProp == nullptr) {
            break;
        }
        if (myLayerProp->GetType() > TOC_NAME_NOT_GENERIC) {
            auto* myLayer = new ProjectDefMemoryLayers;
            myLayer->m_LayerID = myLayerProp->GetID();
            myLayer->m_LayerName = myLayerProp->GetNameDisplay();
            myLayer->m_LayerType = (PRJDEF_LAYERS_TYPE)myLayerProp->GetType();
            myLayers.Insert(myLayer, 0);
            myLayersName.Insert(myLayerProp->GetNameDisplay(), 0);
        }
    }

    // display dialog
    tmCloseFile_DLG myDlg(m_Parent, myLayersName);
    if (myDlg.ShowModal() != wxID_OK) {
        return;
    }

    wxArrayInt myLayerToRemoveIndex = myDlg.GetSelections();
    if (myLayerToRemoveIndex.IsEmpty()) {
        wxLogWarning(_("Nothing selected, no layer will be closed"));
        return;
    }

    // removing
    for (unsigned int i = 0; i < myLayerToRemoveIndex.GetCount(); i++) {
        long my_item_id = myLayers.Item(myLayerToRemoveIndex.Item(i))->m_LayerID;
        if (!m_TocCtrl->GetLayerById(my_item_id)) {
            wxLogError(_("Item with layer id : %ld not found in the TOC"), my_item_id);
            continue;
        }
        m_TocCtrl->RemoveLayer(my_item_id);
        if (!m_DB->RemoveTOCLayer(my_item_id)) {
            wxLogError(_("Unable to remove layer : '%s'"),
                       myLayers.Item(myLayerToRemoveIndex.Item(i))->m_LayerName.c_str());
        }
    }
    LoadProjectLayers();
}

void tmLayerManager::OnRotationWarning(wxCommandEvent& event) {
    wxASSERT(m_RotationName.GetCount() == m_RotationStatus.GetCount());

    auto* myPt = (wxRealPoint*)event.GetClientData();
    wxASSERT(myPt);
    double rx = myPt->x;
    double ry = myPt->y;
    wxDELETE(myPt);

    // avoid 2 times the same rotation dialog
    for (unsigned int i = 0; i < m_RotationLayerNames.GetCount(); i++) {
        if (m_RotationLayerNames.Item(i) == event.GetString()) {
            return;
        }
    }
    m_RotationLayerNames.Add(event.GetString());

    bool bShouldAdd = true;
    bool bShouldDisplay = true;
    for (unsigned int i = 0; i < m_RotationName.GetCount(); i++) {
        if (m_RotationName.Item(i) == event.GetString()) {
            bShouldAdd = false;
            if (m_RotationStatus.Item(i) == 1) {  // hide message
                bShouldDisplay = false;
            }
            break;
        }
    }

    // display dialog
    if (bShouldDisplay) {
        tmRotationWarning_DLG myDlg(nullptr, wxID_ANY, _("Rotation Warning"));
        myDlg.SetLayerName(event.GetString());
        myDlg.SetRotation1(rx);
        myDlg.SetRotation2(ry);
        if (myDlg.ShowModal() == wxID_OK) {
            if (bShouldAdd) {
                m_RotationName.Add(event.GetString());
                m_RotationStatus.Add((short)myDlg.GetHide());
            }

            // update hide information
            for (unsigned int i = 0; i < m_RotationName.GetCount(); i++) {
                if (m_RotationName.Item(i) == event.GetString()) {
                    m_RotationStatus.Item(i) = (short)myDlg.GetHide();
                }
            }
        }
    }
}

void tmLayerManager::OnIncompatibleLayerWarning(wxCommandEvent& event) {
    wxLogWarning(_("File %s has incompatible transformation coefficients and cannot be displayed."),
                 wxFileName(event.GetString()).GetFullName());

    tmLayerProperties* item = m_TocCtrl->GetLayerByPath(event.GetString());

    if (item) {
        m_DB->RemoveTOCLayer(item->GetID());
        m_TocCtrl->RemoveLayer(item->GetID());
    }
}

/***************************************************************************/ /**
  @brief Response to the event sent by the "Add Gis Data" menu
  @details This function is doing following operations :
  - Checking that #tmTOCCtrl is ready : tmTOCCtrl::IsTOCReady()
  - Importing data
  - Saving layer to the database and getting back his real ID
  - Adding layer to the tmTOCCtrl
  with his real ID.
  @author Lucien Schreiber (c) CREALP 2008
  @date 11 July 2008
  *******************************************************************************/
void tmLayerManager::AddLayer(wxCommandEvent& event) {
    // check that the a project was opened !
    // normal project contain 4 layers minimum
    if (!IsOK()) {
        return;
    }

    // create All supported extension
    wxString myExt = _("All supported formats|");
    wxArrayString myExts = tmGISData::GetAllSupportedGISFormatsExtensions();
    for (unsigned int i = 0; i < myExts.GetCount(); i++) {
        myExt.Append(myExts.Item(i));
        myExt.Append(_T(";"));
    }
    myExt.RemoveLast();
    myExt.Append(_T("|"));
    myExt.Append(tmGISData::GetAllSupportedGISFormatsWildcards());

    wxFileDialog myDlg(m_Parent, _("Link Data"), wxEmptyString, _T(""), myExt,
                       wxFD_OPEN | wxFD_MULTIPLE | wxFD_CHANGE_DIR);
    if (myDlg.ShowModal() == wxID_CANCEL) {
        return;
    }

    wxString myLastOpenedPath = myDlg.GetDirectory();
    wxArrayString myFilesNames;
    myDlg.GetPaths(myFilesNames);

    // zoom to full extent only if only base layer are present.
    bool bZoomToFullExtent = false;
    if (m_TocCtrl->GetCountLayers() == 5) {
        bZoomToFullExtent = true;
    }

    for (unsigned int i = 0; i < myFilesNames.GetCount(); i++) {
        wxFileName myFilename(myFilesNames.Item(i));
        if (!OpenLayer(myFilename, false)) {
            continue;
        }
    }

    ReloadProjectLayers(bZoomToFullExtent, true);
}

/// Create the group, and add it to the database. Then call the
/// TocCtrl and add it.
void tmLayerManager::GroupAdd(wxCommandEvent& event) {
    if (!IsOK()) {
        return;
    }

    wxString newGroupName = wxGetTextFromUser("Group name");
    if (newGroupName.IsEmpty()) {
        return;
    }
    wxASSERT(m_DB);
    auto* layer_properties = new tmLayerProperties();
    layer_properties->SetType(TOC_NAME_GROUP);
    layer_properties->SetName(wxFileName("", newGroupName));
    layer_properties->SetVisible(true);

    long my_group_id = m_DB->AddTOCLayer(layer_properties);
    if (my_group_id == wxNOT_FOUND) {
        return;
    }
    layer_properties->SetID(my_group_id);

    // if something is selected in the toc, use it as a parent
    wxDataViewItem sel_item = m_TocCtrl->GetSelection();
    if (sel_item.IsOk()) {
        auto node = TocCtrlModel::ConvertFromDataViewItem(sel_item);
        wxASSERT(node);
        if (node->IsContainer()) {
            layer_properties->SetLayerParentId(node->m_LayerProp->GetID());
        } else {
            layer_properties->SetLayerParentId(node->GetParent()->m_LayerProp->GetID());
        }
    }
    m_TocCtrl->InsertLayer(layer_properties);
}

void tmLayerManager::AddWebLayer() {
    // list WMS xml files in share/toolmap
    wxString myWebPagePathText = _T("/../share/toolmap");
#ifdef __WXMSW__
    myWebPagePathText.Replace(_T("/"), _T("\\"));
#endif
    wxFileName myWebPath(wxStandardPaths::Get().GetExecutablePath());
    myWebPath.SetPath(myWebPath.GetPath() + myWebPagePathText);
    myWebPath.SetName(wxEmptyString); // Remove the name of the binary
    myWebPath.SetExt("");
    if(!myWebPath.Normalize(wxPATH_NORM_ABSOLUTE)){
        wxLogError(_("Unable to normalize path: '%s"), myWebPath.GetFullPath());
        return ;
    };
    wxLogDebug(myWebPath.GetFullPath());

    if (!myWebPath.Exists()) {
        wxLogError(_("WMS directory didn't exists! Try re-installing ToolMap"));
        return;
    }

    // create the list with all WMS files
    wxArrayString myWMSFilesFullPath;
    wxDir::GetAllFiles(myWebPath.GetFullPath(), &myWMSFilesFullPath, _T("*.xml"), wxDIR_FILES);
    if (myWMSFilesFullPath.GetCount() == 0) {
        wxLogError(_("No WMS files found! Try re-installing ToolMap!"));
        return;
    }

    wxArrayString myWMSFilesShortNames;
    for (unsigned int i = 0; i < myWMSFilesFullPath.GetCount(); ++i) {
        wxString myFileName = wxEmptyString;
        wxFileName::SplitPath(myWMSFilesFullPath.Item(i), nullptr, nullptr, &myFileName, nullptr);
        myFileName.Replace(_T("_"), _T(" "));
        myWMSFilesShortNames.Add(myFileName);
    }

    wxMultiChoiceDialog myDlg(m_Parent, _("Select web layer to add:"), _("Add web layer"), myWMSFilesShortNames);
    if (myDlg.ShowModal() == wxID_CANCEL) {
        return;
    }

    for (unsigned int j = 0; j < myDlg.GetSelections().GetCount(); ++j) {
        OpenLayer(wxFileName(myWMSFilesFullPath.Item(myDlg.GetSelections().Item(j))));
    }
    ReloadProjectLayers(false, false);
}

bool tmLayerManager::_ReplaceLayer(const wxFileName& filename, const wxString& originalname) {
    if (originalname == wxEmptyString) {
        return false;
    }

    wxRegEx myRegex(_T("-([0-9]{3})$"));
    // check if layer exists
    bool bReset = true;
    tmLayerProperties* myLayerToReplace = nullptr;
    while (true) {
        tmLayerProperties* myLayer = m_TocCtrl->IterateLayers(bReset);
        bReset = false;
        if (myLayer == nullptr) {
            return false;
        }
        if (myLayer->GetType() != TOC_NAME_SHP) {
            continue;
        }

        wxString myTOCLayerName = myLayer->GetName().GetName();
        wxString myReplaceNameOrigin = originalname;

        // is original filename
        if (myTOCLayerName == myReplaceNameOrigin) {
            myLayerToReplace = myLayer;
            break;
        }

        // check for modified filename (-XXX)
        if (myRegex.Matches(myTOCLayerName)) {
            wxString myTocWithoutIncrement = myTOCLayerName.Left(myTOCLayerName.Len() - 4);
            if (myTocWithoutIncrement == myReplaceNameOrigin) {
                myLayerToReplace = myLayer;
                break;
            }
        }
    }

    wxFileName myCompleteName(filename);
    // myCompleteName.SetExt(filename.GetExt());
    m_TocCtrl->UpdateLayerName(myLayerToReplace, myCompleteName.GetFullName());
    myLayerToReplace->SetName(myCompleteName);

    // Update database
    wxString myQuery = _T("UPDATE %s SET CONTENT_PATH=\"%s\", CONTENT_NAME=\"%s\" WHERE CONTENT_ID = %ld;");
    wxString myPath = myCompleteName.GetPath();
    DataBaseTM::ConvertPath(myPath);
    if (!m_DB->DataBaseQuery(
            wxString::Format(myQuery, TABLE_NAME_TOC, myPath, myCompleteName.GetFullName(), myLayerToReplace->GetID()),
            true)) {
        return false;
    }

    // need to create an attribut index ?
    if (myLayerToReplace->GetSymbolRuleManagerRef()->GetRulesRef()->GetCount() > 0 &&
        myLayerToReplace->GetSymbolRuleManagerRef()->IsUsingRules()) {
        auto myGISData = (tmGISDataVectorSHP*)tmGISData::LoadLayer(myLayerToReplace);
        if (myGISData == nullptr) {
            wxLogError(_("Unable to load layer : %s"), myLayerToReplace->GetName().GetName());
            return false;
        }
        wxString myQuery2 = wxString::Format(_T("DROP INDEX on %s"), myLayerToReplace->GetName().GetName());
        myGISData->ExecuteSQLQuery(myQuery2);
        wxString myFieldName = myLayerToReplace->GetSymbolRuleManagerRef()->GetFieldName();
        if (myFieldName != wxEmptyString) {
            myQuery2 = wxString::Format(_T("CREATE INDEX ON %s USING %s"), myLayerToReplace->GetName().GetName(),
                                        myFieldName);
            myGISData->ExecuteSQLQuery(myQuery2);
            wxLogMessage(_("Creating attribut filter for : %s"), myFieldName);
        }
        wxDELETE(myGISData);
    }
    return true;
}

void tmLayerManager::_BuildOverviewsIfNeeded(tmGISData* layer, const wxString& displayname) {
    // build pyramids and spatial index ??
    wxConfigBase* myConfig = wxFileConfig::Get();
    wxASSERT(myConfig);
    bool bCreateIndex = myConfig->ReadBool("SPATIAL_INDEX/create_index", true);
    if (bCreateIndex) {
        if (layer->IsRaster() == 1) {
            tmGISDataRaster* myRaster = (tmGISDataRaster*)layer;
            wxArrayString myPyramids;
            int myGetPyramidInfo = myRaster->GetPyramidsInfo(&myPyramids);
            if (myPyramids.GetCount() == 0 && myGetPyramidInfo != wxNOT_FOUND) {
                vrProgressSimple myProgressData(m_Parent, _("Building Overviews (Pyramids)"),
                                                wxString::Format(_("Building Overviews: '%s'"), displayname));
                layer->CreateSpatialIndex(GDALUpdateSimple, &myProgressData);
            }
        } else {
            layer->CreateSpatialIndex(nullptr, nullptr);
        }
    }
}

bool tmLayerManager::OpenLayer(const wxFileName& filename, bool replace, const wxString& originalname) {
    if (replace) {
        if (_ReplaceLayer(filename, originalname)) {
            return true;
        }
    }

    tmLayerProperties* item = new tmLayerProperties();
    item->InitFromPathAndName(filename.GetPath(), filename.GetFullName(),
                              tmGISData::GetAllSupportedGISFormatsExtensions());

    // try to open the file for getting the spatial type
    tmGISData* myLayer = tmGISData::LoadLayer(item);
    if (myLayer == nullptr) {
        wxLogError(_("Not able to open the layer : %s"), item->GetNameDisplay().c_str());
        wxDELETE(item);
        return false;
    }

    _BuildOverviewsIfNeeded(myLayer, item->GetNameDisplay());

    item->SetSpatialType(myLayer->GetSpatialType());
    wxDELETE(myLayer);
    if (item->GetSpatialType() == LAYER_EMPTY) {
        wxLogError(_("Layer '%s' is empty and will not be loaded."), item->GetNameDisplay());
        wxDELETE(item);
        return false;
    }
    if (item->GetSpatialType() == LAYER_ERR || item->GetSpatialType() == LAYER_SPATIAL_UNKNOWN) {
        wxLogError(_("Spatial type of layer '%s' unknown or not supported!"), item->GetNameDisplay());
        wxDELETE(item);
        return false;
    }

    item->InitSymbology(wxEmptyString);
    // saving to the database and getting the last ID
    long lastinsertedID = m_DB->AddTOCLayer(item);
    if (lastinsertedID < 0) {
        wxLogError(_("Error Adding layer: '%s'"), item->GetNameDisplay());
        wxDELETE(item);
        return false;
    }

    item->SetID(lastinsertedID);
    wxLogDebug(_T("Last inserted item id is : %ld"), lastinsertedID);

    // adding entry to TOC
    if (!m_TocCtrl->InsertLayer(item)) {
        wxLogError(_("Error adding layer: '%s' into TOC"), item->GetNameDisplay());
        wxDELETE(item);
        return false;
    }
    return true;
}

void tmLayerManager::ZoomToSelectedLayer() {
    tmLayerProperties* myLayerProp = m_TocCtrl->GetSelectionLayer();
    if (myLayerProp == nullptr) {
        wxLogError(_("No layer or incorrect layer selected"));
        return;
    }

    ZoomToLayer(myLayerProp->GetID());
}

void tmLayerManager::ZoomToFrameLayer() {
    tmLayerProperties* myLayerProp = m_TocCtrl->GetLayerByName(TOC_GENERIC_NAME_STRING[TOC_NAME_FRAME]);
    wxASSERT(myLayerProp);
    ZoomToLayer(myLayerProp->GetID());
}

bool tmLayerManager::ZoomToLayer(long layerid) {
    tmLayerProperties* myLayerProp = m_TocCtrl->GetLayerById(layerid);
    if (myLayerProp == nullptr) {
        wxLogError(_("Layer with specified id doesn't exists (ID = %ld)"), layerid);
        return false;
    }

    tmGISData* myGISData = tmGISData::LoadLayer(myLayerProp);
    if (myGISData == nullptr) {
        wxLogError(_("Unable to get data for layer %s"), myLayerProp->GetNameDisplay().c_str());
        return false;
    }

    wxASSERT(myGISData);
    tmRealRect myRealRect = myGISData->GetMinimalBoundingRectangle();
    vrRealRect myRRect;
    myRRect.SetLeftTop(wxPoint2DDouble(myRealRect.x_min, myRealRect.y_max));
    myRRect.SetRightBottom(wxPoint2DDouble(myRealRect.x_max, myRealRect.y_min));
    wxDELETE(myGISData);

    // layer extent seems correct
    if (!myRRect.IsOk()) {
        wxLogError(_T("Layer extent isn't correct : %.3f, *.3f | %.3f, %.3f"), myRRect.GetLeft(), myRRect.GetTop(),
                   myRRect.GetRight(), myRRect.GetBottom());
        return false;
    }

    if (!m_Scale.ZoomViewTo(myRRect)) {
        wxLogError(_T("Zooming to layer n\u00B0%ld failed"), layerid);
        return false;
    }

    ReloadProjectLayers(FALSE);
    _ZoomChanged();
    return true;
}

/***************************************************************************/ /**
  @brief Called when windows size change
  @details This function is called by the renderer (#tmRenderer) when the size
  change. It call the rendering of all layers in a multi-threaded way.
  @param event Contain a tmArraySize with :
  - Item 0 containing the old Size (in pixels).
  - Item 1 containing the new size (in pixels).
  @author Lucien Schreiber (c) CREALP 2008
  @date 14 October 2008
  *******************************************************************************/
void tmLayerManager::OnSizeChange(wxCommandEvent& event) {
    // pass size to scale object but don't make
    // any computation if no project are opened
    tmArraySize* mySizes = (tmArraySize*)event.GetClientData();
    wxASSERT(mySizes);
    wxSize myOldSize = mySizes->Item(0);
    wxSize myNewSize = mySizes->Item(1);
    // bool mybSmaller = (bool) event.GetInt();
    delete mySizes;

    // actual wnd extent
    m_Scale.SetWindowExtent(wxRect(0, 0, myNewSize.GetWidth(), myNewSize.GetHeight()));

    // compute reel size in MM
    wxClientDC dc(m_GISRenderer);

    if (dc.GetWindow() == nullptr) return;

    m_Scale.SetWindowExtentMM(dc.GetSizeMM());

    // ensure that a project is opened
    if (!IsOK()) return;
    // compute new extend in map unit

    // compute new pixel size
    if (m_Scale.ComptuteNewWindowSize(myOldSize, myNewSize)) {
        ReloadProjectLayers(false, false);
    }

    event.Skip();
}

/***************************************************************************/ /**
  @brief Respond to mouse mouve inside renderer area
  @details This function is called automatically by the tmRenderer class when the
  mouse is moved inside the display area
  @author Lucien Schreiber (c) CREALP 2008
  @date 24 July 2008
  *******************************************************************************/
void tmLayerManager::OnUpdateCoordinates(wxCommandEvent& event) {
    wxPoint* pmousepoint = (wxPoint*)event.GetClientData();
    wxPoint mousepoint(*pmousepoint);
    wxDELETE(pmousepoint);

    if (!m_StatusBar) {
        return;
    }

    // ensure that a project is opened
    if (!IsOK()) {
        return;
    }

    if (!m_Scale.IsLayerExtentValid()) {
        m_StatusBar->SetStatusText(_T(""), 0);
        return;
    }
    // clear status bar
    m_StatusBar->SetStatusText(_T(""), 4);
    wxRealPoint mouserealpoint = m_Scale.PixelToReal(mousepoint);

    if (m_MemoryPrjRef == nullptr) {
        return;
    }

    tmCoordConvert myConvert(m_MemoryPrjRef->m_PrjProjType);

    wxString myXtxt = wxEmptyString;
    wxString myYtxt = wxEmptyString;
    wxString myUnit = wxEmptyString;

    switch (m_MemoryPrjRef->m_PrjUnitType) {
        case UNIT_METERS:
            myXtxt = wxString::FromCDouble(mouserealpoint.x, 2);
            myYtxt = wxString::FromCDouble(mouserealpoint.y, 2);
            myUnit = _T(" [m]");
            break;

        case UNIT_DMS:
            mouserealpoint = myConvert.GetPointWGS(mouserealpoint);
            myXtxt = wxString(CPLDecToDMS(mouserealpoint.x, "Long", 3));
            myYtxt = wxString(CPLDecToDMS(mouserealpoint.y, "Lat", 3));
            myUnit = _T(" [DMS]");
            break;

        default:
            mouserealpoint = myConvert.GetPointWGS(mouserealpoint);
            myXtxt = wxString::FromCDouble(mouserealpoint.x);
            myYtxt = wxString::FromCDouble(mouserealpoint.y);
            myUnit = _T(" [DD]");
            break;
    }

    wxString myStatusText = myXtxt + _T(" ") + myYtxt + myUnit;
    m_StatusBar->SetStatusText(myStatusText, 0);
}

void tmLayerManager::OnUpdateAngle(wxCommandEvent& event) {
    if (!m_StatusBar) return;

    int myAngle = event.GetInt();

    if (myAngle == wxNOT_FOUND) {
        m_StatusBar->SetStatusText(_T(""), 3);
        return;
    }

    wxString myAngleText = wxString::Format(_T("Angle : %d\u00B0"), myAngle);
    m_StatusBar->SetStatusText(myAngleText, 3);
}

/***************************************************************************/ /**
  @brief Called when showing or hiding a layer
  @details This function is called by the TOC (#tmTOCCtrl) when a layers is
  displayed or hidden
  @param event empty.
  @author Lucien Schreiber (c) CREALP 2008
  @date 14 October 2008
  *******************************************************************************/
void tmLayerManager::OnShowLayer(wxCommandEvent& event) {
    ReloadProjectLayers(FALSE);
}

/***************************************************************************/ /**
  @brief Called when user change scale
  @details This function is called by the Scale control (#tmScaleCtrlCombo) when
  user specify a scale.
  @param event empty.
  @author Lucien Schreiber (c) CREALP 2008
  @date 14 October 2008
  *******************************************************************************/
void tmLayerManager::OnScaleChanged(wxCommandEvent& event) {
    // checking that project is open and contain GIS data
    if (!IsOK() && m_Scale.GetMaxLayersExtent() == tmRealRect(0, 0, 0, 0)) {
        wxLogError(_T("Not able to process scale! Open a project first!"));
        return;
    }

    // computing new real extent based on scale
    m_Scale.ComputeNewScaleExtent(event.GetExtraLong());
    ReloadProjectLayers(false);
    _ZoomChanged();
}

/***************************************************************************/ /**
  @brief Show properties dialog
  @param event Contain the #tmLayerProperties info. Use GetClientInfo() for
  getting info out.
  @author Lucien Schreiber (c) CREALP 2008
  @date 22 October 2008
  *******************************************************************************/
void tmLayerManager::OnDisplayProperties(wxCommandEvent& event) {
    tmLayerProperties* itemProp = (tmLayerProperties*)event.GetClientData();
    if (!itemProp) {
        wxLogError(_("Problem during data transfert"));
        return;
    }

    wxASSERT(m_DB);
    itemProp->GetSymbolRef()->SetDatabase(m_DB);
    itemProp->GetSymbolRef()->SetTocName(itemProp->GetType());

    if (itemProp->GetType() == TOC_NAME_SHP) {
        if (itemProp->GetSymbolRuleManagerRef()->ShowSymbolRuleDlg(m_TocCtrl, wxGetMousePosition())) {
            ReloadProjectLayers(false);
        }

        // save the TOC
        wxCommandEvent evtSave(tmEVT_LM_TOC_EDITED, wxID_ANY);
        this->QueueEvent(evtSave.Clone());

        return;
    }

    if (itemProp->GetSymbolRef()->ShowSymbologyDialog(m_TocCtrl, wxGetMousePosition()) == wxID_OK) {
        ReloadProjectLayers(false);
    }

    // save the TOC
    wxCommandEvent evtSave(tmEVT_LM_TOC_EDITED, wxID_ANY);
    this->QueueEvent(evtSave.Clone());
}

void tmLayerManager::OnDisplayLabels(wxCommandEvent& event) {
    wxDataViewItem item = (wxDataViewItem)event.GetClientData();
    auto node = TocCtrlModel::ConvertFromDataViewItem(item);
    if (node == nullptr) {
        return;
    }
    auto itemProp = node->m_LayerProp;
    wxASSERT(m_DB);
    itemProp->GetSymbolRef()->SetDatabase(m_DB);
    itemProp->GetSymbolRef()->SetTocName(itemProp->GetType());
    if (itemProp->GetSymbolRef()->ShowLabelDialog(m_TocCtrl, itemProp, wxGetMousePosition()) == wxID_OK) {
        ReloadProjectLayers(false);
    }

    // save the TOC
    wxCommandEvent evtSave(tmEVT_LM_TOC_EDITED, wxID_ANY);
    this->QueueEvent(evtSave.Clone());
}

void tmLayerManager::OnTocEdited(wxCommandEvent& event) {
    // saving TOC status
    if (IsOK()) {
        SaveTOCStatus(false);
    }
}

/***************************************************************************/ /**
  @brief Called when user press Zoom to fit
  @details This functions does the following :
  - Loading all visible layers (computing maximum extent)
  - Drawing
  This function is threaded, one can call
  this multiple time, if a thread allready exists it will be destroyed and a new
  one will be started
  @author Lucien Schreiber (c) CREALP 2008
  @date 24 July 2008
  *******************************************************************************/
void tmLayerManager::OnZoomToFit() {
    ReloadProjectLayers();
    _ZoomChanged();
}

void tmLayerManager::OnZoomRectangle() {
    m_GISRenderer->SetTool(tmTOOL_ZOOM_RECTANGLE);
}

void tmLayerManager::OnPan() {
    m_GISRenderer->SetTool(tmTOOL_PAN);
}

void tmLayerManager::OnSelect() {
    m_GISRenderer->SetTool(tmTOOL_SELECT);
}

/***************************************************************************/ /**
  @brief Searching data for selection
  @details This function search and update the #tmSelectedDataMemory structure
  with searched data. If this function return true we should redraw all layers
  @param rect area we sould search for selecting data
  @param shiftdown status of the shift key. If down we mainly add data to the
  selection
  @return  true if function succeed and we should reload layers, false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 29 October 2008
  *******************************************************************************/
bool tmLayerManager::SelectedSearch(const wxRect& rect, bool shiftdown) {
    // is a project open
    if (!IsOK()) {
        if (IsLoggingEnabled()) {
            wxLogMessage(_("Open a project first"));
        }
        return false;
    }

    // is a layer selected and wich one
    tmLayerProperties* layerprop = m_TocCtrl->GetSelectionLayer();
    if (!layerprop) {
        if (IsLoggingEnabled()) {
            wxLogMessage(_("Select a layer first in the TOC"));
        }
        return false;
    }

    // converting wxRect to RealRect
    tmRealRect mySelReal = m_Scale.PixelsToReal(rect);
    tmGISData* myLayerData = tmGISData::LoadLayer(layerprop);
    if (!myLayerData) {
        if (IsLoggingEnabled()) {
            wxLogDebug(_T("Unable to get data for the layer"));
        }
        return false;
    }

    // TODO: Remove, temp code for drawing rectangle
    // wxClientDC dc(m_GISRenderer);
    // dc.DrawLine(m_Scale.RealToPixel(wxRealPoint(mySelReal.x_min, mySelReal.y_max)),
    // m_Scale.RealToPixel(wxRealPoint(mySelReal.x_max, mySelReal.y_min)));

    // searching for data
    wxArrayLong* myArray = myLayerData->SearchData(mySelReal, layerprop->GetType());
    if (myArray == nullptr) {
        myArray = new wxArrayLong();
    }
    int myArrayCount = myArray->GetCount();

    // are we in the same layer ?
    m_SelectedData.SetLayerID(layerprop->GetID());

    if (IsLoggingEnabled()) {
        wxLogDebug(_T("Number of features selected : %d"), myArrayCount);

        for (int i = 0; i < myArrayCount; i++) {
            if (i == 10) {
                wxLogDebug(_T("- More selected features selected .... "));
                break;
            }
            wxLogMessage(_T(" - Selected Features ID : %ld"), myArray->Item(i));
        }
    }

    // add, remove or clear selection depending on :
    // - number of items selected
    // - Status of shift key.
    if (myArrayCount >= 1) {
        if (!shiftdown) m_SelectedData.Clear();
        m_SelectedData.AddSelected(myArray);
    } else {
        if (!shiftdown) m_SelectedData.Clear();
    }

    myArray->Clear();
    wxDELETE(myArray);
    wxDELETE(myLayerData);
    return true;
}

/***************************************************************************/ /**
  @brief Clear all selected objects
  @details This function clear all selected objects
  @return  true if they where selected items, false otherwise
  @author Lucien Schreiber (c) CREALP 2009
  @date 31 March 2009
  *******************************************************************************/
bool tmLayerManager::SelectedClear() {
    bool bReturn = false;
    if (m_SelectedData.GetCount() > 0) bReturn = true;

    m_SelectedData.Clear();
    ReloadProjectLayers(false, true);

    wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
    m_Parent->GetEventHandler()->QueueEvent(evt.Clone());
    return bReturn;
}

/***************************************************************************/ /**
  @brief Invert selected values
  @return true if sucess, false otherwise
  @author Lucien Schreiber (c) CREALP 2009
  @date 31 March 2009
  *******************************************************************************/
bool tmLayerManager::SelectedInvert() {
    // selection have value ?
    if (m_SelectedData.GetCount() == 0) {
        if (IsLoggingEnabled()) {
            wxLogDebug(_T("No value selected, select object(s) first"));
        }
        return false;
    }

    // get all selected values
    tmLayerProperties* layerprop = m_TocCtrl->GetSelectionLayer();
    if (!layerprop) {
        if (IsLoggingEnabled()) {
            wxLogMessage(_("Select a layer first in the TOC"));
        }
        return false;
    }

    tmGISData* myLayerData = tmGISData::LoadLayer(layerprop);
    if (!myLayerData) {
        if (IsLoggingEnabled()) {
            wxLogDebug(_T("Unable to get data for the layer"));
        }
        return false;
    }

    wxArrayLong* myAllData = myLayerData->GetAllData();
    if (!myAllData) return false;

    // actual values
    wxArrayLong* myActualSelVal = m_SelectedData.GetSelectedValues();
    wxASSERT(myActualSelVal);

    m_SelectedData.Clear();
    m_SelectedData.AddSelected(myAllData);
    m_SelectedData.Remove(myActualSelVal);

    delete myAllData;
    delete myActualSelVal;

    ReloadProjectLayers(false, true);

    wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
    m_Parent->GetEventHandler()->QueueEvent(evt.Clone());
    return true;
}

void tmLayerManager::CheckGeometryValidity() {
    tmLayerProperties* layerprop = m_TocCtrl->GetSelectionLayer();
    if (layerprop == nullptr) {
        return;
    }

    tmGISData* myLayerData = tmGISData::LoadLayer(layerprop);
    if (myLayerData == nullptr) {
        wxLogError(_("Error loading %s data!"), layerprop->GetNameDisplay());
        return;
    }

    if (myLayerData->IsRaster()) {
        wxLogError(_("This tool isn't working on Raster files"));
        return;
    }

    tmGISDataVector* myLayerDataVector = (tmGISDataVector*)myLayerData;
    OGRGeometry* myGeom = nullptr;
    bool bRestart = true;
    long myOid = wxNOT_FOUND;
    long iNumError = 0;
    long iNumCheck = 0;

    m_SelectedData.SetLayerID(layerprop->GetID());
    m_SelectedData.Clear();
    wxArrayLong myOids;

    while ((myGeom = myLayerDataVector->GetNextGeometry(bRestart, myOid)) != nullptr) {
        bRestart = false;

        bool bError = false;
        iNumCheck++;
        if (!myGeom->IsValid()) {
            bError = true;
            wxLogMessage(_("%ld geometry isn't valid!"), myOid);
        }
        if (!myGeom->IsSimple()) {
            bError = true;
            wxLogMessage(_("%ld geometry isn't simple!"), myOid);
        }

        if (bError) {
            myOids.Add(myOid);
            iNumError++;
        }
        OGRGeometryFactory::destroyGeometry(myGeom);
    }

    m_SelectedData.AddSelected(&myOids);
    ReloadProjectLayers(false, true);
    wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
    m_Parent->GetEventHandler()->QueueEvent(evt.Clone());

    wxMessageBox(wxString::Format(_("%ld error(s) found while checking %ld feature(s)"), iNumError, iNumCheck),
                 _("Geometry validity"));
}

void tmLayerManager::ExportSelectedGeometries(const wxFileName& file) {
    tmLayerProperties* layerprop = m_TocCtrl->GetSelectionLayer();
    if (layerprop == nullptr) {
        return;
    }

    tmGISData* myLayerData = tmGISData::LoadLayer(layerprop);
    if (myLayerData == nullptr) {
        wxLogError(_("Error loading %s data!"), layerprop->GetNameDisplay());
        return;
    }

    if (myLayerData->IsRaster()) {
        wxLogError(_("This tool isn't working on Raster files, select a vector file"));
        return;
    }

    // create the vector file
    tmGISDataVectorSHP myShp;
    if (!myShp.CreateFile(file.GetFullPath(), (int)layerprop->GetSpatialType())) {
        wxLogError(_("Creating shapefile: %s failed!"), file.GetFullPath());
        return;
    }

    tmGISDataVector* myLayerDataVector = (tmGISDataVector*)myLayerData;
    OGRGeometry* myGeom = nullptr;
    bool bRestart = true;
    long myOid = wxNOT_FOUND;
    long iCount = 0;

    while ((myGeom = myLayerDataVector->GetNextGeometry(bRestart, myOid)) != nullptr) {
        bRestart = false;

        if (m_SelectedData.IsSelected(myOid)) {
            myShp.AddGeometry(myGeom, myOid);
            myShp.CloseGeometry();
            iCount++;
        }
        OGRGeometryFactory::destroyGeometry(myGeom);
    }

    wxMessageBox(wxString::Format(_("%ld object(s) exported into: %s"), iCount, file.GetFullName()),
                 _("Selected Geometry Export"));
}

bool tmLayerManager::SelectByOid() {
    //
    // Some checks for
    // - Project open
    // - Layer selected
    //
    if (!IsOK()) {
        wxLogError(_("No project opened, open a project first"));
        return false;
    }
    tmLayerProperties* layerprop = m_TocCtrl->GetSelectionLayer();
    if (!layerprop) {
        wxLogError(_("No layer selected, select a layer in the Table of content"));
        return false;
    }

    if (layerprop->GetSpatialType() >= LAYER_SPATIAL_RASTER) {
        wxLogError(_("Unable to select feature on Raster layer!"));
        return false;
    }

    wxASSERT(m_Parent);
    long myOid = wxGetNumberFromUser(wxEmptyString, _("Feature ID: "), _("Select by feature ID"), 1, 0, 1000000,
                                     m_Parent);

    if (myOid == wxNOT_FOUND) {
        return false;
    }

    // test if feature exists
    tmGISDataVector* myVector = (tmGISDataVector*)tmGISData::LoadLayer(layerprop);
    wxASSERT(myVector);
    OGRFeature* myFeature = myVector->GetFeatureByOID(myOid);
    if (myFeature == nullptr) {
        wxDELETE(myVector);
        int msganswer = wxMessageBox(_("This OID didn't exists!"), _("Invalid OID"), wxOK | wxCANCEL, m_Parent);
        if (msganswer == wxCANCEL) {
            return false;
        } else {
            return SelectByOid();
        }
    }
    OGRFeature::DestroyFeature(myFeature);
    wxDELETE(myVector);

    m_SelectedData.SetSelected(myOid);
    m_SelectedData.SetLayerID(layerprop->GetID());
    ReloadProjectLayers(false, true);

    wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
    m_Parent->GetEventHandler()->QueueEvent(evt.Clone());
    return true;
}

void tmLayerManager::OnZoomRectangleIn(wxCommandEvent& event) {
    // computing best ratio between actuall windows size
    // and selection size
    wxRect* mySelectedRect = (wxRect*)event.GetClientData();
    double dSmaller = m_Scale.GetBestDivFactor(*mySelectedRect);

    // calcul the new reduced window size (px)
    wxRect myActualExtent = m_Scale.GetWindowExtent();
    wxRect myNewWndExtent(0, 0, 0, 0);
    double dwidth = (1.0 * myActualExtent.GetWidth()) / dSmaller;
    double dheight = (1.0 * myActualExtent.GetHeight()) / dSmaller;

    myNewWndExtent.SetWidth(wxRound(dwidth));
    myNewWndExtent.SetHeight(wxRound(dheight));

    // modify the real window extent
    m_Scale.ComputeNewRealZoomExtent(myNewWndExtent, wxPoint(mySelectedRect->GetX(), mySelectedRect->GetY()));

    // reload data
    ReloadProjectLayers(FALSE);
    delete mySelectedRect;

    _ZoomChanged();
}

void tmLayerManager::OnZoomRectangleOut(wxCommandEvent& event) {
    wxRect* mySelectedRect = (wxRect*)event.GetClientData();

    // compute ratio between actual window size and selection
    double dBigger = m_Scale.GetBestDivFactor(*mySelectedRect);

    // calcul the new bigger window size (px)
    wxRect myActualExtent = m_Scale.GetWindowExtent();
    wxRect myNewWndExtent(0, 0, 0, 0);
    double dwidth = ((double)myActualExtent.GetWidth()) * dBigger;
    double dheight = ((double)myActualExtent.GetHeight()) * dBigger;

    myNewWndExtent.SetWidth(wxRound(dwidth));
    myNewWndExtent.SetHeight(wxRound(dheight));

    // modify the real window extent
    m_Scale.ComputeNewRealZoomExtent(myNewWndExtent, wxPoint(-mySelectedRect->GetX(), -mySelectedRect->GetY()));

    // reload data
    ReloadProjectLayers(FALSE);
    delete mySelectedRect;
    _ZoomChanged();
}

void tmLayerManager::OnZoomToFeature(wxCommandEvent& event) {
    long myOid = event.GetExtraLong();

    // getting geometry
    long myLayerID = m_SelectedData.GetSelectedLayer();
    tmLayerProperties* myLayerProp = m_TocCtrl->GetLayerById(myLayerID);

    tmGISDataVector* myVectorData = (tmGISDataVector*)tmGISData::LoadLayer(myLayerProp);
    wxASSERT(myVectorData);

    OGRFeature* myFeature = myVectorData->GetFeatureByOID(myOid);
    wxASSERT(myFeature);
    delete myVectorData;

    OGREnvelope myEnveloppe;
    myFeature->GetGeometryRef()->getEnvelope(&myEnveloppe);
    OGRFeature::DestroyFeature(myFeature);

    // zooming
    vrRealRect myRect;
    myRect.SetLeftTop(wxPoint2DDouble(myEnveloppe.MinX, myEnveloppe.MaxY));
    myRect.SetRightBottom(wxPoint2DDouble(myEnveloppe.MaxX, myEnveloppe.MinY));
    if (!m_Scale.ZoomViewTo(myRect)) {
        wxLogError(_T("Error zooming to feature n\u00B0%ld"), myOid);
        return;
    }

    ReloadProjectLayers(FALSE);
    _ZoomChanged();
}

void tmLayerManager::OnMoveToFeature(wxCommandEvent& event) {
    long myOid = event.GetExtraLong();

    // getting geometry
    long myLayerID = m_SelectedData.GetSelectedLayer();
    tmLayerProperties* myLayerProp = m_TocCtrl->GetLayerById(myLayerID);

    tmGISDataVector* myVectorData = (tmGISDataVector*)tmGISData::LoadLayer(myLayerProp);
    wxASSERT(myVectorData);

    OGRFeature* myFeature = myVectorData->GetFeatureByOID(myOid);
    wxASSERT(myFeature);
    delete myVectorData;

    OGREnvelope myEnveloppe;
    myFeature->GetGeometryRef()->getEnvelope(&myEnveloppe);
    OGRFeature::DestroyFeature(myFeature);

    // moving
    vrRealRect myRect;
    myRect.SetLeftTop(wxPoint2DDouble(myEnveloppe.MinX, myEnveloppe.MaxY));
    myRect.SetRightBottom(wxPoint2DDouble(myEnveloppe.MaxX, myEnveloppe.MinY));
    if (!m_Scale.MoveViewTo(myRect)) {
        wxLogError(_T("Error moving to feature %ld"), myOid);
        return;
    }

    ReloadProjectLayers(FALSE);
}

void tmLayerManager::OnPanFinished(wxCommandEvent& event) {
    wxPoint* myNewPosPx = (wxPoint*)event.GetClientData();

    // change coordinates
    m_Scale.ComputeNewRealPanExtent(*myNewPosPx);
    ReloadProjectLayers(FALSE);

    delete myNewPosPx;
    _ZoomChanged();
}

/***************************************************************************/ /**
  @brief Called when selection was made
  @details This is called by the #tmRenderer when a selection (either by point or
  rectangle) was made
  @param event Contain data about the status of the shift key and the rectangle
  size.
  - Use event.GetInt() for getting the status of the shift key
  - Use event.GetClientObject() for getting the wxRect.
  @author Lucien Schreiber (c) CREALP 2008
  @date 29 October 2008
  *******************************************************************************/
void tmLayerManager::OnSelection(wxCommandEvent& event) {
    bool IsShiftDown = static_cast<bool>(event.GetInt());
    wxRect* mySelectedRect = (wxRect*)event.GetClientData();

    if (SelectedSearch(*mySelectedRect, IsShiftDown)) {
        ReloadProjectLayers(false, false);
    }

    wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
    m_Parent->GetEventHandler()->QueueEvent(evt.Clone());

    delete mySelectedRect;
}

/***************************************************************************/ /**
  @brief Is the layer manager ready with some project
  @return  TRUE if some layers are loaded into the project, FALSE otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 24 July 2008
  *******************************************************************************/
bool tmLayerManager::IsOK() {
    // ensure that TOC ctrl isn't empty
    if (m_TocCtrl->GetCountLayers() == 0) {
        return false;
    }
    return true;
}

/***************************************************************************/ /**
  @brief Load all layers (non threaded)
  @details This functions does closely the same that the
  #ReloadProjectLayers() one but isn't threaded and is called during
  project opening.
  @author Lucien Schreiber (c) CREALP 2008
  @date 24 July 2008
  *******************************************************************************/
bool tmLayerManager::LoadProjectLayers() {
    m_RotationLayerNames.Clear();

    // enable logging
    tmGISData::EnableLogging(true);
    tmDrawer::EnableLogging(true);

    if (!IsOK()) {
        return FALSE;
    }

    // invalidate bitmap
    m_GISRenderer->SetBitmapStatus();
    m_GISRenderer->InitSize();
    CreateEmptyBitmap(wxSize(m_Scale.GetWindowExtent().GetWidth(), m_Scale.GetWindowExtent().GetHeight()));

    int iRead = ReadLayerExtent(true, true);
    wxLogDebug(_T("%d layer(s) read"), iRead);

    // test validity of layers extent. If no extent is
    // specified (like no data displayed) return
    if (m_Scale.IsLayerExtentValid()) {
        m_Scale.ComputeMaxExtent();
        m_Drawer.InitDrawer(m_Bitmap, &m_Scale, m_Scale.GetWindowExtentReal());

        // iterate for drawing layers
        ReadLayerDraw();

        // update scale
        m_ScaleCtrl->SetValueScale(m_Scale.GetActualScale());
        m_StatusBar->SetStatusText(_("Visible Width: ") + m_Scale.GetVisibleWidthText(), 1);

        m_Drawer.DrawExtentIntoBitmap(2, *wxRED);
    }

    // set active bitmap
    m_GISRenderer->SetBitmapStatus(m_Bitmap);
    if (m_Bitmap) {
        delete m_Bitmap;
        m_Bitmap = nullptr;
    }

    m_GISRenderer->Refresh();

    // previous zoom
    m_ZoomManager.Clear();
    _ZoomChanged();
    return true;
}

bool tmLayerManager::ReloadProjectLayers(bool bFullExtent, bool bInvalidateFullExt) {
    m_RotationLayerNames.Clear();

    // invalidate bitmap
    m_GISRenderer->SetBitmapStatus();
    CreateEmptyBitmap(wxSize(m_Scale.GetWindowExtent().GetWidth(), m_Scale.GetWindowExtent().GetHeight()));

    // invalidate max_extent
    if (bInvalidateFullExt) {
        m_Scale.SetMaxLayersExtent(tmRealRect(0, 0, 0, 0));
    }

    int iRead = ReadLayerExtent(true);
    if (iRead == -1) {
        return false;
    }

    if (m_Scale.IsLayerExtentValid()) {
        // compute max extent if required by option
        if (bFullExtent) {
            m_Scale.ComputeMaxExtent();
        }

        // read layers for drawing
        m_Drawer.InitDrawer(m_Bitmap, &m_Scale, m_Scale.GetWindowExtentReal());
        int iNbLayersDraw = ReadLayerDraw();
        if (iNbLayersDraw == -1) {
            return false;
        }

        // update scale
        m_ScaleCtrl->SetValueScale(m_Scale.GetActualScale());
        m_StatusBar->SetStatusText(_("Visible Width: ") + m_Scale.GetVisibleWidthText(), 1);
    }

    // set active bitmap
    m_GISRenderer->SetBitmapStatus(m_Bitmap);
    wxDELETE(m_Bitmap);

    m_GISRenderer->Refresh();
    m_GISRenderer->Update();
    return true;
}

/***************************************************************************/ /**
  @brief Creating an empty white bitmap
  @param size the size (in pixels) of the new bitmap to create
  @author Lucien Schreiber (c) CREALP 2008
  @date 24 July 2008
  *******************************************************************************/
void tmLayerManager::CreateEmptyBitmap(const wxSize& size) {
    if (m_Bitmap != nullptr) {
        delete m_Bitmap;
        m_Bitmap = nullptr;
    }

    m_Bitmap = new wxBitmap(size.GetWidth(), size.GetHeight());

    // ensure that the bitmap is white (win and linux)
    wxMemoryDC dc;
    dc.SelectObject(*m_Bitmap);
    dc.SetBackground(wxBrush(*wxWHITE_BRUSH));
    dc.Clear();
}

/***************************************************************************/ /**
  @brief Iterate all layers for extent
  @details This function iterates through all layers, loading layers and their
  extent.
  @param loginfo TRUE if the function should log some info about what is done.
  <B>When used in thread, this parameter has to be set to false</B>
  @return  the number of layers really read, a. k. a. number of visible layers
  @author Lucien Schreiber (c) CREALP 2008
  @date 09 September 2008
  *******************************************************************************/
int tmLayerManager::ReadLayerExtent(bool loginfo, bool buildpyramids) {
    // iterate throught all layers
    int iRank = 0;
    int iReaded = 0;

    tmLayerProperties* pLayerProp = nullptr;
    tmRealRect myExtent(0, 0, 0, 0);

    // prepare loading of MySQL data
    tmGISDataVectorMYSQL::SetDataBaseHandle(m_DB);
    while (1) {
        if (iRank == 0) {
            pLayerProp = m_TocCtrl->IterateLayers(TRUE);
        } else {
            pLayerProp = m_TocCtrl->IterateLayers(FALSE);
        }

        if (!pLayerProp) break;

        if (pLayerProp->IsVisible()) {
            if (pLayerProp->GetName().Exists() || pLayerProp->GetType() < TOC_NAME_NOT_GENERIC) {
                // loading data
                tmGISData* layerData = tmGISData::LoadLayer(pLayerProp);

                if (layerData) {
                    // build pyramids if needed and only during project opening
                    if (buildpyramids) {
                        _BuildOverviewsIfNeeded(layerData, pLayerProp->GetNameDisplay());
                    }
                    // ignore extend for web rasters because they are too big
                    if (pLayerProp->GetType() != TOC_NAME_WEB) {
                        myExtent = layerData->GetMinimalBoundingRectangle();
                        m_Scale.SetMaxLayersExtentAsExisting(myExtent);
                    }
                    iReaded++;
                }
                wxDELETE(layerData);
            }
        }

        iRank++;
    }
    return iReaded;
}

/***************************************************************************/ /**
  @brief Iterate all visible layers for drawing them
  @return  Number of layers really drawn (visible layers)
  @author Lucien Schreiber (c) CREALP 2008
  @date 09 September 2008
  *******************************************************************************/
int tmLayerManager::ReadLayerDraw() {
    int iReaded = 0;
    tmGISDataVectorMYSQL::SetDataBaseHandle(m_DB);
    bool reset = true;
    while (true) {
        auto pLayerProp = m_TocCtrl->IterateLayers(reset);
        reset = false;

        if (!pLayerProp) {
            break;
        }

        if (!pLayerProp->IsVisible()) {
            continue;
        }

        if (!m_TocCtrl->IsAllParentLayerVisible(pLayerProp->GetID())) {
            continue;
        }

        tmGISData* layerData = tmGISData::LoadLayer(pLayerProp);
        if (layerData == nullptr) {
            continue;
        }

        tmCoordConvert* myCoordConv = new tmCoordConvert(m_MemoryPrjRef->m_PrjProjType);
        layerData->SetCoordConvert(myCoordConv);

        m_Drawer.Draw(pLayerProp, layerData);
        iReaded++;
        wxDELETE(layerData);
    }
    return iReaded;
}

void tmLayerManager::InitScaleCtrlList() {
    wxASSERT(m_DB);
    wxArrayLong myValues;
    long myDBIndex = -1;
    bool first = true;
    while (true) {
        long myScale = m_DB->GetNextScaleValue(myDBIndex, first);
        first = false;
        if (myScale == wxNOT_FOUND) {
            break;
        }
        // adding scale in the list
        myValues.Add(myScale);
    }
    // send message
    m_ScaleCtrl->InitScaleFromDatabase(myValues);
}

void tmLayerManager::_ZoomChanged() {
    tmZoomExtent myActualExtent;
    myActualExtent.m_ZoomFactor = m_Scale.GetPixelSize();
    myActualExtent.m_TopLeftPosition = m_Scale.GetTopLeftValue();
    if (!myActualExtent.IsOk()) {
        wxLogMessage(_("Incorrect zoom extent set!"));
        return;
    }
    m_ZoomManager.Add(myActualExtent);
}

bool tmLayerManager::ZoomPrevious() {
    tmZoomExtent myPrevExtent;
    if (!m_ZoomManager.GetPrevious(myPrevExtent)) {
        return false;
    }

    wxASSERT(myPrevExtent.IsOk());

    m_Scale.ComputePrevZoomExtent(myPrevExtent.m_ZoomFactor, myPrevExtent.m_TopLeftPosition);
    ReloadProjectLayers(false, false);
    return true;
}

bool tmLayerManager::HasZoomPrevious() {
    bool bReturn = false;
    if (m_ZoomManager.GetCount() > 0) {
        bReturn = true;
    }
    return bReturn;
}
