/***************************************************************************
								tmaattribtree.cpp
		Definition of advanced attribution controls such as the tmAAttribTree
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

#include "tmaattribtree.h"



/***************************************************************************//**
 @brief Two step constructor.
 @details Call Create when this constructor is called to build up the
 tmAAttribTree
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
tmAAttribTree::tmAAttribTree()
{
	
}


/***************************************************************************//**
 @brief Default constructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
tmAAttribTree::tmAAttribTree(wxWindow * parent,wxWindowID id, const wxPoint & pos,
							 const wxSize & size,long style, const wxValidator & validator,
							 const wxString & name) : 
							wxTreeMultiCtrl(parent,id,pos,size,style,validator)
{

	SetBackgroundColour(*wxWHITE);
	CreateRoot();
}



/***************************************************************************//**
 @brief Two step creation
 @details Whenever the control is created without any parameters, use Create to
 actually create it. Don't access the control's public methods before this is
 called
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
bool tmAAttribTree::CreateRoot () 
{
	ProjectDefMemoryFields myField;
	myField.m_Fieldname = _T("Coucou");
	myField.m_FieldType = TM_FIELD_ENUMERATION;
	myField.m_FieldPrecision = 6;
	myField.m_FieldScale = 2;
	
	PrjMemFieldCodedValArray myArray;
	ProjectDefMemoryFieldsCodedVal myVal;
	myVal.m_ValueName = _T("Actif");
	myArray.Add(myVal);
	myVal.m_ValueName = _T("Inactif");
	myArray.Add(myVal);
	myVal.m_ValueName = _T("Tassé");
	myArray.Add(myVal);	
	myVal.m_ValueName = _T("Mangé");
	myArray.Add(myVal);
	
	myField.m_pCodedValueArray = &myArray;

	
	AddLayerNode(_T("failles"));
	AddControl(myField);
	
	AddLayerNode(_T("tests"));
	AddControl(myField);
	// add root
   /* wxTreeMultiItem item = AddRoot(_T("Advanced attribution"));
	
    // create a wxTreeMultiWindowInfo object for indenting etc
    wxTreeMultiWindowInfo wndinfo (0, 10,10);
	
    // add subitem to root
    AppendWindow(item, new wxButton(this, -1, _T("Press this")), _T(""), wndinfo);
	
    // indent a few
    wndinfo.Indent(4);
	int ID_MYCHECK = 22220;
    // add a new one
    AppendWindow(item, new wxCheckBox(this, ID_MYCHECK, _T("Check this!")), _T(""), wndinfo);
	
    // now check the value of the checkbox
    SetBooleanValue(ID_MYCHECK, true);
	
	wxTreeMultiItem item2 = AddRoot(_T("Faille"));*/
	
	return true;
}



/***************************************************************************//**
 @brief destructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
tmAAttribTree::~tmAAttribTree()
{
	
}


/***************************************************************************//**
 @brief Add a node for specified layer
 @param layername The name of the layer
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 March 2009
 *******************************************************************************/
void tmAAttribTree::AddLayerNode (const wxString & layername)
{
	 wxTreeMultiItem item = AddRoot(layername);
	 m_ActualNode = item;
}


/***************************************************************************//**
 @brief Search for a given node
 @details We keep a object for the actual node, using this function set this object
 to the found node.
 @param layername name of the layer we are searching for
 @return  true if node was found
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 March 2009
 *******************************************************************************/
bool tmAAttribTree::FindLayerNode (const wxString & layername)
{
	m_ActualNode = FindItem(layername);
	if (m_ActualNode.IsOk())
		return true;
	
	return false;
}



/***************************************************************************//**
 @brief Add control into actual node
 @param fieldinfo All fields info such as Type, name, etc.
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 March 2009
 *******************************************************************************/
void tmAAttribTree::AddControl (const ProjectDefMemoryFields & fieldinfo)
{
	tmAAttribCtrl * mypControl = NULL;
	switch (fieldinfo.m_FieldType)
	{
		case TM_FIELD_INTEGER:
			tmAAttribCtrlInteger * mypControlInt = new tmAAttribCtrlInteger(this, fieldinfo);
			mypControl = mypControlInt;
			break;
			
		case TM_FIELD_FLOAT:
			tmAAttribCtrlFloat * mypControlFloat = new tmAAttribCtrlFloat(this, fieldinfo);
			mypControl = mypControlFloat;
			break;
			
		case TM_FIELD_ENUMERATION:
			tmAAttribCtrlEnum * mypControlEnum = new tmAAttribCtrlEnum(this, fieldinfo);
			mypControl = mypControlEnum;
			break;
			
			
		default: // tmfieldText
			tmAAttribCtrlText * mypControlText = new tmAAttribCtrlText(this, fieldinfo);
			mypControl = mypControlText;
			break;
	}
	//mypControl->SetProperties(fieldinfo);
	
	
	wxTreeMultiWindowInfo wndinfo (0, 0,0);
	// add subitem to root
    AppendWindow(m_ActualNode, mypControl, _T(""), wndinfo);
	
	
	/*wxPanel* m_panel4;
	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText8;
	m_staticText8 = new wxStaticText( m_panel4, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	//m_staticText8->Wrap( -1 );
	bSizer24->Add( m_staticText8, 0, wxALL, 5);  //wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	/*wxTextCtrl* m_textCtrl4;
	m_textCtrl4 = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_textCtrl4->SetSize(200, -1);
	m_textCtrl4->SetMinSize(wxSize(200,-1));
	bSizer24->Add( m_textCtrl4, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );*/
	
	/*wxChoice* m_choice3;
	wxString m_choice3Choices[] = { _("test1"), _("test2"), _("test3") };
	int m_choice3NChoices = sizeof( m_choice3Choices ) / sizeof( wxString );
	m_choice3 = new wxChoice( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice3NChoices, m_choice3Choices, 0 );
	m_choice3->SetSelection( 0 );
	bSizer24->Add( m_choice3, 0, wxALL|wxEXPAND, 5 );*/
	
	
	/*tmAAttribCtrlText * myTree;
	myTree = new tmAAttribCtrlText(this, wxID_ANY);
	myTree->SetProperties(fieldinfo);*/
	
	/*tmAAttribCtrlInteger * myTree;
	myTree = new tmAAttribCtrlInteger(this, wxID_ANY);
	myTree->SetProperties(fieldinfo);*/
	
	/*tmAAttribCtrlFloat * myTree;
	myTree = new tmAAttribCtrlFloat(this, wxID_ANY);
	myTree->SetProperties(fieldinfo);*/
	
	
	
	/*wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	wxRadioButton* m_radioBtn4;
	m_radioBtn4 = new wxRadioButton( m_panel4, wxID_ANY, _("incertain"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer25->Add( m_radioBtn4, 0, wxALL, 5 );
	
	wxRadioButton* m_radioBtn5;
	m_radioBtn5 = new wxRadioButton( m_panel4, wxID_ANY, _("inactif"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer25->Add( m_radioBtn5, 0, wxALL, 5 );
	
	wxRadioButton* m_radioBtn6;
	m_radioBtn6 = new wxRadioButton( m_panel4, wxID_ANY, _("actif"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer25->Add( m_radioBtn6, 0, wxALL, 5 );
	
	bSizer24->Add( bSizer25, 1, wxEXPAND, 5 );
	
	
	m_panel4->SetSizer( bSizer24 );
	m_panel4->Layout();
	bSizer24->Fit( m_panel4 );*/
	
	
	
}

