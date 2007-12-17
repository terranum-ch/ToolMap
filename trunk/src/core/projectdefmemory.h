/***************************************************************************
								projectdefmemory.h
                    Manage all the project definition in memory before
					Creating the database (to allow cancel operations)
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


#ifndef PROJECTDEFMEMORY_H
#define PROJECTDEFMEMORY_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


// Definitions for the field type, we define the text and  the id of 
// each string. The number of items in the list is also defined here.
static wxString PRJDEF_FIELD_TYPE_STRING[] = 
{
	_("Integer"),
	_("Float"),
	_("Text"),
	_("Date")
};
enum PRJDEF_FIELD_TYPE
{
	FIELD_INTEGER = 0,
	FIELD_FLOAT,
	FIELD_TEXT,
	FIELD_DATE
};
static const int PRJDEF_FIELD_TYPE_NUMBER = 4;

// Definition for the constrain value type
enum PRJDEF_FIELD_CONSTAIN_VALUE_TYPE
{
	FIELD_CONSTRAIN_CODED = 0,
	FIELD_CONSTRAIN_RANGE,
	FIELD_NOT_CONSTRAIN
};



class ProjectDefMemoryFields
	{
	private:
		void InitMemberValues();
		
	public:
		int m_FieldID;
		wxString m_Fieldname;
		PRJDEF_FIELD_TYPE m_FieldType;
		int m_FieldPrecision;
		int m_FieldScale;
		bool m_FieldOrientation;
		PRJDEF_FIELD_CONSTAIN_VALUE_TYPE m_FieldConstrain;
		
		ProjectDefMemoryFields();
		~ProjectDefMemoryFields();
		
	
	};





#endif
