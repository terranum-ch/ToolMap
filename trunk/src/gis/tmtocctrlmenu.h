/***************************************************************************
							tmtocctrlmenu.h
				Display contextual menu for item in the TOC Ctrl
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


#ifndef _TM_TOCCTRLMENU_H_
#define _TM_TOCCTRLMENU_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmtocctrl.h"
#include <wx/menu.h>

class tmLayerProperties;


const int ID_TOCMENU_REMOVE					= 20100;
const int ID_TOCMENU_SHOW_VERTEX_BEGIN_END	= 20101;
const int ID_TOCMENU_SHOW_VERTEX_ALL		= 20102;
const int ID_TOCMENU_SHOW_VERTEX_NONE		= 20103;
const int ID_TOCMENU_PROPERTIES				= 20104;




class tmTOCCtrlMenu : public wxMenu
	{
	private:
		void CreateTOCBasic (bool bIsGeneric);
		void CreateTOCShowVertex (PRJDEF_LAYERS_TYPE spattype);
		void CreateTOCProperties ();
		
	protected:
		void CreateTOCContextMenu(PRJDEF_LAYERS_TYPE spattype, bool bIsGeneric);
		
		
	public:
		tmTOCCtrlMenu(){;}
		tmTOCCtrlMenu(tmLayerProperties * item);
		~tmTOCCtrlMenu();
	};





#endif
