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

// EVENT TABLE
BEGIN_EVENT_TABLE( Snapping_PANEL, ManagedAuiWnd )
	EVT_TEXT( ID_SNAP_TOLERENCE_TXT, Snapping_PANEL::OnUpdateTolerence )
	EVT_FLATBUTTON( ID_SNAP_ADD, Snapping_PANEL::OnAddSnapping )
	EVT_FLATBUTTON( ID_SNAP_REMOVE, Snapping_PANEL::OnRemoveSnapping )
	EVT_FLATBUTTON( ID_SNAP_CLEAR, Snapping_PANEL::OnClearSnapping )
END_EVENT_TABLE()



/***************************************************************************//**
 @brief Snapping panel constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
Snapping_PANEL::Snapping_PANEL( wxWindow* parent, wxWindowID id, wxAuiManager * auimanager)
: ManagedAuiWnd(auimanager)
{
	InitMembersValue();
	m_ParentEvt = parent;
	m_ParentEvt->PushEventHandler(this);
	
	wxPanel *  ContentFrame = new wxPanel (parent, wxID_ANY);
	CreateControls(ContentFrame);	
	
	// define properties of Panel.
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
	
	AddManagedPane(ContentFrame, m_PaneInfo);

}



/***************************************************************************//**
 @brief Set default values to member
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
void Snapping_PANEL::InitMembersValue()
{
	m_Tolerence = NULL;
	m_SnappingList = NULL;
	m_ParentEvt = NULL;
	m_pDB = NULL;
}



/***************************************************************************//**
 @brief Create the Panel controls
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
wxSizer * Snapping_PANEL::CreateControls(wxWindow * parent,
										 bool call_fit, bool set_sizer)
{
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText5;
	m_staticText5 = new wxStaticText( parent, wxID_ANY, _("Tolerence (map unit) :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer14->Add( m_staticText5, 0, wxALL, 5 );
	
	m_Tolerence = new wxTextCtrl( parent, ID_SNAP_TOLERENCE_TXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_Tolerence, 1, wxALL|wxEXPAND, 5 );
	
	bSizer13->Add( bSizer14, 0, wxEXPAND, 5 );
	
	wxArrayString colname;
	colname.Add(_("Layer"));
	colname.Add(_("Vertex"));
	colname.Add(_("Begin/End"));
	
	wxArrayInt colsize;
	colsize.Add(150);
	colsize.Add(80);
	colsize.Add(80);
	
    m_SnappingList = new SnappingList( parent, ID_SNAP_LIST,
									&colname, & colsize,
									wxSize(100, 100));
	
	
	bSizer13->Add( m_SnappingList, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlatButton* m_button10;
	m_button10 = new wxFlatButton( parent, ID_SNAP_ADD, wxFLATBUTTON_TEXT_ADD);
	bSizer15->Add( m_button10, 0, wxLEFT|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	wxFlatButton* m_button11;
	m_button11 = new wxFlatButton( parent, ID_SNAP_REMOVE, wxFLATBUTTON_TEXT_REMOVE);
	bSizer15->Add( m_button11, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	wxFlatButton* m_button12;
	m_button12 = new wxFlatButton( parent, ID_SNAP_CLEAR, _("Clear snapping"), wxDefaultSize);
	bSizer15->Add( m_button12, 0, wxTOP|wxBOTTOM, 5 );
	
	bSizer13->Add( bSizer15, 0, wxEXPAND, 5 );
	
	
	if (set_sizer)
    {
        parent->SetSizer( bSizer13 );
        if (call_fit)
		{
            bSizer13->SetSizeHints( parent );
		}
	}
    
    return bSizer13;
	
	
}


Snapping_PANEL::~Snapping_PANEL()
{
}



/***************************************************************************//**
 @brief Load the snapping from database
 @details Call the Snapping_PANEL::SetDataBase() function before
 @return  true if snapping status was loaded successfully
 @author Lucien Schreiber (c) CREALP 2009
 @date 19 January 2009
 *******************************************************************************/
bool Snapping_PANEL::LoadSnappingStatus ()
{
	wxASSERT(m_pDB);
	
	long mylid = 0;
	wxString mylName = _T("");
	int mySnapStatus = tmSNAPPING_OFF;
	bool iFirstLoop = true;
	int iLoop = 0;
	m_SnappingList->DeleteAllItems();
	
	while (1)
	{
		if (!m_pDB->GetNextSnapping(mylid, mylName, mySnapStatus, iFirstLoop))
			break;
		iFirstLoop = false;
		m_SnappingList->AddItemToList(mylName, iLoop);
		m_SnappingList->SetSnappingStatus(mySnapStatus, iLoop, false);
		m_SnappingList->SetItemData(iLoop, mylid);
		iLoop++;
	}
	
	
	return true;
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
	return m_SnappingList->SaveSnappingStatus();
}


/***************************************************************************//**
 @brief Called when user press add snapping
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
void Snapping_PANEL::OnAddSnapping( wxCommandEvent& event )
{
	if (m_pDB)
	{
		m_SnappingList->AddItem();
	}
}


/***************************************************************************//**
 @brief Called when user press remove snapping
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
void Snapping_PANEL::OnRemoveSnapping( wxCommandEvent& event )
{
	if (m_pDB)
	{
		m_SnappingList->DeleteItem();
	}

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
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
SnappingList::~SnappingList()
{
	delete m_SnappingMemory;
}



/***************************************************************************//**
 @brief Set the snapping status
 @details Put yes or empty in snapping status column
 @param snapStatus The snapping status as integer :
  - 0 = No snapping 
  - 1 = Snapping vertex 
  - 2 = snapping Begin/End
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
 @brief Load snapping info into memory
 @details This function is only needed for adding to the panel the ability to
 initially load snapping info to memory. After, all informations are updated by
 the #SnappingList itself (during adding, removing, etc)
 @param lids layer ID to add to the memory
 @param snapstatus snapping status to attach to the layer ID
 @author Lucien Schreiber (c) CREALP 2009
 @date 21 January 2009
 *******************************************************************************/
void SnappingList::LoadSnappingIntoMemory (const long & lid, const int & snapstatus)
{
	m_SnappingMemory->AddSnappingMemory(lid, snapstatus);
}


/***************************************************************************//**
 @brief Save snapping status to the database
 @details Layers used for snapping were allready saved, we must save now the
 snapping staus for those layers
 @return  true if snapping status was saved successfully
 @author Lucien Schreiber (c) CREALP 2009
 @date 21 January 2009
 *******************************************************************************/
bool SnappingList::SaveSnappingStatus ()
{
	if (m_pDB)
	{
		return m_pDB->SaveSnappingAllStatus(m_SnappingMemory);
	}
	
	return true;
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
	
	wxString myDlgMessage = _("Select one or more layer to add to the snapping list");
	wxString myDlgCaption = _("Add one or more snapping layer(s)");
	
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
			
			// Stores items in memory
			m_SnappingMemory->AddSnappingMemory(m_LayersID.Item(mySelectedLayers.Item(i)),
												0);
		}
		
		// add snapping layers into database
		m_pDB->AddLayersSnapping(myRealSelectedID);
		
		//TODO: send message to update snapping status
		
		
	}
	delete m_pDialog;
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
	
	// TODO: send message to update snapping status
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
		m_SnappingMemory->SetSnappingMemoryStatus(GetItemData(event.GetIndex()),
												  iActualSnapStatus);
		
		//TODO: send message to update snapping status
	}
}

