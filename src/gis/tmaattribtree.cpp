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
	//CreateRoot();
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
	
	/*PrjMemFieldCodedValArray myArray;
	ProjectDefMemoryFieldsCodedVal myVal;
	myVal.m_ValueName = _T("Actif");
	myArray.Add(myVal);
	myVal.m_ValueName = _T("Inactif");
	myArray.Add(myVal);
	myVal.m_ValueName = _T("Tassé");
	myArray.Add(myVal);	
	myVal.m_ValueName = _T("Mangé");
	myArray.Add(myVal);*/
	
	ProjectDefMemoryFields myField1;
	myField1.m_Fieldname = _T("Coucou");
	myField1.m_FieldType = TM_FIELD_TEXT;
	myField1.m_FieldPrecision = 10;
	//myField1.m_pCodedValueArray = & myArray;
	
	
	PrjMemFieldCodedValArray myArray2;
	ProjectDefMemoryFieldsCodedVal myVal2;
	myVal2.m_ValueName = _T("Actif");
	myArray2.Add(myVal2);
	myVal2.m_ValueName = _T("Inactif");
	myArray2.Add(myVal2);

	
	
	ProjectDefMemoryFields myField2;
	myField2.m_Fieldname = _T("Coucou2");
	myField2.m_FieldType = TM_FIELD_ENUMERATION;
	myField2.m_pCodedValueArray = & myArray2;

	
	//myField2.m_pCodedValueArray = &myArray;*/
	
		
/*	
	myField2.m_FieldPrecision = 6;
	myField2.m_FieldScale = 2;

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
	
	myField.m_pCodedValueArray = &myArray;*/

	
	AddLayerNode(_T("failles"));
	tmAAttribCtrl * myCtrl = AddControl(myField1);
	myCtrl->SetControlValue(_T("Inactif"));
	
	AddLayerNode(_T("tests"));
	myCtrl = AddControl(myField2);
	myCtrl->SetControlValue(_T("Inactif"));
	//AddControl(myField);
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
 @brief Iterate all controls
 @param restart read controls from the begining ?
 @return  A valid tmAAttribCtrl pointer (don't destroy) or NULL if no more
 controls
 @author Lucien Schreiber (c) CREALP 2009
 @date 18 March 2009
 *******************************************************************************/
tmAAttribCtrl * tmAAttribTree::IterateAllCtrl (bool restart)
{
	if (restart == true)
	{
		m_ActualNode = GetFirstRoot();
		wxASSERT(m_ActualNode.IsOk());
	}
	
	m_ActualNode = GetNext(m_ActualNode);
	if (!m_ActualNode.IsOk())
		return NULL;
	
	if (m_ActualNode.IsWindowItem()==false)
	{
		// if we are passing on the parent branch
		IterateAllCtrl(false);
	}
	
	
	tmAAttribCtrl * myCtrl = (tmAAttribCtrl *) GetWindow(m_ActualNode);
	return myCtrl;

}



/***************************************************************************//**
 @brief Add control into actual node
 @param fieldinfo All fields info such as Type, name, etc.
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 March 2009
 *******************************************************************************/
tmAAttribCtrl * tmAAttribTree::AddControl (const ProjectDefMemoryFields & fieldinfo)
{
	tmAAttribCtrl * mypControl = NULL;
	tmAAttribCtrlInteger * mypControlInt;
	tmAAttribCtrlFloat * mypControlFloat;
	tmAAttribCtrlEnum * mypControlEnum;
	tmAAttribCtrlText * mypControlText;
	tmAAttribCtrlDate * mypControlDate;
	tmAAttribCtrlSafeDate * mypControlSafeDate;
	
	switch (fieldinfo.m_FieldType)
	{
		case TM_FIELD_INTEGER:
			mypControlInt = new tmAAttribCtrlInteger(this, fieldinfo);
			mypControl = mypControlInt;
			break;
			
		case TM_FIELD_FLOAT:
			mypControlFloat = new tmAAttribCtrlFloat(this, fieldinfo);
			mypControl = mypControlFloat;
			break;
			
		case TM_FIELD_ENUMERATION:
			mypControlEnum = new tmAAttribCtrlEnum(this, fieldinfo);
			mypControl = mypControlEnum;
			break;
			
		case TM_FIELD_DATE:
#ifdef __WXOSX__
			mypControlSafeDate = new tmAAttribCtrlSafeDate(this, fieldinfo);
			mypControl = mypControlSafeDate;
#else
			mypControlDate = new tmAAttribCtrlDate(this, fieldinfo);
			mypControl = mypControlDate;
#endif
			
			break;
			
		default: // tmfieldText
			mypControlText = new tmAAttribCtrlText(this, fieldinfo);
			mypControl = mypControlText;
			break;
	}

	wxASSERT (mypControl);
	
	wxTreeMultiWindowInfo wndinfo (0,0,0);
	
	
	// add subitem to root
    AppendWindow(m_ActualNode, mypControl, _T(""), wndinfo);
	return mypControl;
}

