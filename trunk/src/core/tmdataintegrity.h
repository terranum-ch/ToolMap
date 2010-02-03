/***************************************************************************
 tmdataintegrity.h
 
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber 
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


#ifndef _TMDATAINTEGRITY_H
#define _TMDATAINTEGRITY_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../gis/tmlayerproperties.h"

class DataBaseTM;


class tmDataIntegrity {
private:
	DataBaseTM * m_pDB;
	
    wxString m_QueryTemplateType;
    wxString m_QueryTemplateAAttrib;
	
    long m_ErrorType;
    long m_ErrorAAttrib;
	
	
public:
    tmDataIntegrity(DataBaseTM * database);
	
    ~tmDataIntegrity();
	
	bool CheckType(const TOC_GENERIC_NAME & layertype);
    bool CheckAAttrib(long layerindex, const TOC_GENERIC_NAME & layertype);
	
    bool CorrectType(long layerindex, const TOC_GENERIC_NAME & layertype);
    bool CorrectAAttrib(long layerindex, const TOC_GENERIC_NAME & layertype);
	
    wxString GetLastError();
	
};
#endif
