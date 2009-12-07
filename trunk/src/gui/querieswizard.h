/***************************************************************************
								querieswizard.h
                    Main part of the new query system
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
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

#ifndef _QUERIESWIZARD_H
#define _QUERIESWIZARD_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include <wx/wizard.h>
#include "queriespages.h"
#include "../gis/tmlayerpropertiesdef.h"
#include "../core/projectdefmemory.h"
#include "../database/database_tm.h"


class QueriesPageIntro;
class QueriesPageLayer;


enum tmQUERIES_TYPE {
	QUERY_LAYERS = 0,
	QUERY_SELECTED,
	QUERY_GENERIC,
	QUERY_SQL	
};



class QueriesWizard : public wxWizard {
private:
    
    //Used by all panels
    wxString m_QueryName;
    //Used by all panels
    wxString m_QuerySQL;
    //Layers Based panel only
    long m_QueryLayerID;
    //Expert panel only
    TOC_GENERIC_NAME m_QueryLayerType;
    long m_QueryObjectID;
    
    PrjMemLayersArray m_QueryFieldsLayers;
    wxArrayString m_QueryFieldsValues;
	DataBaseTM * m_pDB;
	
	// Wizard's page
	QueriesPageIntro * m_PageIntro;
	
	void _Init();
	
public:
	//Used by first panel
    tmQUERIES_TYPE m_QueryType;
	long m_QueryObjectGeomID;

public:
	
    QueriesWizard(wxWindow * parent, DataBaseTM * database, int id = wxID_ANY);
    ~QueriesWizard();
	virtual int ShowWizard();
	
	
	
};
#endif
