/***************************************************************************
 tmimportcsv.h
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TMIMPORTCSV_H
#define _TMIMPORTCSV_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include "../core/prjdefmemmanage.h"
#include "tmimport.h"

class tmImportCSV : public tmImport {
  private:
    int m_Xcolumn;
    int m_Ycolumn;
    wxFileInputStream* m_FileStream;
    wxTextInputStream* m_TextStream;
    wxArrayString m_Fields;

    bool _ResetReading();

    bool _GetNextData(wxArrayString& tokenArray);

    bool _GetCoordinates(const wxArrayString& tokenArray, double& x, double& y);

    bool _ImportToPointLayer(DataBaseTM* database, PrjDefMemManage* prj, wxProgressDialog* progress = nullptr);

  public:
    tmImportCSV();

    virtual ~tmImportCSV();

    virtual bool Open(const wxFileName& filename);

    virtual bool Import(DataBaseTM* database, PrjDefMemManage* prj, wxProgressDialog* progress = nullptr);

    virtual bool GetExistingAttributeValues(const wxString& attName, wxArrayString& values);

    virtual bool IsOk();

    void ListFields();

    wxArrayString GetFieldsList();

    void GuessXYfields();

    virtual bool GetFieldNames(wxArrayString& Fields);

    int GetColumnX() {
        return m_Xcolumn;
    }

    int GetColumnY() {
        return m_Ycolumn;
    }

    void SetXYColumn(int x, int y);

    virtual wxArrayInt GetTargetSupported();
};

#endif
