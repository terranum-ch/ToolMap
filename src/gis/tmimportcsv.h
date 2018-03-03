/***************************************************************************
 tmimportcsv.h
 
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

#ifndef _TMIMPORTCSV_H
#define _TMIMPORTCSV_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/txtstrm.h>
#include <wx/wfstream.h>


#include "tmimport.h"

class tmImportCSV : public tmImport
{
private:
    int m_Xcolumn;
    int m_Ycolumn;
    wxFileInputStream *m_FileStream;
    wxTextInputStream *m_TextStream;

    bool _ResetReading();

    bool _GetNextData(double &x, double &y);

    bool _ImportToPointLayer(DataBaseTM *database, wxProgressDialog *progress = NULL);

    bool _ImportToLineLayer(DataBaseTM *database, wxProgressDialog *progress = NULL);


public:
    tmImportCSV();

    virtual ~tmImportCSV();

    virtual bool Open(const wxFileName &filename);

    virtual bool Import(DataBaseTM *database, wxProgressDialog *progress = NULL);

    virtual bool IsOk();

    wxArrayString ListColumns();

    int GetColumnX()
    { return m_Xcolumn; }

    int GetColumnY()
    { return m_Ycolumn; }

    void SetColumn(int x, int y);

    virtual wxArrayInt GetTargetSupported();
};

#endif
