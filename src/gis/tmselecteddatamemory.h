/***************************************************************************
								tmSelectedDataMemory.h
                    Store ID of selected data into memory 
                             -------------------
    copyright            : (C) 2008 CREALP Lucien Schreiber 
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


#ifndef _TM_SELECTEDDATAMEMORY_H_
#define _TM_SELECTEDDATAMEMORY_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include <wx/dynarray.h> // support for array
WX_DEFINE_SORTED_ARRAY_LONG(long, tmArraySortedLong); 


/***************************************************************************//**
 @brief Store ID of selected data into memory
 @details This class may be used by the #tmLayerManager for storing Seleted
 data's ID.  Only data from one layers at a time may be selected so we keep
 also the Layers ID. For futher developpement we also keep a wxBrush object
 used for drawing selected object.
 @author Lucien Schreiber (c) CREALP 2008
 @date 28 October 2008
 *******************************************************************************/
class tmSelectedDataMemory : public wxObject
	{
	private:
		long m_LayerID;
		tmArraySortedLong * m_SelectedIDs;
		wxColour m_Colour;
		
		void InitMemberValues ();
		

		
	protected:
		int Search (const long & value);
		
	public:
		// ctor(s) and destructors
		tmSelectedDataMemory();
		~tmSelectedDataMemory();
		
		// operations
		bool AddSelected (wxArrayLong * selected);
		bool Remove (wxArrayLong * selected = NULL);
		bool IsSelected (const long & value);
		unsigned int GetCount();
		void SetSelected (long selected);
		
		// getters / setters
		void SetLayerID (long layerID);
		void Clear() {m_SelectedIDs->Clear();}
		wxColour GetSelectionColour(){return m_Colour;}
		void SetSelectionColour(const wxColour & col){m_Colour = col;}
		wxColour GetSelectionColour2() {return *wxGREEN;}
		long GetSelectedLayer () {return m_LayerID;}
		wxArrayLong * GetSelectedValues ();
		long GetSelectedUnique();
		
	};





#endif
