/***************************************************************************
							tmscalectrl.cpp
                    Contain scale control component
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

#include "tmscalectrl.h"




BEGIN_EVENT_TABLE(tmScaleCtrlCombo, wxComboBox)
END_EVENT_TABLE()


tmScaleCtrlCombo::tmScaleCtrlCombo (wxWindow * parent, wxWindowID id, 
									const wxPoint & pos,
									const wxSize & size) :
wxComboBox (parent, id, _T(""), pos, size)
{
	m_ParentEvent = parent;
	
}


tmScaleCtrlCombo::tmScaleCtrlCombo()
{
	
}



void tmScaleCtrlCombo::InitScaleFromDatabase (const wxArrayLong & scale_values)
{
	// clear old values 
	Clear();
	
	wxArrayString myStringScale;
	wxString tempScale = _T("");
	
	for (unsigned int i = 0; i< scale_values.GetCount(); i++)
	{
		ScaleTM::GetScaleFromLong (scale_values.Item(i), tempScale);
		myStringScale.Add(tempScale);
	}

	Append(myStringScale);	
}



void tmScaleCtrlCombo::SetValueScale (const long & scale)
{
	wxString myScaleString = _T("");
	ScaleTM::GetScaleFromLong (scale, myScaleString);
	SetValue(myScaleString);
}



void tmScaleCtrlCombo::OnUserSetValue (wxCommandEvent & event)
{
	
}

