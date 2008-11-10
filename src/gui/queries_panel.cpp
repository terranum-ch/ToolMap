/***************************************************************************
 queries_panel.cpp
  Deals with the queries panel
 -------------------
 copyright            : (C) 2008 CREALP Lucien Schreiber 
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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "queries_panel.h"




BEGIN_EVENT_TABLE( Queries_PANEL, ManagedAuiWnd )
	EVT_FLATBUTTON(ID_QUERIES_ADD, Queries_PANEL::OnAddQueries)
END_EVENT_TABLE()



/***************************************************************************//**
 @brief Creating
 @details For two step creation
 @author Lucien Schreiber (c) CREALP 2008
 @date 08 November 2008
 *******************************************************************************/
Queries_PANEL::Queries_PANEL( wxWindow* parent, wxWindowID id, wxAuiManager * auimanager) :
	ManagedAuiWnd(auimanager)
{
	InitMemberValues();
	
	m_ParentEvt = parent;
	m_ParentEvt->PushEventHandler(this);
	
	wxPanel *  ContentFrame = new wxPanel (parent, wxID_ANY);
	CreateControls(ContentFrame);	
	
	// define properties of Panel.
	m_PaneInfo.Name(SYMBOL_QUERIES_PANEL_TITLE);
	m_PaneInfo.Caption(SYMBOL_QUERIES_PANEL_TITLE);
	m_PaneInfo.Right();
	m_PaneInfo.Layer(3);
	m_PaneInfo.Position(3);
	m_PaneInfo.MinSize(SYMBOL_QUERIES_PANEL_SIZE);
	m_PaneInfo.CloseButton(FALSE);
	m_PaneInfo.Hide();
	
	m_AuiPanelName = SYMBOL_QUERIES_PANEL_TITLE;
	
	AddManagedPane(ContentFrame, m_PaneInfo);

}


/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 08 November 2008
 *******************************************************************************/
Queries_PANEL::~Queries_PANEL()
{
	m_ParentEvt->PopEventHandler(FALSE);
}



/***************************************************************************//**
 @brief Init default values to member objects
 @author Lucien Schreiber (c) CREALP 2008
 @date 08 November 2008
 *******************************************************************************/
void Queries_PANEL::InitMemberValues()
{
	m_pDB = NULL;
	m_ParentEvt = NULL;
	m_IsProjectOpen = false; // project isn't open now
}


/***************************************************************************//**
 @brief Control creation
 @author Lucien Schreiber (c) CREALP 2008
 @date 08 November 2008
 *******************************************************************************/
wxSizer * Queries_PANEL::CreateControls(wxWindow * parent,
						 bool call_fit,	 bool set_sizer)
{    
    //Queries_PANEL* itemManagedAuiWnd1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    //itemManagedAuiWnd1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(parent, wxID_ANY, _("Define your queries"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 1, wxGROW|wxALL, 5);

	wxArrayString colname;
	colname.Add(_("Description"));
	
	wxArrayInt colsize;
	colsize.Add(200);
	
    m_QueriesList = new QueriesList( parent, ID_QUERIES_LIST,
												 &colname, & colsize,
												 wxSize(100, 100));
    itemStaticBoxSizer3->Add(m_QueriesList, 1, wxGROW|wxTOP|wxBOTTOM, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer3->Add(itemBoxSizer5, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    wxFlatButton* itemToggleButton6 = new wxFlatButton( parent, ID_QUERIES_ADD, wxFLATBUTTON_TEXT_ADD);
    itemToggleButton6->SetValue(false);
    itemBoxSizer5->Add(itemToggleButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlatButton* itemToggleButton7 = new wxFlatButton( parent, ID_QUERIES_REMOVE, wxFLATBUTTON_TEXT_REMOVE);
    itemToggleButton7->SetValue(false);
    itemBoxSizer5->Add(itemToggleButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlatButton* itemToggleButton8 = new wxFlatButton( parent, ID_QUERIES_RUN, _("Run selected"), wxDefaultSize);
    itemToggleButton8->SetValue(false);
    itemBoxSizer5->Add(itemToggleButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer9, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton10 = new wxButton( parent, ID_BUTTON, _("Clear selection"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemButton10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    /*wxButton* itemButton11 = new wxButton( parent, wxID_CLOSE, _("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemButton11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);*/

    /*wxButton* itemButton12 = new wxButton( parent, wxID_SAVE, _("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemButton12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);*/

    wxStatusBar* itemStatusBar13 = new wxStatusBar( parent, ID_STATUSBAR1, wxST_SIZEGRIP|wxNO_BORDER );
    itemStatusBar13->SetFieldsCount(2);
    //itemStatusBar13->SetStatusText(_("Query passed OK"), 0);
    //itemStatusBar13->SetStatusText(_("10 Queries"), 1);
    int itemStatusBar13Widths[2];
    itemStatusBar13Widths[0] = -2;
    itemStatusBar13Widths[1] = -1;
    itemStatusBar13->SetStatusWidths(2, itemStatusBar13Widths);
    itemBoxSizer2->Add(itemStatusBar13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
	m_QueriesList->SetStatusBar(itemStatusBar13);
	
	if (set_sizer)
    {
        parent->SetSizer( itemBoxSizer2 );
        if (call_fit)
            itemBoxSizer2->SetSizeHints( parent );
			}
    
    return itemBoxSizer2;

}



/***************************************************************************//**
 @brief Set the database object
 @details Set the database object for the #Queries_PANEL but also for the
 #QueriesList.
 @param database Object of type #DataBaseTM. Validity is checked in debug mode
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 November 2008
 *******************************************************************************/
void Queries_PANEL::SetDataBase (DataBaseTM * database)
{
	wxASSERT(database);
	m_pDB = database;
	m_QueriesList->SetDataBase(database);
}



/***************************************************************************//**
 @brief Load the queries from the database
 @details Don't need to callQueries_PANEL::SetDataBase(), call is done internaly
  @return true if the queries where loaded succesfully
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 November 2008
 *******************************************************************************/
bool Queries_PANEL::LoadQueries (DataBaseTM * database)
{
	SetDataBase(database);
	wxASSERT(m_pDB);
	EnableQueriesPanel(true);
	
	long myQid = 0;
	wxString myQName = _T("");
	wxString myQDescription = _T("");
	bool iFirstLoop = true;
	int iLoop = 0;
	
	
	while (1)
	{
		if(!m_pDB->GetNextQueries(myQid, myQName, myQDescription,iFirstLoop))
			break;
			
		iFirstLoop = false;
		m_QueriesList->AddItemToList(myQName, -1);
		m_QueriesList->SetItemData(iLoop, myQid);
		iLoop ++;
		
	}
	
	
	
	return false;
}



/*******************************************//**
 @brief Indicate if a project is open or not
 @details If a project is open, controls are accessible otherwise not.
 @param projectopen Status of project : is a project open (default is true)
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 November 2008
 *******************************************************************************/
void Queries_PANEL::EnableQueriesPanel (bool projectopen)
{
	m_IsProjectOpen = projectopen;
	
	//TODO: enable or disable control if needed
}


/***************************************************************************//**
 @brief User press the Add queries button
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 November 2008
 *******************************************************************************/
void Queries_PANEL::OnAddQueries (wxCommandEvent & event)
{
	if (m_IsProjectOpen)
		m_QueriesList->AddItem();
}









/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 November 2008
 *******************************************************************************/
QueriesList::QueriesList (wxWindow * parent,
						  wxWindowID id,
						  wxArrayString * pColsName, 
						  wxArrayInt * pColsSize,
						  wxSize size) :
ListGenReportWithStatus(parent, id, pColsName, pColsSize, size)
{
	
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 November 2008
 *******************************************************************************/
QueriesList::~QueriesList()
{
	
}



/***************************************************************************//**
 @brief Called by AddItem just before dispalying the dialog
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 November 2008
 *******************************************************************************/
void QueriesList::BeforeAdding()
{
	QueriesListDLG * myQueriesDlg = new QueriesListDLG (this);
	SetDialog(myQueriesDlg);
														
}


/***************************************************************************//**
 @brief Called just after adding an item
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 November 2008
 *******************************************************************************/
void QueriesList::AfterAdding (bool bRealyAddItem)
{
	wxString myName = ((QueriesListDLG*)m_pDialog)->GetQueriesName();
	wxString myQuery = ((QueriesListDLG*)m_pDialog)->GetQueriesDescription();
	
	
	if (bRealyAddItem)
	{
		// try to add the query into database
		
		// add the item to the list
		AddItemToList(myName, -1);
		
		// set the queries id to the item_data
		
		
		
		
		wxLogDebug(_T("Item added"));
	}
	
	delete m_pDialog;
	
}

/*virtual void BeforeDeleting ();
virtual void BeforeEditing ();
virtual void AfterEditing (bool bRealyEdited);*/



