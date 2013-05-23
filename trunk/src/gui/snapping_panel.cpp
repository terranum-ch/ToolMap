/***************************************************************************
 snapping_panel.cpp
 Deals with the snapping panel, used for setting the
 snapping for layers
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

#include "snapping_panel.h"
#include "../gis/tmrenderer.h"
#include "../database/database_tm.h"	// for database support
#include "../core/datalistreportctrl.h"











tmSnapping_DLG::tmSnapping_DLG( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_LayerNameCtrl = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LayerNameCtrl->Wrap( -1 );
	bSizer1->Add( m_LayerNameCtrl, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("snapping") ), wxVERTICAL );
	
	m_SnapNoneCtrl = new wxRadioButton( this, wxID_ANY, _("None"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	//m_SnapNoneCtrl->SetValue( true );
	sbSizer1->Add( m_SnapNoneCtrl, 0, wxALL, 5 );
	
	m_SnapBeginEndCtrl = new wxRadioButton( this, wxID_ANY, _("Begin / End vertex"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_SnapBeginEndCtrl, 0, wxALL, 5 );
	
	m_SnapAllCtrl = new wxRadioButton( this, wxID_ANY, _("All vertex"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_SnapAllCtrl, 0, wxALL, 5 );
	
	
	bSizer1->Add( sbSizer1, 0, wxEXPAND|wxALL, 5 );
	
	wxStdDialogButtonSizer* m_sdbSizer1;
	wxButton* m_sdbSizer1Save;
	wxButton* m_sdbSizer1Cancel;
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1Save = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1Save );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	
	bSizer1->Add( m_sdbSizer1, 1, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	this->Centre( wxBOTH );
    
    m_SnapStatus = tmSNAPPING_OFF;
    m_LayerName = wxEmptyString;
}



tmSnapping_DLG::~tmSnapping_DLG(){
}



bool tmSnapping_DLG::TransferDataToWindow(){
    m_LayerNameCtrl->SetLabel(m_LayerName);
    m_SnapNoneCtrl->SetValue(true);
    if (m_SnapStatus == tmSNAPPING_BEGIN_END) {
        m_SnapBeginEndCtrl->SetValue(true);
    }
    
    if (m_SnapStatus == tmSNAPPING_VERTEX) {
        m_SnapAllCtrl->SetValue(true);
    }
    
    return true;
}


bool tmSnapping_DLG::TransferDataFromWindow(){
    m_SnapStatus = tmSNAPPING_OFF;
    
    if (m_SnapBeginEndCtrl->GetValue() == true) {
        m_SnapStatus = tmSNAPPING_BEGIN_END;
    }
    
    if (m_SnapAllCtrl->GetValue() == true) {
        m_SnapStatus = tmSNAPPING_VERTEX;
    }
    return true;
}










// EVENT TABLE
BEGIN_EVENT_TABLE( Snapping_PANEL, ManagedAuiWnd )
	EVT_SPINCTRL( ID_SNAP_TOLERENCE_TXT, Snapping_PANEL::OnUpdateTolerence )
	EVT_MENU ( ID_SNAP_ADD, Snapping_PANEL::OnAddSnapping )
	EVT_MENU ( ID_SNAP_REMOVE, Snapping_PANEL::OnRemoveSnapping )
END_EVENT_TABLE()



/***************************************************************************//**
 @brief Snapping panel constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
Snapping_PANEL::Snapping_PANEL( wxWindow* parent, wxWindowID id, wxAuiManager * auimanager)
: ManagedAuiWnd(auimanager)
{
    m_TolerenceCtrl = NULL;
	m_ListCtrl = NULL;
	m_pDB = NULL;
    m_Renderer = NULL;
    m_SnappingMemory.Clear();
	m_ParentEvt = parent;
	
    // TODO: Check if this is needed ?
    m_ParentEvt->PushEventHandler(this);
	
	wxPanel * myPanel = _CreateControls();
	m_PaneInfo.Name(SNAPPING_PANEL_TITLE);
	m_PaneInfo.Caption(SNAPPING_PANEL_TITLE);
	m_PaneInfo.Float();
	m_PaneInfo.Right();
	m_PaneInfo.Layer(4);
	m_PaneInfo.Position(5);
	m_PaneInfo.MinSize(SNAPPING_PANEL_SIZE);
	m_PaneInfo.CloseButton(true);
	m_PaneInfo.Hide();
	m_PaneInfo.FloatingPosition(100, 150);
	m_PaneInfo.FloatingSize(SNAPPING_PANEL_SIZE + wxSize(100,100));
	m_AuiPanelName = SNAPPING_PANEL_TITLE;
	
	AddManagedPane(myPanel, m_PaneInfo);

    m_ListCtrl->Bind(wxEVT_UPDATE_UI, &Snapping_PANEL::OnUpdateUIContextualMenuAdd, this, ID_SNAP_ADD);
    m_ListCtrl->Bind(wxEVT_UPDATE_UI, &Snapping_PANEL::OnUpdateUIContextualMenuRemove, this, ID_SNAP_REMOVE);
    m_ListCtrl->Bind(wxEVT_CONTEXT_MENU, &Snapping_PANEL::OnContextualMenu, this);
    m_ListCtrl->Bind(wxEVT_LIST_ITEM_ACTIVATED, &Snapping_PANEL::OnDClickList, this);
}





/***************************************************************************//**
 @brief Create the Panel controls
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
wxPanel * Snapping_PANEL::_CreateControls(){
	wxPanel *  ContentFrame = new wxPanel (m_ParentEvt, wxID_ANY);
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText5;
	m_staticText5 = new wxStaticText( ContentFrame, wxID_ANY, _("Tolerance (map unit) :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer14->Add( m_staticText5, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5 );
	
	m_TolerenceCtrl = new wxSpinCtrl( ContentFrame, ID_SNAP_TOLERENCE_TXT,
								 _T("10"), 
								 wxDefaultPosition, 
								 wxDefaultSize, 
								 wxSP_ARROW_KEYS,
								 1,
								 100,
								 10);
	bSizer14->Add( m_TolerenceCtrl, 1, wxALL|wxEXPAND, 5 );
	bSizer13->Add( bSizer14, 0, wxEXPAND, 5 );
	
	
    m_ListCtrl = new DataListReportCtrl( ContentFrame, wxID_ANY, wxDefaultPosition,
                                    wxDefaultSize, wxLC_REPORT | wxLC_HRULES | wxLC_VRULES);
    m_ListCtrl->InsertColumn(0, _("Layer"));
    m_ListCtrl->InsertColumn(1, _("Mode"));
    
 	bSizer13->Add( m_ListCtrl, 1, wxEXPAND, 5 );
	
	ContentFrame->SetSizer(bSizer13);
    bSizer13->Fit(ContentFrame);
    
    wxSize myFrameWidth = ContentFrame->GetSize();
    m_ListCtrl->SetColumnWidth(0, myFrameWidth.GetWidth() / 2);
    m_ListCtrl->SetColumnWidth(1, myFrameWidth.GetWidth() / 2);
    return ContentFrame;
}



Snapping_PANEL::~Snapping_PANEL(){
    // TODO: Check if still needed!
	m_ParentEvt->PopEventHandler(false);
    
    m_ListCtrl->Unbind(wxEVT_UPDATE_UI, &Snapping_PANEL::OnUpdateUIContextualMenuAdd, this, ID_SNAP_ADD);
    m_ListCtrl->Unbind(wxEVT_UPDATE_UI, &Snapping_PANEL::OnUpdateUIContextualMenuRemove, this, ID_SNAP_REMOVE);
    m_ListCtrl->Unbind(wxEVT_CONTEXT_MENU, &Snapping_PANEL::OnContextualMenu, this);
}



/***************************************************************************//**
 @brief Load the snapping from database
 @details Call the Snapping_PANEL::SetDataBase() function before
 @return  true if snapping status was loaded successfully
 @author Lucien Schreiber (c) CREALP 2009
 @date 19 January 2009
 *******************************************************************************/
void Snapping_PANEL::LoadSnappingStatus (){
	wxASSERT(m_pDB);
    m_SnappingMemory.Clear();
    
	int iTolerence = m_pDB->GetSnappingTolerence();
	m_SnappingMemory.SetTolerence(iTolerence);
	m_TolerenceCtrl->SetValue(iTolerence);
	
    m_ListCtrl->DeleteAllItems();
    long mylid = 0;
	wxString mylName = _T("");
	int mySnapStatus = tmSNAPPING_OFF;
    bool iFirstLoop = true;
    int iLoop = 0;
	while (1)
	{
		if (m_pDB->GetNextSnapping(mylid, mylName, mySnapStatus, iFirstLoop) == false){
			break;
        }
		iFirstLoop = false;
        
        long myListIndex = m_ListCtrl->InsertItem(m_ListCtrl->GetItemCount(), mylName);
        m_ListCtrl->SetText(myListIndex, tmSNAPPING_TEXT[mySnapStatus], 1);
        m_ListCtrl->SetItemData(myListIndex, mylid);
		
		// load item to memory
        m_SnappingMemory.AddSnappingMemory(mylid, mySnapStatus);
		iLoop++;
	}
}


/***************************************************************************//**
 @brief Save snapping status to the database
 @details Layers used for snapping were allready saved, we must save now the
 snapping staus for those layers
 @return  true if snapping status was saved successfully
 @author Lucien Schreiber (c) CREALP 2009
 @date 21 January 2009
 *******************************************************************************/
bool Snapping_PANEL::SaveSnappingStatus ()
{
    if (m_pDB == NULL) {
        return false;
    }
    if (m_pDB->SetSnappingTolerence(m_SnappingMemory.GetTolerence()) == false){
        return false;
    }
    
    // save snapping status
    return m_pDB->SaveSnappingAllStatus(&m_SnappingMemory);
}


/***************************************************************************//**
 @brief Called when user press add snapping
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
void Snapping_PANEL::OnAddSnapping( wxCommandEvent& event ){
    if (m_pDB == NULL) {
        return;
    }
    
    wxArrayLong myLayersID;
	wxArrayString myLayersName;
    
	wxString myDlgMessage = _("Select one or more layer(s) to add to the snapping list");
	wxString myDlgCaption = _("Snapping layers");
	
	m_pDB->GetValidLayersForSnapping(myLayersID, myLayersName);
	wxMultiChoiceDialog * myDlg = new wxMultiChoiceDialog(m_ListCtrl, myDlgMessage,  myDlgCaption, myLayersName);
    if(myDlg->ShowModal() != wxID_OK){
        return;
    }
    
    wxArrayInt mySelectedLayers = myDlg->GetSelections();
	if (mySelectedLayers.GetCount() == 0){
        return;
    }
    
    wxArrayLong myRealSelectedID;
    for (unsigned int i = 0; i< mySelectedLayers.GetCount(); i++){
        myRealSelectedID.Add(myLayersID.Item(mySelectedLayers.Item(i)));
    }
    m_pDB->AddLayersSnapping(myRealSelectedID, tmSNAPPING_BEGIN_END);
    LoadSnappingStatus();
}



/***************************************************************************//**
 @brief called when tolerence is changed
 @author Lucien Schreiber (c) CREALP 2009
 @date 22 January 2009
 *******************************************************************************/
void Snapping_PANEL::OnUpdateTolerence( wxSpinEvent & event ){
	m_SnappingMemory.SetTolerence(event.GetInt());
	m_pDB->SetSnappingTolerence(event.GetInt());
    
    wxASSERT(m_Renderer);
    m_Renderer->Refresh();
    m_Renderer->Update();
	event.Skip();
}


/***************************************************************************//**
 @brief Called when user press remove snapping
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
void Snapping_PANEL::OnRemoveSnapping( wxCommandEvent& event ){
    long mySelectedIndex = m_ListCtrl->GetSelectedFirst();
    if (mySelectedIndex == wxNOT_FOUND) {
        return;
    }
    
    if(m_pDB->DeleteLayerSnapping(m_ListCtrl->GetItemData(mySelectedIndex))==false){
        return;
    }
    
    LoadSnappingStatus();
}




void Snapping_PANEL::OnContextualMenu (wxContextMenuEvent & event){
    wxMenu myContextMenu;
    myContextMenu.Append(ID_SNAP_ADD, _("Add Layer..."));
    myContextMenu.Append(ID_SNAP_REMOVE, _("Remove Layer..."));
    m_ListCtrl->PopupMenu(&myContextMenu);
}


void Snapping_PANEL::OnDClickList (wxListEvent & event){
    int mySnappingStatus = m_SnappingMemory.GetSnappingMemoryStatus(event.GetData());
    
    tmSnapping_DLG myDlg(m_ListCtrl);
    myDlg.m_LayerName = event.GetText();
    myDlg.m_SnapStatus = mySnappingStatus;
    
    if (myDlg.ShowModal() != wxID_OK) {
        return;
    }
    
    m_SnappingMemory.SetSnappingMemoryStatus(event.GetData(), myDlg.m_SnapStatus);
    m_pDB->SaveSnappingAllStatus(&m_SnappingMemory);
    LoadSnappingStatus();
}



void Snapping_PANEL::OnUpdateUIContextualMenuAdd (wxUpdateUIEvent & event){
    if (m_pDB == NULL) {
        event.Enable(false);
        return;
    }
    
    event.Enable(true);
}



void Snapping_PANEL::OnUpdateUIContextualMenuRemove(wxUpdateUIEvent & event){
    if (m_pDB != NULL && m_ListCtrl->GetSelectedFirst() != wxNOT_FOUND) {
        event.Enable(true);
        return;
    }
    event.Enable(false);
}






