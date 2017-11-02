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

DEFINE_EVENT_TYPE(tmEVT_EV_DISPLAY_VERTEX_COORD);


// EVENT TABLE
BEGIN_EVENT_TABLE(EditVertexDLG, wxDialog)
	EVT_FLATBUTTON (ID_BTN_ADD_VERTEX, EditVertexDLG::OnVertexAdd)
	EVT_FLATBUTTON (ID_BTN_REMOVE_VERTEX, EditVertexDLG::OnVertexRemove)
	EVT_FLATBUTTON (ID_BTN_DISPLAYVERTEX, EditVertexDLG::OnVertexHighlight)
	EVT_BUTTON (wxID_OK, EditVertexDLG::OnSave)
	EVT_IDLE (EditVertexDLG::OnIdleTime)
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
	m_LayerType = -1;
	
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
	
	m_BtnAddVertex = new wxFlatButton( this, ID_BTN_ADD_VERTEX, wxFLATBUTTON_TEXT_ADD);
	bSizer20->Add( m_BtnAddVertex, 0, 0, 0 );
	
	m_BtnRemoveVertex = new wxFlatButton( this, ID_BTN_REMOVE_VERTEX, wxFLATBUTTON_TEXT_REMOVE);
	bSizer20->Add( m_BtnRemoveVertex, 0, wxLEFT , 5 );
	m_BtnRemoveVertex->Enable(false);
	
	m_DisplayVertexPosBtn = new wxFlatButton( this, ID_BTN_DISPLAYVERTEX, 
										 _("Display vertex position"), wxDefaultSize);
	m_DisplayVertexPosBtn->Enable(false);
	bSizer20->Add( m_DisplayVertexPosBtn, 0, wxLEFT, 5 );	
	bSizer19->Add( bSizer20, 0, wxEXPAND | wxALL, 5 );
	
	/*wxStaticLine* m_staticline2;
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer19->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );*/
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	
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
	bSizer16->Add(m_Status, 0, wxALL|wxGROW, 0);
	
	
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
	wxString myStatText = wxString::Format(_("Object ID : %ld"), oid);
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
	double dx = 0, dy = 0;
	m_VertexPts.Clear();
	
	for (int i = 0; i< m_VertexGrid->GetNumberRows();i++)
	{
		m_VertexGrid->GetCellValue(i, 0).ToDouble(&dx);
		m_VertexGrid->GetCellValue(i, 1).ToDouble(&dy);
		m_VertexPts.Add(wxRealPoint(dx,dy));
	}
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
		iIndex = m_VertexGrid->GetNumberRows();
	
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



/***************************************************************************//**
 @brief Called when user press +
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
void EditVertexDLG::OnVertexAdd (wxCommandEvent & event)
{
	int iSelected = GridGetSelection();
	if (iSelected == wxNOT_FOUND)
		iSelected = m_VertexGrid->GetNumberRows()-1;
	
	wxASSERT (m_LayerType != -1);
	if (m_LayerType == LAYER_SPATIAL_POINT)
		return;	
		
	GridInsertLine(iSelected+1, NULL);
	SetStatusNumberVertex(m_VertexGrid->GetNumberRows());
}



/***************************************************************************//**
 @brief Called when user press -
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
void EditVertexDLG::OnVertexRemove (wxCommandEvent & event)
{
	int iSelected = GridGetSelection();
	if (iSelected == wxNOT_FOUND)
		return;
	
	// not able to remove if layertype is point
	wxASSERT (m_LayerType != -1);
	if (m_LayerType == LAYER_SPATIAL_POINT)
		return;
	
	m_VertexGrid->DeleteRows(iSelected, 1);
	SetStatusNumberVertex(m_VertexGrid->GetNumberRows());
}



/***************************************************************************//**
 @brief Called when user press "Highlight vertex"
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
void EditVertexDLG::OnVertexHighlight (wxCommandEvent & event)
{
	// get selection status
	int iSel = GridGetSelection();
	if (iSel == wxNOT_FOUND)
		return;
	
	// getting cell value
	double dx = 0, dy = 0;
	m_VertexGrid->GetCellValue(iSel, 0).ToDouble(&dx);
	m_VertexGrid->GetCellValue(iSel, 1).ToDouble(&dy);
	
	// sending event to tmEditManager
	wxCommandEvent evt(tmEVT_EV_DISPLAY_VERTEX_COORD, wxID_ANY);
	wxRealPoint * myPt = new wxRealPoint(dx,dy);
	evt.SetClientData(myPt);
	GetParent()->GetEventHandler()->QueueEvent(evt.Clone());
}


/***************************************************************************//**
 @brief Called during idle time
 @details Update the buttons and controls. + and - controls are disabled if
 nothing seleced
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
void EditVertexDLG::OnIdleTime (wxIdleEvent & event)
{
	// get selection status
	bool bSelection = false;
	if (GridGetSelection() != wxNOT_FOUND)
		bSelection = true;
	
	UpdateAddVertexButton(bSelection);
	UpdateRemoveVertexButton(bSelection);
	UpdateHighlightVertexButton(bSelection);
	UpdateSaveButton();
	
}


/***************************************************************************//**
 @brief Called when Save button is pressed
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
void EditVertexDLG::OnSave (wxCommandEvent & event)
{
	wxString myErrMsg = _("Some coordinates aren't correctly defined\n");
	wxString myErrMsg2 = _("Please define all coordinates before updating");
	
	int iRow = 0, iCol = 0;
	if (IsAllCellsNumber(iCol, iRow)== false)
	{
		wxMessageBox(myErrMsg + myErrMsg2,
					 _("Coordinate error"),
					 wxICON_STOP | wxOK , this);
				
		m_VertexGrid->SelectRow(iRow);
		m_VertexGrid->MakeCellVisible(iRow, iCol);
	}
	else
		event.Skip();
}



/***************************************************************************//**
 @brief Checks validity of cells
 @details This function checks all cells for empy or non numeric ones.
 @param col adress of first false cell (col)
 @param row adress of first false cell (row)
 @return true if all cells are numeric, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
bool EditVertexDLG::IsAllCellsNumber(int & col, int & row)
{
	double dx = 0, dy = 0;
	
	for (int i=0; i< m_VertexGrid->GetNumberRows(); i++)
	{
		wxString myNumX = m_VertexGrid->GetCellValue(i, 0);
		wxString myNumY = m_VertexGrid->GetCellValue(i, 1);
		
		
		if (!myNumX.ToDouble(&dx))
		{
			row = i;
			col = 0;
			return false;
		}
		
		if (!myNumY.ToDouble(&dy))
		{
			row = i;
			col = 1;
			return false;
		}
		
	}
	return true;
}

/***************************************************************************//**
 @brief Enable / Diable Add vertex button
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
void EditVertexDLG::UpdateAddVertexButton(bool selection)
{
	if (m_LayerType == LAYER_SPATIAL_LINE)
		m_BtnAddVertex->Enable(true);
	else
		m_BtnAddVertex->Enable(false);
		
}



/***************************************************************************//**
 @brief Enable / Diable Remove vertex button
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
void EditVertexDLG::UpdateRemoveVertexButton (bool selection)
{
	if (selection && m_LayerType == LAYER_SPATIAL_LINE)
		m_BtnRemoveVertex->Enable(true);
	else
		m_BtnRemoveVertex->Enable(false);
}


/***************************************************************************//**
 @brief Enable / Diable highlight vertex button
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
void EditVertexDLG::UpdateHighlightVertexButton (bool selection)
{
	if (selection)
		m_DisplayVertexPosBtn->Enable(true);
	else 
		m_DisplayVertexPosBtn->Enable(false);

}


/***************************************************************************//**
 @brief Enable / Diable Update  button
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
void EditVertexDLG::UpdateSaveButton ()
{
	if (m_LayerType == LAYER_SPATIAL_LINE && m_VertexGrid->GetNumberRows() < 2)
		m_BtnUpdate->Enable(false);
	else
		m_BtnUpdate->Enable(true);
}

