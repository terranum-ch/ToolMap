/***************************************************************************
								tmscalectrl.h
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


#ifndef _TM_SCALECTRL_H_
#define _TM_SCALECTRL_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "../database/database_tm.h"	// for loading data from db
#include "../core/scale.h"				// for ScaleTM

// EVENT FOR SCALE CTRL
//DECLARE_EVENT_TYPE(tmEVT_SCALE_INIT_DATABASE, -1)

class tmScaleCtrlCombo : public wxComboBox 
	{
	private:
		wxWindow * m_ParentEvent;
		DECLARE_EVENT_TABLE();
	protected:
	public:
		tmScaleCtrlCombo();
		tmScaleCtrlCombo (wxWindow * parent, wxWindowID id, const wxPoint & pos = wxDefaultPosition,
						  const wxSize & size = wxDefaultSize);
		~tmScaleCtrlCombo(){;}
		
		void InitScaleFromDatabase (const wxArrayLong & scale_values);
		void SetValueScale (const long & scale);
		void OnUserSetValue (wxCommandEvent & event);
		
	};




#endif
