/***************************************************************************
								tmaattribctrls.h
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


#ifndef _TM_AATTRIBCTRLS_H_
#define _TM_AATTRIBCTRLS_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../components/wxtreemultictrl/wxTreeMultiCtrl.h"	// for wxTreeMultiCtrl
#include "../components/wxtreemultictrl/TreeMultiItemWindow.h"


class tmAAttribTree : public wxTreeMultiCtrl
	{
	private:
	protected:
	public:
		tmAAttribTree();
		tmAAttribTree(wxWindow * parent,
					  wxWindowID id = -1,
					  const wxPoint & pos = wxDefaultPosition,
					  const wxSize & size = wxDefaultSize,
					  long style = wxTMC_DEFAULT_STYLE,
					  const wxValidator & validator = wxDefaultValidator,
					  const wxString & name = wxTreeMultiCtrlNameStr);
		bool Create (wxWindow * parent,
					 wxWindowID id = -1,
					 const wxPoint & pos = wxDefaultPosition,
					 const wxSize & size = wxDefaultSize,
					 long style = wxTMC_DEFAULT_STYLE,
					 const wxValidator & validator = wxDefaultValidator,
					 const wxString & name = wxTreeMultiCtrlNameStr);
		~tmAAttribTree();
};





#endif
