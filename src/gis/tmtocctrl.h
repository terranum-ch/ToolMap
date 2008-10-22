/***************************************************************************
							tmtocctrl.h
                    Contain description of the GIS toc ctrl
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


#ifndef _TM_TOCCTRL_H_
#define _TM_TOCCTRL_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


//#include "../core/projectdefmemory.h"	// for PRJDEF_LAYERS_TYPE
#include <wx/treectrl.h>
#include "wx/imaglist.h"			// for image list for toc
#include "tmtocctrlmenu.h"			// for contextual menu
#include "tmsymbol.h"				// for symbology





// EVENT FOR TOC CTRL
DECLARE_EVENT_TYPE(tmEVT_LM_REMOVE, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_ADD, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_UPDATE, -1) // used also for updating symbology
DECLARE_EVENT_TYPE (tmEVT_LM_SHOW_PROPERTIES, -1)



/***************************************************************************//**
 @brief GIS TOC class
 @details This object is in charge of all operations linked to the table of
 content (TOC)
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
class tmTOCCtrl  : public wxTreeCtrl
	{
	private:
		wxTreeItemId m_root;
		wxWindow * m_ParentEvt;
		wxTreeItemIdValue m_Cookie;
		tmTOCCtrlMenu * m_ContextMenu;
		wxTreeItemId m_ActualItemID;
		
		// private functions
		void InitMemberValues();
		void LoadImageList();
		
		// graphical display
		void SetItemStyle (wxTreeItemId id, tmLayerProperties * item);
				
		// private event functions
		void OnMouseClick (wxMouseEvent & event);
		void OnMouseItemRightClick (wxTreeEvent & event);
		
		
		// contextual menu event functions
		void OnShowProperties (wxCommandEvent & event);
		void OnVertexMenu (wxCommandEvent & event);
		
		DECLARE_EVENT_TABLE()
		
		
	public:
		// construcor / destructor
		tmTOCCtrl (wxWindow * parent, wxWindowID id, wxSize size,  long style);
		~tmTOCCtrl();
		
		// adding, removing layers functions
		void InsertProjectName (const wxString & prjname);
		bool InsertLayer(tmLayerProperties * item, wxTreeItemId postion = 0);
		bool RemoveLayer (wxTreeItemId position, bool bRemoveChild = TRUE);
		void ClearAllLayers();
		bool EditLayer (tmLayerProperties * newitemdata, wxTreeItemId position);
		tmLayerProperties * IterateLayers (bool ResetToLast);
		
		// moving item functions
		
		// counting layers only
		unsigned int GetCountLayers();
		
		// public event function
		void OnRemoveItem (wxCommandEvent & event);
		
		// Project check function
		bool IsTOCReady();
		

		
	};



#endif
