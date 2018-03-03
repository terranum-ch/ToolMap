/***************************************************************************
							project_def.h
						Project definition dialog
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

#include "project_def_dlg.h"


IMPLEMENT_DYNAMIC_CLASS(ProjectDefDLG, wxDialog)


BEGIN_EVENT_TABLE(ProjectDefDLG, wxDialog)
                EVT_FLATBUTTON(ID_DLGPD_SPAT_MDL_ADD, ProjectDefDLG::OnAddLayer)
                EVT_FLATBUTTON(ID_DLGPD_SPAT_MDL_DEL, ProjectDefDLG::OnRemoveLayer)
                EVT_IDLE(ProjectDefDLG::OnIdleWait)
END_EVENT_TABLE()


void ProjectDefDLG::OnAddLayer(wxCommandEvent &event)
{
    m_DlgPd_Stat_Model_List->AddItem();
}

void ProjectDefDLG::OnRemoveLayer(wxCommandEvent &event)
{
    // remove the object from the array
    m_DlgPd_Stat_Model_List->DeleteItem();
}


void ProjectDefDLG::OnIdleWait(wxIdleEvent &event)
{
    if (!CheckIdleRules()) {
        m_DlgPd_Button_Ok->Enable(FALSE);
    } else
        m_DlgPd_Button_Ok->Enable(TRUE);

}

bool ProjectDefDLG::CheckIdleRules()
{
    // check the rules to be verified during the Idle time

    // contain minimum one layer
    if (m_DlgPd_Stat_Model_List->GetItemCount() < 1)
        return FALSE;

    return TRUE; // check passed
}

ProjectDefDLG::ProjectDefDLG()
{
    Init();
}

ProjectDefDLG::ProjectDefDLG(wxWindow *parent,
                             PrjDefMemManage *myPrjDefinition,
                             bool isEditingMode,
                             wxWindowID id, const wxString &caption,
                             const wxPoint &pos, const wxSize &size, long style)
{
    Init();
    m_bIsModeEditing = isEditingMode;


    // create UI
    Create(parent, id, caption, pos, size, style);

    m_pPrjDefinition = myPrjDefinition;
    // Pass Address of Project Definition to the list
    m_DlgPd_Stat_Model_List->PassPrjDefToList(m_pPrjDefinition);


}


bool
ProjectDefDLG::Create(wxWindow *parent, wxWindowID id, const wxString &caption, const wxPoint &pos, const wxSize &size,
                      long style)
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create(parent, id, caption, pos, size, style);

    CreateControls();
    if (GetSizer()) {
        GetSizer()->SetSizeHints(this);
    }
    Centre();

    return true;
}


ProjectDefDLG::~ProjectDefDLG()
{
}


void ProjectDefDLG::Init()
{
    m_DlgPd_Panel_Proj = NULL;
    m_DlgPd_Proj_Unit = NULL;
    m_DlgPd_Proj_Projection = NULL;
    m_DlgPd_Proj_Author = NULL;
    m_DlgPd_Proj_Comment = NULL;
    m_DlgPd_Panel_Spatial = NULL;
    m_DlgPd_Stat_Model_List = NULL;
    m_DlgPd_Spat_Mdl_Add = NULL;
    m_DljPd_Spat_Mdl_Del = NULL;
    m_DlgPd_Button_Ok = NULL;
    m_DlgPd_Status = NULL;

    m_bIsModeEditing = FALSE;
    m_pPrjDefinition = NULL;
}


void ProjectDefDLG::DisableControlsForEdition()
{
    //m_DlgPd_Proj_Projection->Enable(FALSE);
    //m_DlgPd_Proj_Unit->Enable(FALSE);
    m_DlgPd_Button_Ok->SetLabel(_("Update Project"));
}


void ProjectDefDLG::CreateControls()
{
    ProjectDefDLG *itemDialog1 = this;

    wxBoxSizer *itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxNotebook *itemNotebook3 = new wxNotebook(itemDialog1, ID_DLGPD_NOTEBOOK, wxDefaultPosition, wxDefaultSize,
                                               wxBK_DEFAULT);

    m_DlgPd_Panel_Proj = new wxPanel(itemNotebook3, ID_DLGPD_PANEL_PROJ, wxDefaultPosition, wxDefaultSize,
                                     wxTAB_TRAVERSAL);
    wxBoxSizer *itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    m_DlgPd_Panel_Proj->SetSizer(itemBoxSizer5);

    wxFlexGridSizer *itemFlexGridSizer6 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer6->AddGrowableCol(1);
    itemBoxSizer5->Add(itemFlexGridSizer6, 0, wxGROW | wxALL, 5);

    wxStaticText *itemStaticText11 = new wxStaticText(m_DlgPd_Panel_Proj, wxID_STATIC, _("Units"), wxDefaultPosition,
                                                      wxDefaultSize, 0);
    itemFlexGridSizer6->Add(itemStaticText11, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    m_DlgPd_Proj_Unit = new wxChoice(m_DlgPd_Panel_Proj, ID_DLGPD_PROJ_UNIT, wxDefaultPosition, wxDefaultSize,
                                     PRJDEF_UNIT_TYPE_NUMBER, PRJDEF_UNIT_TYPE_STRING);
    //m_DlgPd_Proj_Unit->SetSelection(UNIT_METERS);
    //m_DlgPd_Proj_Unit->Enable(FALSE);
    itemFlexGridSizer6->Add(m_DlgPd_Proj_Unit, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticText *itemStaticText13 = new wxStaticText(m_DlgPd_Panel_Proj, wxID_STATIC, _("Projection :"),
                                                      wxDefaultPosition, wxDefaultSize, 0);
    itemFlexGridSizer6->Add(itemStaticText13, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    m_DlgPd_Proj_Projection = new wxChoice(m_DlgPd_Panel_Proj, ID_DLGPD_PROJ_PROJECTION, wxDefaultPosition,
                                           wxDefaultSize, PRJDEF_PROJ_TYPE_NUMBER, PRJDEF_PROJ_TYPE_STRING);
    m_DlgPd_Proj_Projection->SetSelection(0);
    m_DlgPd_Proj_Projection->Enable(true);
    itemFlexGridSizer6->Add(m_DlgPd_Proj_Projection, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticBox *itemStaticBoxSizer15Static = new wxStaticBox(m_DlgPd_Panel_Proj, wxID_ANY, _("Summary"));
    wxStaticBoxSizer *itemStaticBoxSizer15 = new wxStaticBoxSizer(itemStaticBoxSizer15Static, wxVERTICAL);
    itemBoxSizer5->Add(itemStaticBoxSizer15, 1, wxGROW | wxALL, 5);
    wxFlexGridSizer *itemFlexGridSizer16 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer16->AddGrowableRow(1);
    itemFlexGridSizer16->AddGrowableCol(1);
    itemStaticBoxSizer15->Add(itemFlexGridSizer16, 1, wxGROW | wxALL, 5);
    wxStaticText *itemStaticText17 = new wxStaticText(m_DlgPd_Panel_Proj, wxID_STATIC,
                                                      _("Author :"), wxDefaultPosition, wxDefaultSize, 0);
    itemFlexGridSizer16->Add(itemStaticText17, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    m_DlgPd_Proj_Author = new wxTextCtrl(m_DlgPd_Panel_Proj, ID_DLGPD_PROJ_AUTHOR, _T(""),
                                         wxDefaultPosition, wxSize(300, -1), 0);
    m_DlgPd_Proj_Author->SetMaxLength(254);
    itemFlexGridSizer16->Add(m_DlgPd_Proj_Author, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticText *itemStaticText19 = new wxStaticText(m_DlgPd_Panel_Proj, wxID_STATIC, _("Comments :"),
                                                      wxDefaultPosition, wxDefaultSize, 0);
    itemFlexGridSizer16->Add(itemStaticText19, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 5);

    m_DlgPd_Proj_Comment = new wxTextCtrl(m_DlgPd_Panel_Proj, ID_DLGPD_PROJ_COMMENT, _T(""), wxDefaultPosition,
                                          wxDefaultSize, wxTE_MULTILINE);
    itemFlexGridSizer16->Add(m_DlgPd_Proj_Comment, 1, wxGROW | wxALL, 5);

    itemNotebook3->AddPage(m_DlgPd_Panel_Proj, _("Project properties"));

    m_DlgPd_Panel_Spatial = new wxPanel(itemNotebook3, ID_DLGPD_PANEL_SPAT, wxDefaultPosition, wxDefaultSize,
                                        wxTAB_TRAVERSAL);
    wxBoxSizer *itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
    m_DlgPd_Panel_Spatial->SetSizer(itemBoxSizer22);

    m_DlgPd_Stat_Model_List = new ProjectDefList(m_DlgPd_Panel_Spatial, ID_DLGPD_SPAT_MODEL_LIST, wxSize(-1, 150),
                                                 this);
    itemBoxSizer22->Add(m_DlgPd_Stat_Model_List, 1, wxGROW | wxALL, 5);

    wxBoxSizer *itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer22->Add(itemBoxSizer24, 0, wxGROW | wxALL, 5);
    m_DlgPd_Spat_Mdl_Add = new wxFlatButton(m_DlgPd_Panel_Spatial, ID_DLGPD_SPAT_MDL_ADD, wxFLATBUTTON_TEXT_ADD);
    //m_DlgPd_Spat_Mdl_Add->SetValue(false);
    itemBoxSizer24->Add(m_DlgPd_Spat_Mdl_Add, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, 5);

    m_DljPd_Spat_Mdl_Del = new wxFlatButton(m_DlgPd_Panel_Spatial, ID_DLGPD_SPAT_MDL_DEL, wxFLATBUTTON_TEXT_REMOVE);
    //m_DljPd_Spat_Mdl_Del->SetValue(false);
    itemBoxSizer24->Add(m_DljPd_Spat_Mdl_Del, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, 5);

    itemNotebook3->AddPage(m_DlgPd_Panel_Spatial, _("Spatial model"));

    itemBoxSizer2->Add(itemNotebook3, 1, wxGROW | wxALL, 5);

    wxStaticLine *itemStaticLine27 = new wxStaticLine(itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize,
                                                      wxLI_HORIZONTAL);
    itemBoxSizer2->Add(itemStaticLine27, 0, wxGROW | wxALL, 5);

    wxStdDialogButtonSizer *itemStdDialogButtonSizer28 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer28, 0, wxALIGN_RIGHT | wxALL, 5);
    m_DlgPd_Button_Ok = new wxButton(itemDialog1, wxID_OK, _("&Save"), wxDefaultPosition, wxDefaultSize, 0);
    itemStdDialogButtonSizer28->AddButton(m_DlgPd_Button_Ok);

    wxButton *itemButton30 = new wxButton(itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0);
    itemStdDialogButtonSizer28->AddButton(itemButton30);

    itemStdDialogButtonSizer28->Realize();


    // status bar
    m_DlgPd_Status = new wxStatusBar(itemDialog1, wxID_ANY, wxST_SIZEGRIP | wxNO_BORDER);
    m_DlgPd_Status->SetFieldsCount(2);
    //m_DlgPd_Status->SetStatusText(_T(""), 0);
    //m_DlgPd_Status->SetStatusText(_(""), 1);
    //int itemStatusBar13Widths[2];
    //itemStatusBar13Widths[0] = -2;
    //itemStatusBar13Widths[1] = -1;
    //itemStatusBar13->SetStatusWidths(2, itemStatusBar13Widths);
    itemBoxSizer2->Add(m_DlgPd_Status, 0, wxALL | wxGROW, 0);

    // pass the status bar to the list
    m_DlgPd_Stat_Model_List->SetStatusBar(m_DlgPd_Status);

    // if editing mode is on, we disable some controls
    if (m_bIsModeEditing)
        DisableControlsForEdition();


}


void ProjectDefDLG::SetNotebook(int notebooknumber)
{
    wxNotebook *myNotebook = (wxNotebook *) FindWindowById(ID_DLGPD_NOTEBOOK, this);
    wxASSERT(myNotebook);
    myNotebook->SetSelection(notebooknumber);
}


bool ProjectDefDLG::TransferDataFromWindow()
{
    m_pPrjDefinition->m_PrjAuthors = m_DlgPd_Proj_Author->GetValue();
    m_pPrjDefinition->m_PrjSummary = m_DlgPd_Proj_Comment->GetValue();
    m_pPrjDefinition->m_PrjUnitType = static_cast<PRJDEF_UNIT_TYPE> (m_DlgPd_Proj_Unit->GetSelection());
    m_pPrjDefinition->m_PrjProjType = static_cast<PRJDEF_PROJ_TYPE> (m_DlgPd_Proj_Projection->GetSelection());
    return TRUE;
}

bool ProjectDefDLG::TransferDataToWindow()
{
    wxArrayString myLayerList;


    m_DlgPd_Proj_Unit->SetSelection(m_pPrjDefinition->m_PrjUnitType);
    m_DlgPd_Proj_Projection->SetSelection(m_pPrjDefinition->m_PrjProjType);
    m_DlgPd_Proj_Unit->SetSelection(m_pPrjDefinition->m_PrjUnitType);

    // Get data from the DB and parse them into controls
    // only if in editing mode
    if (m_bIsModeEditing) {
        wxASSERT (m_pPrjDefinition);

        m_DlgPd_Proj_Author->SetValue(m_pPrjDefinition->m_PrjAuthors);
        m_DlgPd_Proj_Comment->SetValue(m_pPrjDefinition->m_PrjSummary);

        // fill the layers list
        for (int i = 0; i < m_pPrjDefinition->GetCountLayers(); i++) {
            ProjectDefMemoryLayers *myObjLayers = m_pPrjDefinition->GetNextLayer();

            // fit things returned in the list
            myLayerList.Add(myObjLayers->m_LayerName);
            myLayerList.Add(PRJDEF_LAYERS_TYPE_STRING[myObjLayers->m_LayerType]);
            m_DlgPd_Stat_Model_List->EditDataToList(myLayerList);
            myLayerList.Clear();
        }

        // update status bar
        m_DlgPd_Stat_Model_List->UpdateStatus(STATUS_FIELD_ITEM_COUNT);

    }
    return TRUE;
}


/************************ PROJECT DEF LIST **********************************/
ProjectDefList::ProjectDefList(wxWindow *parent, wxWindowID id, wxSize size, ProjectDefDLG *myParentDlg)
        : ListGenReportWithStatus(parent, id, NULL)
{
    m_bIsModeEditing = myParentDlg->IsEditMode();
    // Create columns
    wxArrayString myColNames;
    wxArrayInt myColsWidths;

    myColNames.Add(_("Name"));
    myColNames.Add(_("Type"));

    myColsWidths.Add(250);
    myColsWidths.Add(80);

    CreateColumns(&myColNames, &myColsWidths);

    m_ChoiceToChange = NULL;

    // create an array for storing spatial thems
    //m_LayersArray = new PrjMemLayersArray();
    m_LayersArray = NULL;

    m_LayersDialog = NULL;

    SetTextFields(_("Number of layer(s) : %d"), _("%d layer(s) selected"));
    m_bAscending[0] = TRUE;
    m_bAscending[1] = TRUE;

    m_pPrjDefinition = NULL;
    m_LayersObj = NULL;
}


ProjectDefList::~ProjectDefList()
{
    //if (!m_LayersArray->IsEmpty())
//		m_LayersArray->Clear();
//	delete m_LayersArray;
}


void ProjectDefList::BeforeAdding()
{

    // create and set the dialog here
    ProjectDefLayersDlg *myLayerDialog = new ProjectDefLayersDlg(this,
                                                                 m_pPrjDefinition,
                                                                 m_bIsModeEditing,
                                                                 m_bIsModeEditing);
    //wxLogDebug(_T("Creating Thematic Layer Dialog"));
    SetDialog(myLayerDialog);

    // creating a new layer, if not used, delete it during
    // After adding.
    m_LayersObj = m_pPrjDefinition->AddLayer();

    // pass container to the dialog for using Transfert data to and from
    // dialog.
    ((ProjectDefLayersDlg *) m_pDialog)->SetMemoryLayersObject(m_LayersObj);

}


void ProjectDefList::AfterAdding(bool bRealyAddItem)
{
    wxArrayString myListValues;

    if (bRealyAddItem) {
        // data allready added to the array

        // add data to the list
        // prepare data for list representation
        myListValues.Add(m_LayersObj->m_LayerName);
        myListValues.Add(PRJDEF_LAYERS_TYPE_STRING[m_LayersObj->m_LayerType]);
        EditDataToList(myListValues);

    } else
        m_pPrjDefinition->RemoveLayer(); // remove last unused layer object

    // delete the dialog here
    //wxLogDebug(_T("Destroying Thematic Layer Dialog"));
    //delete m_pDialog;

}


void ProjectDefList::BeforeEditing()
{
    // create the dialog
    m_LayersDialog = new ProjectDefLayersDlg(this, m_pPrjDefinition, m_bIsModeEditing);
    //wxLogDebug(_T("Creating Thematic Layer Dialog"));
    SetDialog(m_LayersDialog);

    // get selected item from the list
    long mySelectedListItem = GetSelectedItem();
    wxString myLayerName = GetItemColText(mySelectedListItem, 0);


    m_LayersObj = m_pPrjDefinition->FindLayer(myLayerName);

//	// find item selected and then call a new Dialog
//	// for editing the existing Field
//	int iItemIndex = FindObjInLayersArray(this, m_LayersArray);
    if (m_LayersObj != NULL) {
        // transfert the data obj to the dialog, data will be
        // filled during DataTransfer...
        ((ProjectDefLayersDlg *) m_pDialog)->SetMemoryLayersObject(m_LayersObj);
    }

}

void ProjectDefList::AfterEditing(bool bRealyEdited)
{
    wxArrayString myListValues;

    if (bRealyEdited) {
        // data modified is allready stored in the project definition

        // prepare data for list representation
        myListValues.Add(m_LayersObj->m_LayerName);
        myListValues.Add(PRJDEF_LAYERS_TYPE_STRING[m_LayersObj->m_LayerType]);
        EditDataToList(myListValues, GetSelectedItem());

    }

    // delete dialog
    //wxLogDebug(_T("Destroying Thematic Layer Dialog"));
    //delete m_LayersDialog;
}


void ProjectDefList::BeforeDeleting()
{
    wxString myLayerName;
    ProjectDefMemoryLayers *layer = NULL;
    //int iDelete = 0;
    // remove item from array before removing it from the list
    // because of the unknown position of item (may have been moved)
    // if a corresponding item was found, remove it from the array

    // get selected items from the list
    wxArrayLong mySelectedListItems;
    int iNbSelectedItems = GetAllSelectedItem(mySelectedListItems);
    for (int i = 0; i < iNbSelectedItems; i++) {
        myLayerName = GetItemColText(mySelectedListItems[i], 0);

        // if we are in editing mode we must save the items to delete
        // in the deletelayer array.
        if (m_bIsModeEditing == TRUE) {
            layer = m_pPrjDefinition->FindLayer(myLayerName);
            if (layer && layer->m_LayerID > 0) {
                wxLogDebug(_T("Marqued layer for deleting : %d"), layer->m_LayerID);
                m_pPrjDefinition->m_StoreDeleteLayers.Add(layer->m_LayerID);
            }
        }

        if (!m_pPrjDefinition->RemoveLayer(myLayerName)) {
            wxLogDebug(_T("Layer %s not removed."), myLayerName);
        }
    }

}


/***************************************************************************//**
 @brief Called when user click on the column header
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 June 2008
 *******************************************************************************/
void ProjectDefList::OnSortColumns(wxListEvent &event)
{
    int myTypeCol[2] = {FIELD_STRING, FIELD_STRING};
    int iCol = event.m_col;

    SortListItem(iCol, 0, -1, myTypeCol[iCol], m_bAscending[iCol]);

    // invert ascending : descending
    m_bAscending[iCol] == TRUE ? m_bAscending[iCol] = FALSE : m_bAscending[iCol] = TRUE;
}




