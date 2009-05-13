/***************************************************************************
								tooldanglingnodes.h
                    Tool engine for searching dangling nodes
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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

// comment doxygen


#ifndef _TOOL_DANGLING_NODES_H_
#define _TOOL_DANGLING_NODES_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../core/tmarraysize.h"
#include "../database/database_tm.h"

class ToolDanglingNodes : public wxObject
	{
	private:
		wxArrayRealPoints m_DanglingPts;
		DataBaseTM * m_pDB;
		
		void DNInitValues();
		
		// private check
		bool DNIsLayerCorrect(long layerid);
		
		
		
	protected:
	public:
		ToolDanglingNodes();
		ToolDanglingNodes(DataBaseTM * database);
		~ToolDanglingNodes();
		void Create (DataBaseTM * database);
		
		bool IsOk();
		bool GetDanglingNodes(wxArrayRealPoints & pts);
		
		bool Search (long layerid);
};




#endif
