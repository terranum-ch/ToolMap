/***************************************************************************
 tmimportgis.h
 
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

#ifndef _TMIMPORTGIS_H
#define _TMIMPORTGIS_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "tmimport.h"
#include "../core/prjdefmemmanage.h"

class tmGISDataVector;

class tmImportGIS : public tmImport
{
private:
    tmGISDataVector *m_Vector;

public:
    tmImportGIS();

    virtual ~tmImportGIS();

    virtual bool Open(const wxFileName &filename);

    virtual bool Import(DataBaseTM *database, PrjDefMemManage *prj, wxProgressDialog *progress = NULL);

    virtual bool GetExistingAttributeValues(const wxString &attName, wxArrayString &values);

    virtual wxArrayInt GetTargetSupported();

    virtual bool GetFieldNames(wxArrayString &Fields);
};

#endif
