/***************************************************************************
							tmShortcutMemory.cpp
                    Store shortcut in memory for attribution
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

#include "tmshortcutmemory.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY (tmShortCutObjArray);


tmShortcutObject::tmShortcutObject()
{
	InitMemberValues();
}

tmShortcutObject::~tmShortcutObject()
{
	
}


void tmShortcutObject::InitMemberValues()
{
	m_LayerType = -1;
	m_ShortcutKey = 0;
	m_ShortcutDescription = _T("");
	m_ShortcutValues.Clear();
	m_ShortcutValid = false;
}



/******************************* tmShortcutMemory *******************/
tmShortcutMemory::tmShortcutMemory()
{

}


tmShortcutMemory::~tmShortcutMemory()
{


}

