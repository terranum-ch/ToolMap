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
#include "querieswizard.h"


DEFINE_EVENT_TYPE(tmEVT_QUERY_RUN)
DEFINE_EVENT_TYPE(tmEVT_QUERY_MENU)


BEGIN_EVENT_TABLE( Queries_PANEL, ManagedAuiWnd )
	EVT_COMMAND (wxID_ANY, tmEVT_QUERY_MENU, Queries_PANEL::OnPressQueryMenu)
	EVT_FLATBUTTON(ID_QUERIES_ADD, Queries_PANEL::OnAddQueries)
	EVT_FLATBUTTON(ID_QUERIES_REMOVE,Queries_PANEL::OnRemoveQueries)
	EVT_FLATBUTTON(ID_QUERIES_RUN, Queries_PANEL::OnRunQueries)
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

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);

	wxArrayString colname;
	colname.Add(_("Description"));
	
	wxArrayInt colsize;
	colsize.Add(200);
	
    m_QueriesList = new QueriesList( parent, ID_QUERIES_LIST,
												 &colname, & colsize,
												 wxSize(100, 100));
    itemBoxSizer2->Add(m_QueriesList, 1, wxGROW,0);//|wxTOP|wxBOTTOM, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    wxFlatButton* itemToggleButton6 = new wxFlatButton( parent, ID_QUERIES_ADD, wxFLATBUTTON_TEXT_ADD);
    itemBoxSizer5->Add(itemToggleButton6, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxFlatButton* itemToggleButton7 = new wxFlatButton( parent, ID_QUERIES_REMOVE, wxFLATBUTTON_TEXT_REMOVE);
    itemBoxSizer5->Add(itemToggleButton7, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxFlatButton* itemToggleButton8 = new wxFlatButton( parent, ID_QUERIES_RUN, _("Run selected"), wxDefaultSize);
    itemBoxSizer5->Add(itemToggleButton8, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	
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



void Queries_PANEL::SetSelectedData (tmSelectedDataMemory * selected)
{
	wxASSERT(selected);
	m_QueriesList->SetSelected(selected);
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
	m_QueriesList->DeleteAllItems();
	
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
	if (m_IsProjectOpen){
		m_QueriesList->AddItem();
		//QueriesWizard myWizard (m_ParentEvt, m_pDB, wxID_ANY);
		//myWizard.ShowWizard();
	}
		
		//
}


/***************************************************************************//**
 @brief User press the remove queries
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 November 2008
 *******************************************************************************/
void Queries_PANEL::OnRemoveQueries (wxCommandEvent & event)
{
	if (m_IsProjectOpen)
		m_QueriesList->DeleteItem();
}



/***************************************************************************//**
 @brief User press the run query
 @details This function get data about the selected query and send a message
 tmEVT_QUERY_RUN to the #tmAttributionManager
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 November 2008
 *******************************************************************************/
void Queries_PANEL::OnRunQueries (wxCommandEvent & event)
{
	if (!m_IsProjectOpen){
		return;
	}
	
	wxArrayLong myResutls;
	m_QueriesList->GetAllSelectedItem(myResutls);
	if (myResutls.GetCount() == 0){
		return;
	}
		
	int myQid = m_QueriesList->GetItemData(myResutls.Item(0));
	wxString myQName = wxEmptyString;
	wxString myQCode =wxEmptyString;
	int myQTarget = 0;
	
	
	if (m_pDB->GetQueriesById(myQid, myQTarget, myQName, myQCode)){
		// sending event to the tmAttributionManager
		wxCommandEvent evt (tmEVT_QUERY_RUN, wxID_ANY);
		evt.SetString(myQCode);
		evt.SetInt(myQTarget);
		m_ParentEvt->GetEventHandler()->AddPendingEvent(evt);
	}

}

				 
void Queries_PANEL::OnPressQueryMenu (wxCommandEvent & event){
	wxCommandEvent myEmptyEvent;
	OnRunQueries(myEmptyEvent);
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
ListGenReportWithDialog(parent, id, pColsName, pColsSize, size)
{
	m_Selected = NULL;
	m_pDB = NULL;
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 November 2008
 *******************************************************************************/
QueriesList::~QueriesList()
{
	
}



void QueriesList::AddItem()
{	
	BeforeAdding();
	wxASSERT(m_pDialog);
	if (((QueriesWizard*) m_pDialog)->ShowWizard()==wxID_OK)
	{
		AfterAdding(TRUE);
	}
	else
		AfterAdding(FALSE);
	
	// delete dialog after use
	wxASSERT(m_pDialog);
	delete m_pDialog;
	m_pDialog = NULL;
	
}



/***************************************************************************//**
 @brief Called by AddItem just before dispalying the dialog
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 November 2008
 *******************************************************************************/
void QueriesList::BeforeAdding()
{
	QueriesWizard * myQueriesWzd = new QueriesWizard(this, m_pDB, wxID_ANY);
	wxASSERT(m_Selected);
	if (m_Selected->GetCount() == 1){
		if (myQueriesWzd->GetData()->IsGenericLayer(m_pDB, m_Selected->GetSelectedLayer())){
			if (myQueriesWzd->GetData()->m_QueryLayerType == TOC_NAME_ANNOTATIONS){
				wxLogError(_T("Annotations queries not supported now"));
			}
			else {
				myQueriesWzd->GetData()->m_QueryObjectGeomID = m_Selected->GetSelectedUnique();
			}
		}
	}
	SetDialog(myQueriesWzd);
														
}


/***************************************************************************//**
 @brief Called just after adding an item
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 November 2008
 *******************************************************************************/
void QueriesList::AfterAdding (bool bRealyAddItem)
{
	long myID = -1;
	
	QueriesWizard * pWizard = (QueriesWizard*) m_pDialog;
	wxASSERT(pWizard);
	QueriesBuilder myBuilder(pWizard->GetData());
	
		if (bRealyAddItem)
	{
		
		// Validate the query data
		if (myBuilder.IsOk()==false) {
			wxMessageBox(_("Error with the query. Please see Log for more informations"),
						 _("Query error"), wxOK | wxICON_ERROR);
			//delete m_pDialog;
			return;
		}
		
		// try creating the query
		wxASSERT(m_pDB);
		if (myBuilder.Create(m_pDB)==false){
			wxMessageBox(_("Error creating the query. Please see Log for more informations"),
						 _("Query error"), wxOK | wxICON_ERROR);
			//delete m_pDialog;
			return;
		}
		
		// try saving the query
		if (myBuilder.Save(m_pDB)==false) {
			wxMessageBox(_("Error saving the query. Please see Log for more informations"),
						 _("Query error"), wxOK | wxICON_ERROR);
			//delete m_pDialog;
			return;
			
		}
	
	
		// add query to the list.
		myID = m_pDB->DataBaseGetLastInsertedID();
		if (myID != wxNOT_FOUND) {
			AddItemToList(pWizard->GetData()->m_QueryName, -1);
			SetItemData(GetItemCount()-1, myID);
		}
	}
	
	// dialog deleted by base class
	//delete m_pDialog;
	
}


/***************************************************************************//**
 @brief Called just before deleting item(s)
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 November 2008
 *******************************************************************************/
void QueriesList::BeforeDeleting ()
{
	wxArrayLong mySelected;
	GetAllSelectedItem(mySelected);
	
	for (unsigned int i = 0; i< mySelected.GetCount();i++)
	{
		if (!m_pDB->DeleteQuery(GetItemData(mySelected.Item(i))))
		{
			wxLogDebug(_T("Error deleting query"));
			break;
		}
	}
	
}



/***************************************************************************//**
 @brief Called just before editing item
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 November 2008
 *******************************************************************************/
void QueriesList::BeforeEditing ()
{
	// get info for first selected item
	wxArrayLong mySelected;
	GetAllSelectedItem(mySelected);
	
	wxString myName = wxEmptyString;
	wxString myQuery = _T("Error getting the query");
	int myQTarget = 0;
	
	int iIndex = mySelected.Item(0);
	if (!m_pDB->GetQueriesById(GetItemData(iIndex), myQTarget, myName, myQuery))
		wxLogDebug(_T("Error getting the query"));
	
	QueriesListDLG * myQueriesDlg = new QueriesListDLG (this);
	myQueriesDlg->SetQueriesName(myName);
	myQueriesDlg->SetQueriesDescription(myQuery);
	myQueriesDlg->SetQueriesTarget(myQTarget);
	SetDialog(myQueriesDlg);
	
}



/***************************************************************************//**
 @brief Called just after editing is done
 @param bRealyEdited true if edition was accepted using wxID_OK, false if
 edition was cancelled
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 November 2008
 *******************************************************************************/
void QueriesList::AfterEditing (bool bRealyEdited)
{
	wxArrayLong mySelected;
	GetAllSelectedItem(mySelected);
	long myQid = GetItemData(mySelected.Item(0));
	
	wxString myName = ((QueriesListDLG*)m_pDialog)->GetQueriesName();
	wxString myQuery = ((QueriesListDLG*)m_pDialog)->GetQueriesDescription();
	int myQTarget = ((QueriesListDLG*)m_pDialog)->GetQueriesTarget();
	
	// if save pressed, update the DB
	if (bRealyEdited)
	{
		// update the query name
		SetItemText(mySelected.Item(0),0, myName);
		
		if(!m_pDB->EditQueries(myQTarget, myName, myQuery, myQid))
			wxLogDebug(_T("Error modifying the query"));
		
	}
	
	
	
	delete m_pDialog;
}






