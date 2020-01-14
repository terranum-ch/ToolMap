/***************************************************************************
 tableexport.cpp
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tableexport.h"
#include "database.h"
#include "databaseresult.h"

TableExport::TableExport(DataBase *database)
{
    m_Database = database;
    m_Encoding = wxEmptyString;
    wxASSERT(m_Database);
}


TableExport::~TableExport()
{
}


bool TableExport::ExportCSV(const wxString &tablename, const wxFileName &path, int limit, bool addtablename)
{
    wxASSERT(m_Database);
    wxString mySeparator = ";";

    // some checks
    if (tablename.IsEmpty()) {
        wxLogError(_("Exporting table : '%s' failed!"), tablename, path.GetFullPath());
        return false;
    }

    wxString filetext = tablename;
    if (path.HasName() == true) {
        filetext = path.GetName();
    }

    // open file for export
    wxFileName myFileName(path.GetPath(), filetext, "csv");
    wxLogMessage(_("Exporting to '%s'"), myFileName.GetFullPath());

    wxFile myFile;
    if (myFile.Open(myFileName.GetFullPath(), wxFile::write) == false) {
        wxLogError(_("Opening '%s' failed!"), myFileName.GetFullPath());
        return false;
    }

    // get table data
    wxString myQuery = "SELECT * FROM " + tablename;
    if (limit != wxNOT_FOUND) {
        myQuery.Append(wxString::Format(" LIMIT %d", limit));
    }

    if (m_Database->DataBaseQuery(myQuery) == false) {
        return false;
    }

    DataBaseResult myResult;
    m_Database->DataBaseGetResults(&myResult);

    // export header
    if (addtablename == true) {
        myFile.Write(tablename + "\n");
    }


    wxArrayString myColsName;
    myResult.GetColName(myColsName);
    for (unsigned int i = 0; i < myColsName.GetCount(); i++) {
        if (m_Encoding != wxEmptyString) {
            myFile.Write(myColsName[i] + mySeparator, wxCSConv(m_Encoding));
        } else { // unicode
            myFile.Write(myColsName[i] + mySeparator);
        }
    }

    // export content
    for (unsigned long row = 0; row < myResult.GetRowCount(); row++) {
        myFile.Write("\n");
        myResult.NextRow();
        for (unsigned long col = 0; col < myResult.GetColCount(); col++) {
            wxString myText = "";
            myResult.GetValue(col, myText);
            if (m_Encoding != wxEmptyString) {
                myFile.Write(myText + mySeparator, wxCSConv(m_Encoding));
            } else { // unicode
                myFile.Write(myText + mySeparator);

            }
        }
    }

    myFile.Close();
    return true;
}

