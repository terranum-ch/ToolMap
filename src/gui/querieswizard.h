/***************************************************************************
 querieswizard.h
                    Main part of the new query system
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _QUERIESWIZARD_H
#define _QUERIESWIZARD_H

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include <wx/wizard.h>
#include "queriespages.h"
#include "../gis/tmlayerpropertiesdef.h"
#include "../core/projectdefmemory.h"
#include "../core/queriesdata.h"


class QueriesPageIntro;

class QueriesPageLayer;

class DataBaseTM;


class QueriesWizard : public wxWizard
{
private:
    DataBaseTM *m_pDB;
    QueriesData *m_QueryData;
    // Wizard's page
    QueriesPageIntro *m_PageIntro;


public:
    //Used by first panel
    tmQUERIES_TYPE m_QueryType;
    long m_QueryObjectGeomID;


public:

    QueriesWizard(wxWindow *parent, DataBaseTM *database, int id = wxID_ANY);

    virtual ~QueriesWizard();

    virtual int ShowWizard();

    inline QueriesData *GetData();

};


inline QueriesData *QueriesWizard::GetData()
{
    return m_QueryData;
}


#endif
