/***************************************************************************
								tmaattribwindow.cpp
                    Display the Advanced Attribution Window
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

#include "tmaattribwindow.h"

// init static member
wxRect tmAAttribWindow::m_WndPos = wxRect(wxDefaultPosition, wxDefaultSize);


/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
tmAAttribWindow::tmAAttribWindow( wxWindow* parent, PrjMemLayersArray * layers,
								 wxArrayString * values, wxWindowID id, 
								 const wxString& title, const wxPoint& pos,
								 const wxSize& size, long style ) :
wxDialog( parent, id, title, pos, size, style )
{
	InitMemberValue();
	CreateControls();

	m_Layers = layers;
	m_Values = values;
	m_iTotalControls = GetNumberControls();
	CheckValuesAndControls();
}



/***************************************************************************//**
 @brief Init member values with default values
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
void tmAAttribWindow::InitMemberValue ()
{
	m_AAttribTree = NULL;
	m_Layers = NULL;
	m_Values = NULL;
	m_Ctrls.Clear();
	m_iTotalControls = 0;
}



/***************************************************************************//**
 @brief Check for correct number of values and controls
 @details Ensure that we have the same number of values that we have controls
 @return true if we have the same number of controls and values
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 March 2009
 *******************************************************************************/
bool tmAAttribWindow::CheckValuesAndControls()
{
	if (m_iTotalControls <= 0)
		return false;
	
	if ((signed)m_Values->GetCount() != m_iTotalControls)
	{
		wxLogError(_T("Not same number of values (%d) that they are controls (%d)"),
				   m_Values->GetCount(), m_iTotalControls);
		return false;
	}
	return true;
}



/***************************************************************************//**
 @brief Create default controls for UI
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
void tmAAttribWindow::CreateControls ()
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_AAttribTree = new tmAAttribTree( this, wxID_ANY);
	bSizer20->Add( m_AAttribTree, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	wxButton* m_button18;
	m_button18 = new wxButton( this, wxID_CANCEL, _("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_button18, 0, wxBOTTOM | wxLEFT | wxRIGHT, 5 );
	
	wxButton* m_button19;
	m_button19 = new wxButton( this, wxID_OK, _("&Attribute"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button19->SetDefault(); 
	bSizer21->Add( m_button19, 0, wxBOTTOM | wxLEFT | wxRIGHT, 5 );
	
	bSizer20->Add( bSizer21, 0, wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer20 );
	this->Layout();
	bSizer20->Fit( this );
	
	SetWindowPosition();
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
tmAAttribWindow::~tmAAttribWindow()
{
	GetWindowPosition();
}



/***************************************************************************//**
 @brief Get Window position
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 March 2009
 *******************************************************************************/
void tmAAttribWindow::GetWindowPosition()
{
	m_WndPos.SetPosition(this->GetPosition());
	m_WndPos.SetSize(this->GetSize());
}



/***************************************************************************//**
 @brief Set Window position
 @details Checks that we are inside the visible part of the screen
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 March 2009
 *******************************************************************************/
void tmAAttribWindow::SetWindowPosition()
{
	if (m_WndPos.IsEmpty())
	{
		wxSize myCurrentSize = this->GetSize();
		if (myCurrentSize.GetWidth() < 200 || myCurrentSize.GetHeight() < 200)
			this->SetSize(300, 200);
		this->Center(wxVERTICAL | wxHORIZONTAL);
	}
	else
	{
		// check that we are inside the screen
		wxSize myScreenSize = wxGetDisplaySize();
		wxRect myScreenRect (wxPoint (0,0), myScreenSize);
		if (myScreenRect.Contains(m_WndPos))
		{
			this->SetSize(m_WndPos);
		}
		else
		{
			this->Center(wxVERTICAL | wxHORIZONTAL);
		}
	}
}



/***************************************************************************//**
 @brief Get the number of controls
 @details This function iterate for all controls stored into PrjMemLayersArray.
 This function is called once in the ctor. After that you want to get the
 number of ctrl, use the m_iTotalControls member value.
 @return  Total number of controls returned
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 March 2009
 *******************************************************************************/
int tmAAttribWindow::GetNumberControls ()
{
	wxASSERT (m_Layers);

	int iNumControl = 0;
	PrjMemFieldArray * m_Fields = NULL;
	
	for (unsigned int i = 0; i< m_Layers->GetCount();i++)
	{
		
		m_Fields = m_Layers->Item(i).m_pLayerFieldArray;
		wxASSERT (m_Fields);
		iNumControl += m_Fields->GetCount();
	}
	
	if (iNumControl == 0)
	{
		wxLogError(_T("No control returned, there is an error, quit toolmap"));
	}
	
	return iNumControl;
}



/***************************************************************************//**
 @brief Transfer data to window
 @details This function is called automatically when the tmAAttribWindow is
 displayed
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 March 2009
 *******************************************************************************/
bool tmAAttribWindow::TransferDataToWindow()
{
	wxASSERT (m_Layers);
	wxASSERT (m_Values);
	wxASSERT (m_AAttribTree);
	wxASSERT (m_iTotalControls > 0);
	
	tmAAttribCtrl * mypCtrl = NULL;
	PrjMemFieldArray * m_Fields = NULL;
	tmAAttribCtrlStruct myCtrl;
	int iCtrlPos = 0;
	
	
	for (unsigned int i = 0; i< m_Layers->GetCount();i++)
	{
		m_AAttribTree->AddLayerNode(m_Layers->Item(i).m_LayerName);
		m_Fields = m_Layers->Item(i).m_pLayerFieldArray;
		wxASSERT (m_Fields);
		for (unsigned int j = 0; j< m_Fields->GetCount();j++)
		{
			mypCtrl = m_AAttribTree->AddControl(m_Fields->Item(j));
			myCtrl.m_Ctrl = mypCtrl;
			m_Ctrls.Add(myCtrl);
			SetValue(iCtrlPos, mypCtrl);
			iCtrlPos++;
		}
	}
	
	return true;
}


bool tmAAttribWindow::SetValue (unsigned int pos, tmAAttribCtrl * ctrl)
{
	wxASSERT (m_Values);
	if (pos >= m_Values->GetCount())
	{
		wxLogDebug(_T("Value for control @ position %d doesn't exist !"), pos);
		return false;
	}
	
	wxString mySetVal = m_Values->Item(pos);
	if (mySetVal.IsEmpty())
	{
		wxLogDebug(_T("Empty string detected for item @ position : %d"), pos);
		return false;
	}
	
	ctrl->SetControlValue(mySetVal);
	return true;
}

