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
#include <wx/treectrl.h>
#include <wx/imaglist.h>
#include <wx/renderer.h>
#include <wx/dcgraph.h>


//#include "../core/projectdefmemory.h"	for PRJDEF_LAYERS_TYPE
			// for image list for toc
#include "tmtocctrlmenu.h"			// for contextual menu


// EVENT FOR TOC CTRL
DECLARE_EVENT_TYPE(tmEVT_LM_REMOVE, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_ADD, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_UPDATE, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_SHOW_PROPERTIES, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_EDIT_START, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_EDIT_STOP,-1)
DECLARE_EVENT_TYPE(tmEVT_TOC_SELECTION_CHANGED, -1)



/***************************************************************************//**
 @brief GIS TOC class
 @details This object is in charge of all operations linked to the table of
 content (TOC). Most of the events comming from the #tmTOCCtrlMenu are processed
 here except events needing direct acces to GIS data wich are processed in the 
 #tmLayerManager.
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
class tmTOCCtrl  : public wxTreeCtrl
	{
	private:
		wxTreeItemId m_root;
		wxWindow * m_ParentEvt;
		//wxTreeItemIdValue m_Cookie;
		tmTOCCtrlMenu * m_ContextMenu;
		wxTreeItemId m_ActualItemID;
		tmLayerProperties * m_EditingLayer;
        bool m_IsImageInited;
		
		// private functions
		void InitTocMemberValues();
		void LoadImageList();
		int GetSelectedPosition ();
		bool MoveLayers (const wxTreeItemId & item, int newpos);
		bool SwapLayers (const wxTreeItemId & item, int newpos);
		void StartEditing ();
		void StopEditing (bool bSentmessage = false);
		
		// graphical display
		void SetItemStyle (wxTreeItemId id, tmLayerProperties * item);
				
		// private event functions
		void OnMouseClick (wxMouseEvent & event);
		void OnMouseItemRightClick (wxTreeEvent & event);
		void OnMouseItemDoubleClick (wxTreeEvent & event);
		void OnMoveLayers (wxCommandEvent & event);
		void OnShortcutKey (wxKeyEvent & event);
		void OnLayerSelected (wxTreeEvent & event);
        
        void OnPaint(wxPaintEvent & event);
		
		// contextual menu event functions
		void OnShowProperties (wxCommandEvent & event);
		void OnVertexMenu (wxCommandEvent & event);
		void OnEditingChange (wxCommandEvent & event);
		
		// dragging functions
		wxTreeItemId m_DragItemID;
		void OnDragStart(wxTreeEvent & event);
		void OnDragStop(wxTreeEvent & event);
		
		
		DECLARE_EVENT_TABLE()
		
		
	public:
		// construcor / destructor
		tmTOCCtrl (wxWindow * parent, wxWindowID id, wxSize size,  long style);
		~tmTOCCtrl();
		
		// adding, removing layers functions
		void InsertProjectName (const wxString & prjname);
		bool InsertLayer(tmLayerProperties * item, wxTreeItemId postion = 0);
        bool UpdateLayerName (tmLayerProperties * item, const wxString & newname);
		bool RemoveLayer (wxTreeItemId position, bool bRemoveChild = TRUE);
		bool GetItemByID (wxTreeItemId & position,long searchedid);
		void ClearAllLayers();
		bool EditLayer (tmLayerProperties * newitemdata, wxTreeItemId position);
		tmLayerProperties * IterateLayers (bool ResetToLast);
		
		// counting layers only
		unsigned int GetCountLayers();
		
		// selection
		tmLayerProperties * GetSelectionLayer ();
		void SetSelectedLayer (int layerID);
		tmLayerProperties * GetLayerById (long layerid);
        tmLayerProperties * GetLayerByName (const wxString & layername);
		
		
		// public event function
		void OnRemoveItem (wxCommandEvent & event);
		
		// Project check function
		bool IsTOCReady();
		
		// expanding all child
		bool ExpandAllLayers();
		
		// Setting, Getting editing layer
		tmLayerProperties * GetEditLayer() {return m_EditingLayer;}
		void SetEditLayer(tmLayerProperties * layerprop){m_EditingLayer = layerprop;}
		

		
	};



#endif
