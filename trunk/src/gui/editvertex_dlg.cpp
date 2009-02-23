/***************************************************************************
								editvertex_dlg.cpp
                    Dialog for manuel editing vertex position
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber
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

#include "editvertex_dlg.h"

BEGIN_EVENT_TABLE(EditVertexDLG, wxDialog)
	EVT_FLATBUTTON (ID_BTN_ADD_VERTEX, EditVertexDLG::OnVertexAdd)
	EVT_FLATBUTTON (ID_BTN_REMOVE_VERTEX, EditVertexDLG::OnVertexRemove)
END_EVENT_TABLE()


/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
EditVertexDLG::EditVertexDLG( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	m_VertexPts.Clear();
	m_SelectedOID = -1;
	
	CreateControls();
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
EditVertexDLG::~EditVertexDLG()
{
}



/***************************************************************************//**
 @brief Controls creation
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
void EditVertexDLG::CreateControls()
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	m_VertexGrid = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	//m_VertexGrid->SetDefaultColSize(100);
	m_VertexGrid->CreateGrid( 5, 2 );
	m_VertexGrid->EnableEditing( true );
	m_VertexGrid->EnableGridLines( true );
	m_VertexGrid->EnableDragGridSize( true );
	m_VertexGrid->SetMargins( 0, 0 );
	
	// Columns
	m_VertexGrid->EnableDragColMove( false );
	m_VertexGrid->EnableDragColSize( true );
	m_VertexGrid->SetColLabelSize( 30 );
	m_VertexGrid->SetColLabelValue( 0, _("X") );
	m_VertexGrid->SetColLabelValue( 1, _("Y") );
	m_VertexGrid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_VertexGrid->EnableDragRowSize( true );
	m_VertexGrid->SetRowLabelSize( 40 );
	m_VertexGrid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_VertexGrid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	//m_VertexGrid->SetMinSize( wxSize( 200,200 ) );
	
	bSizer19->Add( m_VertexGrid, 1, wxEXPAND|wxBOTTOM, 5 );
	
	wxStaticBoxSizer * bSizer20;
	bSizer20 = new wxStaticBoxSizer( wxHORIZONTAL, this, _("Operations"));
	
	wxFlatButton* m_button17;
	m_button17 = new wxFlatButton( this, ID_BTN_ADD_VERTEX, wxFLATBUTTON_TEXT_ADD);
	bSizer20->Add( m_button17, 0, 0, 0 );
	
	wxFlatButton* m_button18;
	m_button18 = new wxFlatButton( this, ID_BTN_REMOVE_VERTEX, wxFLATBUTTON_TEXT_REMOVE);
	bSizer20->Add( m_button18, 0, wxLEFT , 5 );
	
	m_DisplayVertexPosBtn = new wxFlatButton( this, ID_BTN_DISPLAYVERTEX, 
										 _("Display vertex position"), wxDefaultSize);
	bSizer20->Add( m_DisplayVertexPosBtn, 0, wxLEFT, 5 );	
	bSizer19->Add( bSizer20, 0, wxEXPAND | wxALL, 5 );
	
	/*wxStaticLine* m_staticline2;
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer19->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );*/
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	wxButton* m_BtnUpdate;
	m_BtnUpdate = new wxButton( this, wxID_OK, _("Update"), wxDefaultPosition, wxDefaultSize, 0 );
	m_BtnUpdate->SetDefault(); 
	bSizer18->Add( m_BtnUpdate, 0, wxALL, 5 );
	
	wxButton* m_button16;
	m_button16 = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_button16, 0, wxALL, 5 );
	
	bSizer19->Add( bSizer18, 0, wxALIGN_RIGHT, 5 );
	
	bSizer16->Add( bSizer19, 1, wxEXPAND, 5 );
	
	m_Status = new wxStatusBar( this, wxID_ANY, wxST_SIZEGRIP|wxNO_BORDER );
    m_Status->SetFieldsCount(2);
	bSizer16->Add(m_Status, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 0);
	
	
	this->SetSizer( bSizer16 );
	this->Layout();
	bSizer16->Fit( this );
	
	this->Centre( wxBOTH );	
	this->SetMinSize(wxSize(220,200));
	
	// sizing columns acording to the wnd size
	wxSize myDlgSize = this->GetSize();
	m_VertexGrid->SetDefaultColSize((myDlgSize.GetWidth() - m_VertexGrid->GetRowLabelSize()) / 2 - gSizeMargin,
									true);
}



/***************************************************************************//**
 @brief Update the status text
 @details Display the selected object ID
 @param oid Object ID selected
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
void EditVertexDLG::SetStatusSelectedID (long oid)
{
	wxString myStatText = wxString::Format(_("Object ID : %d"), oid);
	m_Status->SetStatusText(myStatText, 0);
}



/***************************************************************************//**
 @brief Update the status text
 @details Display the number of vertex
 @param ivertexnumber Number of vertex
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
void EditVertexDLG::SetStatusNumberVertex (int ivertexnumber)
{
	wxString myStatText = wxString::Format(_("%d Vertex"), ivertexnumber);
	m_Status->SetStatusText(myStatText, 1);
}



/***************************************************************************//**
 @brief Data transfert
 @details This function is automatically called when dialog is diasplayed
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
bool EditVertexDLG::TransferDataToWindow()
{
	SetStatusSelectedID(m_SelectedOID);
	SetStatusNumberVertex(m_VertexPts.GetCount());
	
	
	if (GridClear() == false)
		return false;
	
	for (unsigned int i = 0; i< m_VertexPts.GetCount();i++)
		if (GridInsertLine(i, &m_VertexPts.Item(i))== false)
			return false;
	
	return true;
}


/***************************************************************************//**
 @brief Data transfert
 @details This function is automatically called when dialog is closed
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
bool EditVertexDLG::TransferDataFromWindow()
{
	
	return true;
}



/***************************************************************************//**
 @brief Insert or add a line
 @param iIndex index of the line to insert or -1 for adding at the end
 @param pt value of the point to add in the new line or null to add an empty line
 @return  true if inserting or adding was successfull, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
bool EditVertexDLG::GridInsertLine (int iIndex, wxRealPoint * pt)
{
	if (iIndex == -1)
		iIndex == m_VertexGrid->GetNumberRows();
	
	wxASSERT (iIndex <= m_VertexGrid->GetNumberRows());
	
	
	if (m_VertexGrid->InsertRows(iIndex, 1) == false)
	{
		wxLogError(_T("Error inserting row @ index = %d"), iIndex);
		return false;
	}
	
	if (pt != NULL)
	{
		wxString myXText = wxString::Format(_T("%.*f"),tmVERTEX_PRECISION, pt->x);
		wxString myYText = wxString::Format(_T("%.*f"),tmVERTEX_PRECISION,pt->y);
		
		m_VertexGrid->SetCellValue(iIndex, 0, myXText);
		m_VertexGrid->SetCellValue(iIndex, 1, myYText);
	}

	return true;
}


/***************************************************************************//**
 @brief Delete all rows from Grid
 @return  true if all rows where successfully deleted
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
bool EditVertexDLG::GridClear ()
{
	return m_VertexGrid->DeleteRows(0, m_VertexGrid->GetNumberRows());
} 



/***************************************************************************//**
 @brief Get the selected row
 @details This function return the index of selected row. Only valid if one row
 is selected. We didn't support multiple row selection actually.
 @return  index of selected row or wxNOT_FOUND if no row or more than one row
 selected
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
int EditVertexDLG::GridGetSelection ()
{
	wxArrayInt mySelRow = m_VertexGrid->GetSelectedRows();
	if (mySelRow.GetCount() == 1)
		return mySelRow.Item(0);
	
	return wxNOT_FOUND;
}
	

void EditVertexDLG::OnVertexAdd (wxCommandEvent & event)
{
	int iSelected = GridGetSelection();
	if (iSelected == wxNOT_FOUND)
		return;
		
	GridInsertLine(iSelected+1, NULL);
	SetStatusNumberVertex(m_VertexGrid->GetNumberRows());
}

void EditVertexDLG::OnVertexRemove (wxCommandEvent & event)
{
	int iSelected = GridGetSelection();
	if (iSelected == wxNOT_FOUND)
		return;
	
	m_VertexGrid->DeleteRows(iSelected, 1);
	SetStatusNumberVertex(m_VertexGrid->GetNumberRows());
}


void EditVertexDLG::OnVertexHighlight (wxCommandEvent & event)
{
	
}
