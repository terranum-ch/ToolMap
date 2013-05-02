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
#include "tmmanagerevent.h"				// for shared event with other manager
//#include "tmrenderer.h"						// for GIS rendering
#include "../core/tmsnappingmemory.h"		// for snapping 
#include "../gis/tmgisdatavectormemory.h"	// for storing editing data in memory
#include "../gui/editvertex_dlg.h"			// for editing vertex position
#include "../gis/tmattributiondataline.h"	// for getting attribution (merging lines)
#include "../gis/tmdraweredit.h"			// for drawing line in modification
#include <wx/list.h>						// for iterating lists
#include "../gis/tmpointorientattrib.h"		// for oriented points
#include <wx/notifmsg.h>


const int tmEM_CONTEXTMENU_VERTEX_INSERT =	22200;
const int tmEM_CONTEXTMENU_VERTEX_DELETE =	22201;
const int tmEM_CONTEXTMENU_LINE_SAVE =		22202;
const int tmEM_CONTEXTMENU_LINE_CANCEL =	22203;

class ToolMapFrame;
class DataBaseTM;
class tmRenderer;


class tmSharedNodeEdit {
private:
    long m_LineID;
    int m_VertexID;
    wxPoint m_CoordVertexPrevious;
    wxPoint m_CoordVertex;
	
	
public:
    tmSharedNodeEdit(long lineid, int vertexid, const wxPoint & coord, const wxPoint & coordprevious);
    virtual ~tmSharedNodeEdit();

    void DrawLine(wxClientDC * dc, wxPoint * point = NULL);
	
    long GetLineID (){return m_LineID;}
    int GetVertexID(){return m_VertexID;}
	void SetCoordVertex (const wxPoint & point){m_CoordVertex = point;}
};
WX_DECLARE_OBJARRAY(tmSharedNodeEdit, tmArraySharedNodes);




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
		ToolMapFrame * m_ParentEvt;
		tmTOCCtrl * m_TOC;
		tmSelectedDataMemory * m_SelectedData;
		tmRenderer * m_Renderer;
		tmGISDataVectorMemory * m_GISMemory;
		bool m_EditStarted;
		tmDrawerEditLine m_DrawLine;
		int m_INSDELVertex;
		wxRealPoint m_INSVertexPos;
		tmPointOrientAttrib m_OrientedPt;
		wxPoint m_LastMousePos;
		tmArraySharedNodes m_SharedNodes;
		wxOverlay m_OverlaySharedNodes;
		
		// defined by function or event.
		DataBaseTM * m_pDB;
		tmSnappingMemory * m_SnapMem;
		tmGISScale * m_Scale;
				
		// FUNCTIONS
		void InitMemberValues();
		
		// Extern EVENT function
		void OnSnappingChange (wxCommandEvent & event);
		void OnViewUpdated (wxCommandEvent & event);
		void OnDrawDown(wxCommandEvent & event);
		void OnDrawUp (wxCommandEvent & event);
		void OnDrawMove (wxCommandEvent & event);
		void OnEditStart (wxCommandEvent & event);
		void OnEditStop (wxCommandEvent & event);
		void OnDrawFeatureValidate (wxCommandEvent & event);
		void OnDrawFeatureEscape (wxCommandEvent & event);
		void OnCutLines (wxCommandEvent & event);
		void OnShowVertexPosition (wxCommandEvent & event);
		void OnModifySearch (wxCommandEvent & event);
		void OnModifyMove (wxCommandEvent & event);
		void OnModifyUp (wxCommandEvent & event);
		void OnModifyMenu (wxCommandEvent & event);
		void OnSetRenderFocus (wxCommandEvent & event);
		void OnEditSharedDown (wxCommandEvent & event);
		void OnEditSharedUp (wxCommandEvent & event);
		void OnEditSharedMove(wxCommandEvent & event);
		
		void OnOrientedPtsDown(wxCommandEvent & event);
		void OnOrientedPtsMove (wxCommandEvent & event);
		void OnOrientedPtsUp (wxCommandEvent & event);
		
		// menu event
		void OnMenuInsertVertex(wxCommandEvent & event);
		void OnMenuDeleteVertex(wxCommandEvent & event);
		
		// member tool function
		void DisplayRendererSnappingTolerence();
		
		// internal verification
		bool IsCorrectLayerSelected();
		bool IsObjectSelected();
		bool IsObjectMinNumberSelected (unsigned int iNumbermin = 1);
		bool IsLayerTypeSelected (int layertype = LAYER_SPATIAL_LINE);
		
		// modify function
		bool EMModifySearchLine(const wxRealPoint & pt);
		bool EMModifySearchPoint(const wxRealPoint & pt);
		
		
		// snapping function
		bool EMGetSnappingCoord (wxRealPoint & pt);
		wxRealPoint * EMIterateAllSnappingLayers(const wxRealPoint & clickedpoint);
		void EMDrawSnappingStatus (const wxPoint & pt);
        wxRealPoint * EMSearchLineMemorySnapping (const wxRealPoint & clickedpoint);
		
		// menu functions
		void EMCreateMenu(wxMenu & menu);
		void EMGetMenuLine(wxMenu & menu);
		void EMGetMenuVertex(wxMenu & menu);
		
		// loading functions
		bool EMLoadModifyData();
		
		// adding, storing object
		bool AddLineVertex (const wxRealPoint & pt);
		bool AddPointVertex (const wxRealPoint & pt);
		long StorePoint (const wxRealPoint & pt);
		bool UpdatePoint();
		long StoreLine ();
		bool UpdateLine();
		
		// drawing editing object
		void DrawLastSegment ();
		void DrawEditBitmapLine ();
		void DrawMemoryData(bool refresh);
		
        // checking
        wxArrayLong _GetErrorLines(wxArrayLong linetocheck);
		
		
		DECLARE_EVENT_TABLE()
	protected:
		
	public:
		// ctor - dtor
		tmEditManager(ToolMapFrame * parent,
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
        void OnToolBezier();
		void OnToolModify ();
		void OnToolEditShared();
		void OnToolCutLines();
		void OnToolOrientedPoint();
		
		// validiting editing / modfication
		bool IsDrawingAllowed();
		bool IsModifictionAllowed();
		bool IsLayerType(int layertype = LAYER_SPATIAL_LINE);
        bool IsMultipleModifictionAllowed();

		
		// deleting selected
		bool DeleteSelected(bool Clearselection = true);
		bool UndoLastVertex ();
		bool HasLastVertex();
		int GetSelectionCount();
		
		// search function (from tmLayerManager)
		bool SelectedSearch (const wxPoint & screenpt);  
		
		// segmentation
		bool CreateIntersections ();
		
		// editing vertex dialog
		bool EditVertexPosition ();
		
		// merging lines
		bool MergeSelectedLines ();
		bool FlipLine();
		
        
        void BezierAddPoint(const tmBezierPointInt & point);
        
	};




#endif
