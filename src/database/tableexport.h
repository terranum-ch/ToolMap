/***************************************************************************
 tableexport.h
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
#ifndef _TABLEEXPORT_H
#define _TABLEEXPORT_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/filename.h>




class DataBase;

class TableExport {
private:
    DataBase * m_Database;
    wxString m_Encoding;
    
    
public:
    TableExport(DataBase * database);
    virtual ~TableExport();
    
    void SetEncoding(const wxString & encoding){m_Encoding = encoding;}
    bool ExportCSV(const wxString & tablename, const wxFileName & path, int limit = wxNOT_FOUND, bool addtablename = false);
    
};
#endif
