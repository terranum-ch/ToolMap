/***************************************************************************
 siparam.h
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber
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
#ifndef _SIPARAM_H_
#define _SIPARAM_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/tokenzr.h>


class siParam {
private:
    wxString m_Separator;
    
public:
    siParam(const wxString & separator = _T("\t"));
    virtual ~siParam();
    
    wxString GetParam(const wxString & text, const wxString & paramname, bool & error);
    //bool GetParamPeer(const wxString & text, wxString & param1, wxString & param2);
    wxString GetParamByCol(const wxString & text, int column, bool & error);
    int GetParamCount(const wxString & text);
    bool IsEmpty(const wxString & text);

};
#endif
