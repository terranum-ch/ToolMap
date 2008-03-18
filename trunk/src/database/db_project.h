/***************************************************************************
								db_project.h.h
			deals with the creation and modification of a database project
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

/***************************************************************************//**
 @file db_project.h
 @brief Deals with the project in database
 @details Contain class for creating and modifying project in the database.
 @author Lucien Schreiber (c) CREALP 2007
 @date 23 January 2008
 *******************************************************************************/


#ifndef DB_PROJECT_H
#define DB_PROJECT_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "database_tm.h"
#include "../core/prjdefmemmanage.h" 

class DatabaseNewPrj : public DataBaseTM
{
private:
	// private members
	PrjDefMemManage * pPrjDefinition;
	
	// private functions
	void InitMembersValues();
	//bool CreateDefaultTables ();
	
	
public:
	// constructors and destructor
	DatabaseNewPrj();
	DatabaseNewPrj(PrjDefMemManage * myPrjDefinition);
	~DatabaseNewPrj();
	
	
	// setter, getter functions
	void SetPrjDefMemory (PrjDefMemManage * myPrjDefinition) {pPrjDefinition = myPrjDefinition;}
	bool IsPrjDefMemorySet();
	
	bool CreateEmptyProject();
	bool PassProjectDataToDB();
	
	
	
};

#endif
