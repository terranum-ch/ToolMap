/***************************************************************************
								tmeditmanager.h
				Deals with editing data in construction layers
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


#ifndef _TM_EDIT_MANAGER_H_
#define _TM_EDIT_MANAGER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "tmtocctrl.h"						// for TOC ctrl 
#include "tmselecteddatamemory.h"			// for selection data
#include "../database/database_tm.h"		// for database
//#include "tmmanagerevent.h"				// for shared event with other manager
#include "tmrenderer.h"						// for GIS rendering
#include "../core/tmsnappingmemory.h"		// for snapping 
#include "../gis/tmgisdatavectormemory.h"	// for storing editing data in memory
#include "../gui/editvertex_dlg.h"			// for editing vertex position
#include "../gis/tmattributiondataline.h"	// for getting attribution (merging lines)


/***************************************************************************//**
 @brief Deals with editing data
 @details This class is in charge of all stuff related to the editing process
 (points, lines)
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
class tmEditManager : public wxEvtHandler
	{
	private:
		// defined by ctor
		wxWindow * m_ParentEvt;
		tmTOCCtrl * m_TOC;
		tmSelectedDataMemory * m_SelectedData;
		tmRenderer * m_Renderer;
		tmGISDataVectorMemory * m_GISMemory;
		bool m_EditStarted;
		wxPoint m_OldVertexPos;
		
		
		// defined by function or event.
		DataBaseTM * m_pDB;
		tmSnappingMemory * m_SnapMem;
		tmGISScale * m_Scale;
		
				
		// FUNCTIONS
		void InitMemberValues();
		
		// Extern EVENT function
		void OnSnappingChange (wxCommandEvent & event);
		void OnViewUpdated (wxCommandEvent & event);
		void OnDrawClicked (wxCommandEvent & event);
		void OnEditStart (wxCommandEvent & event);
		void OnEditStop (wxCommandEvent & event);
		void OnDrawFeatureStop (wxCommandEvent & event);
		void OnCutLines (wxCommandEvent & event);
		void OnShowVertexPosition (wxCommandEvent & event);
		
		// member tool function
		void DisplayRendererSnappingTolerence();
		
		// internal verification
		bool IsCorrectLayerSelected();
		bool IsObjectSelected();
		bool IsObjectMinNumberSelected (unsigned int iNumbermin = 1);
		bool IsLayerTypeSelected (int layertype = LAYER_SPATIAL_LINE);
		
		// snapping function
		wxRealPoint * IterateAllSnappingLayers(const wxRealPoint & clickedpoint);
		
		// adding, storing object
		bool AddLineVertex (const wxRealPoint & pt);
		bool AddPointVertex (const wxRealPoint & pt);
		long StorePoint (const wxRealPoint & pt);
		long StoreLine ();
		
		// drawing editing object
		void DrawLastSegment ();
		void DrawEditLine ();
		
		
		
		DECLARE_EVENT_TABLE()
	protected:
		
	public:
		// ctor - dtor
		tmEditManager(wxWindow * parent,
					  tmTOCCtrl * toc,
					  tmSelectedDataMemory * seldata,
					  tmRenderer * renderer,
					  tmGISScale * scale);
		~tmEditManager();
		
		//! @brief Set database functions.
		//! If database is valid then project is open.
		void SetDatabase (DataBaseTM * database){m_pDB = database;}
	
	
		//change tool functions
		void OnToolEdit ();
		void OnToolModify ();
		void OnToolCutLines();
		
		// validiting editing / modfication
		bool IsDrawingAllowed();
		bool IsModifictionAllowed();
		
		// deleting selected
		bool DeleteSelected(bool Clearselection = true);
		bool UndoLastVertex ();
		
		// search function (from tmLayerManager)
		bool SelectedSearch (const wxPoint & screenpt);  
		
		// segmentation
		bool CreateIntersections ();
		
		// editing vertex dialog
		bool EditVertexPosition ();
		
		// merging lines
		bool MergeSelectedLines ();
	};




#endif
