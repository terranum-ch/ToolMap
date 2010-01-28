/***************************************************************************
								queriesdata.h
                   
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

#ifndef _QUERIESDATA_H
#define _QUERIESDATA_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../database/database_tm.h"
#include "../core/projectdefmemory.h"
#include "../gis/tmattributionmanager.h"


enum tmQUERIES_TYPE {
	QUERY_LAYERS,
	QUERY_OBJECTS,
	QUERY_SELECTED,
	QUERY_GENERIC,
	QUERY_SQL,
	QUERY_LINES,
	QUERY_NODES,
	QUERY_NUMBER
};


enum tmQUERIES_AATTRIBUTION_TYPE {
	AATTRIBUTION_NO,
	AATTRIBUTION_EMPTY,
	AATTRIBUTION_YES
	
};



class QueriesData {
private:
    bool _IsQueryLayersCorrect();
    bool _IsQuerySelectedCorrect();
    bool _IsQueryGenericCorrect();
    bool _IsQuerySQLCorrect();
	bool _IsQueryObjectCorrect();
    bool _IsQueryLineSizeCorrect();
    bool _IsQueryNodeNumberCorrect();
	
	
  public:
    tmQUERIES_TYPE m_QueryType;

    //Used by all panels
    wxString m_QueryName;

    //Used by all panels
    wxString m_QuerySQL;

    //Layers Based panel only
    long m_QueryLayerID;

    long m_QueryObjectID;

    long m_QueryObjectGeomID;

    
	//bool m_QueryUseFields;
	tmQUERIES_AATTRIBUTION_TYPE m_QueryFieldsStatus;
    PrjMemFieldArray m_QueryFields;
	wxArrayString m_QueryFieldsValues;

    //Expert panel only
    TOC_GENERIC_NAME m_QueryLayerType;

    int m_QueryLineSize;
	
    int m_QueryNodeNumber;
	
	
	bool m_QueryRun;


    QueriesData();

    ~QueriesData();
	
	bool IsOk();
	bool IsGenericLayer(DataBaseTM * database, long dblayerID);
	
	bool GetLayers(DataBaseTM * database, PrjMemLayersArray & layers);
	bool GetObjectsForSelection(DataBaseTM * database, PrjMemObjectsArray & objects);
	bool GetObjectsForTypes(DataBaseTM * database, PrjMemObjectsArray & objects);
	bool GetParentLayer(DataBaseTM * database, long & layerid);
	bool GetFieldsValues(DataBaseTM * database, long layerid,
						 PrjMemFieldArray & fieldsdef, wxArrayString & fieldsvalue);
	bool DeleteFieldsValue(int index);
	bool HasFieldsValues();


};
#endif
