/***************************************************************************
								queriesbuilder.h
                   
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

#ifndef _QUERIESBUILDER_H
#define _QUERIESBUILDER_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "queriesdata.h"

class DataBaseTM;

class QueriesBuilder {
  private:
    QueriesData * m_QueryData;
	bool m_IsCreated;
	
	bool _IsQueryNull();
  	bool _IsCreated();
	
	// private creating function
	bool _CreateLayersQuery();
    bool _CreateSelectionQuery();
	bool _CreateObjectQuery();
    bool _CreateGeomLineQuery();
    bool _CreateGeomNodeQuery();
    bool _CreateDuplicateQuery();

  public:
    QueriesBuilder(QueriesData * query);

    ~QueriesBuilder();

    bool IsOk();

    bool Save(DataBaseTM * database);
	bool Create(DataBaseTM * database);
};
#endif
