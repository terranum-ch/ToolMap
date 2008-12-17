/***************************************************************************
								tmShortcutMemory.h
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


#ifndef _TM_SHORTCUT_MEMORY_H_
#define _TM_SHORTCUT_MEMORY_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif




class tmShortcutObject
	{
	private:
		void InitMemberValues();
		
	public:
		int m_LayerType;
		int m_ShortcutKey;
		wxString m_ShortcutDescription;
		wxArrayLong m_ShortcutValues;
		bool m_ShortcutValid;
		
		
			
		tmShortcutObject();
		~tmShortcutObject();
		
		
		
	};

// Creating a list of MemoryObjects
WX_DECLARE_OBJARRAY(tmShortcutObject, tmShortCutObjArray);




class tmShortcutMemory : public wxObject
	{
	private:
		tmShortCutObjArray m_Shortcuts;
		
		
		
	protected:
	public:
		tmShortcutMemory();
		~tmShortcutMemory();
		
		
};





#endif
