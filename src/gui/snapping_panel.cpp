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






/************************ SNAPPING LIST *****************************************/
/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
SnappingList::SnappingList (wxWindow * parent,
						  wxWindowID id,
						  wxArrayString * pColsName, 
						  wxArrayInt * pColsSize,
						  wxSize size) :
ListGenReportWithDialog(parent, id, pColsName, pColsSize, size)
{
	m_Parent = parent;
	m_SnappingMemory = new tmSnappingMemory();
	m_pDB = NULL;
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
SnappingList::~SnappingList()
{
	wxDELETE(m_SnappingMemory);
}



/***************************************************************************//**
 @brief Set the snapping status
 @details Put yes or empty in snapping status column
 @param snapStatus The snapping status as integer :
  - 0 = No snapping 
  - 1 = Snapping vertex 
  - 2 = snapping Begin/End
 @param iRow the zero based row index of the item to change
 @param clearbefore If set to true, we remove old values.
 @author Lucien Schreiber (c) CREALP 2009
 @date 19 January 2009
 *******************************************************************************/
void SnappingList::SetSnappingStatus (int snapStatus, int iRow, bool clearbefore)
{
	// clear text
	if (clearbefore)
	{
		SetItemText(iRow, 1, _T(""));
		SetItemText(iRow, 2, _T(""));
	}
	
	if ((snapStatus & tmSNAPPING_VERTEX) == tmSNAPPING_VERTEX)
		SetItemText(iRow, 1, tmSNAPPING_TEXT_YES);
	
	if ((snapStatus & tmSNAPPING_BEGIN_END) == tmSNAPPING_BEGIN_END)
		SetItemText(iRow, 2, tmSNAPPING_TEXT_YES);
		
}



/***************************************************************************//**
 @brief Get the snapping status from the list
 @details This function gets the snapping status from one lists row into an
 integer
 @param iRow the zero based index of the row we want to get the snapping status
 @return  one of the following values :
 - 0 = no snapping 
 - 1 = snapping vertex
 - 2 = snapping begin/end 
 - 3 = snapping both
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
int SnappingList::GetSnappingStatus (int iRow)
{
	// ensure that the row exists
	wxASSERT (iRow <= (GetItemCount() -1));
	
	int iReturnSnap = 0;
	if (GetItemColText(iRow, 1) == tmSNAPPING_TEXT_YES)
		iReturnSnap = iReturnSnap | tmSNAPPING_VERTEX;
	
	if (GetItemColText(iRow, 2) == tmSNAPPING_TEXT_YES)
		iReturnSnap = iReturnSnap | tmSNAPPING_BEGIN_END;
		
	
	return iReturnSnap;
}



/***************************************************************************//**
 @brief Called just before displaying the add dialog
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
void SnappingList::BeforeAdding()
{
	m_LayersID.Clear();
	m_LayersName.Clear();
	
	wxString myDlgMessage = _("Select one or more layer(s) to add to the snapping list");
	wxString myDlgCaption = _("Snapping layers");
	
	m_pDB->GetValidLayersForSnapping(m_LayersID, m_LayersName);
	wxMultiChoiceDialog * myDlg = new wxMultiChoiceDialog(this,
														  myDlgMessage,
														  myDlgCaption,
														  m_LayersName);
	SetDialog(myDlg);
}



/***************************************************************************//**
 @brief Called after the adding dialog was closed
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
void SnappingList::AfterAdding (bool bRealyAddItem)
{
	wxArrayInt mySelectedLayers = ((wxMultiChoiceDialog*) m_pDialog)->GetSelections();
	wxArrayLong myRealSelectedID;
	if (bRealyAddItem && mySelectedLayers.GetCount() > 0)
	{
		
		for (unsigned int i = 0; i< mySelectedLayers.GetCount(); i++)
		{
			// get real id from selected (zero based id)
			myRealSelectedID.Add(m_LayersID.Item(mySelectedLayers.Item(i)));
		
			//add snapping layers into list
			AddItemToList(m_LayersName.Item(mySelectedLayers.Item(i)));
			SetItemData(GetItemCount()-1, m_LayersID.Item(mySelectedLayers.Item(i)));
			
			// Stores items in memory
			m_SnappingMemory->AddSnappingMemory(m_LayersID.Item(mySelectedLayers.Item(i)),
												0);
		}
		
		// add snapping layers into database
		//m_pDB->AddLayersSnapping(myRealSelectedID);
	}
	//delete m_pDialog;
}



/***************************************************************************//**
 @brief Called just before deleting an item
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
void SnappingList::BeforeDeleting ()
{
	wxArrayLong mySelected;
	GetAllSelectedItem(mySelected);
	
	for (unsigned int i = 0; i< mySelected.GetCount();i++)
	{
		// delete item from memory
		m_SnappingMemory->DeleteSnappingMemory(GetItemData(mySelected.Item(i)));
		
		// delete item from database
		if(!m_pDB->DeleteLayerSnapping(GetItemData(mySelected.Item(i))))
		{
			wxLogDebug(_T("Error deleting snapping layers : %d"),
					   GetItemData(mySelected.Item(i)));
			break;
		}
	}
}



/***************************************************************************//**
 @brief Called when user double-click an item
 @details Here we don't use the traditionnal dialog for editing type of
 snapping. We toogle the snapping status when double-clicking into one's item
 column.
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
void SnappingList::OnDoubleClickItem (wxListEvent & event)
{	
	int iColClicked = GetColumnClicked(m_Parent,
									   event.GetIndex(), 0);
	
	// get the status and set the status for the snapping
	if (iColClicked > 0)
	{
		int iActualSnapStatus = GetSnappingStatus(event.GetIndex());
		iActualSnapStatus = iActualSnapStatus ^ iColClicked;
		
		SetSnappingStatus(iActualSnapStatus, event.GetIndex(), true);
		
		// modifiy status into memory
		m_SnappingMemory->SetSnappingMemoryStatus(GetItemData(event.GetIndex()),iActualSnapStatus);
	}
}



/***************************************************************************//**
 @brief Remove all snapping set
 @author Lucien Schreiber (c) CREALP 2009
 @date 22 January 2009
 *******************************************************************************/
void SnappingList::ClearSnappingStatus ()
{
	for (int i = 0;i< GetItemCount();i++)
	{
		SetSnappingStatus(tmSNAPPING_OFF, i, true);
	}
	m_SnappingMemory->ClearSnappingStatus();
	
	//SnappingUpdate();
}

