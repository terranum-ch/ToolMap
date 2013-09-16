/***************************************************************************
								attribution_obj_type.h
                    Display wxAui panel for attribution
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



#ifndef ATTRIBUTION_OBJ_TYPE_H
#define ATTRIBUTION_OBJ_TYPE_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "managed_aui_wnd.h"
#include "wx/notebook.h"
#include "wx/datectrl.h"
#include "wx/dateevt.h"
#include <wx/srchctrl.h>
#include "tmchecklistboxrank.h"			// for advanced checklistbox
#include "../core/projectdefmemory.h"	// for item type (LINE, POINT, ...)
#include <wx/choicebk.h>				// for choicebook

class DataBaseTM;
class wxFlatButton;

// EVENT DECLARATION FOR COMMUNICATION WITH tmAttributionManager
//DECLARE_EVENT_TYPE(tmEVT_ATTRIBUTION_BTN_PRESSED, -1)
DECLARE_EVENT_TYPE(tmEVT_INFO_BTN_PRESSED, -1)
DECLARE_EVENT_TYPE(tmEVT_ADD_BTN_PRESSED, -1)
DECLARE_EVENT_TYPE(tmEVT_REMOVE_BTN_PRESSED, -1)


#define ID_DLG_OBJ_ATTRIBUTION 10039
#define ID_NOTEBOOK2 10040
#define ID_PANEL4 10041
#define ID_NOTEBOOK3 10042
#define ID_PANEL5 10043
#define ID_CHECKLISTBOX4 10045
#define ID_PANEL6 10044
#define ID_CHECKLISTBOX3 10063
#define ID_PANEL7 10057
#define ID_CHECKLISTBOX2 10060
#define ID_TEXTCTRL6 10062
#define ID_BUTTON9 10061
#define ID_PANEL8 10058
#define ID_CHECKLISTBOX1 10059
#define ID_PANEL23 10139
#define ID_CHOICE13 10203
#define ID_TEXTCTRL19 10204
#define ID_CHOICE14 10205
#define ID_DATEPICKERCTRL 10214
#define ID_TEXTCTRL21 10206
#define ID_CHECKBOX7 10209
#define ID_SEARCHCTRL1 10053
#define ID_CHECKBOX9 10054
#define ID_DLG_OBJ_ATTRIBUTION_BTN_INFO 10055
#define ID_DLG_OBJ_ATTRIBUTION_BTN_ATTRIBUTE 10056
#define ID_CHECKBOX5 10136
#define SYMBOL_ATTRIBOBJTYPE_PANEL_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ATTRIBOBJTYPE_PANEL_TITLE _("Object Kind")
#define SYMBOL_ATTRIBOBJTYPE_PANEL_IDNAME ID_DLG_OBJ_ATTRIBUTION
#define SYMBOL_ATTRIBOBJTYPE_PANEL_SIZE wxSize(230, 150)
#define SYMBOL_ATTRIBOBJTYPE_PANEL_POSITION wxDefaultPosition
const int ID_DLG_OBJ_ATTRIBUTION_BTN_ADD = wxWindow::NewControlId();
const int ID_DLG_OBJ_ATTRIBUTION_BTN_REMOVE = wxWindow::NewControlId();


class AttribObjType_PANEL: public ManagedAuiWnd
	{
		wxSizer * CreateControls(wxWindow * parent,
								 bool call_fit = TRUE, 
								 bool set_sizer = TRUE);

		wxAuiPaneInfo mPaneInfo;
		bool UpdateObjectList(DataBaseTM * pDB,tmCheckListBox * pList,
							  int type, short frequency = 2);
		wxWindow * m_ParentEvt;
		int m_NbFeaturesSelected;
		
		wxFlatButton * m_AttribBtn;
        wxFlatButton * m_AddBtnCtrl;
		wxFlatButton * m_InfoBtn;
		wxChoicebook * m_AttribNotebook;
        wxStaticText * m_WarningMultiFeatureCtrl;
		bool m_AutoDisplayAttributes;
		bool m_EmptyListAfterAttributes;
		bool m_EnableFullAttribution;
		wxBoxSizer* m_ButtonSizer;
		wxBoxSizer* m_AttribSizer;
		
		// event function
		//void OnAttributeBtn (wxCommandEvent & event);
		void OnAddBtn (wxCommandEvent & event);
        void OnRemoveBtn (wxCommandEvent & event);
		void OnInfoBtn (wxCommandEvent & event);
		
		void OnEditStart(wxCommandEvent & event);
		void OnEditStop (wxCommandEvent & event);
		
        
				
		DECLARE_EVENT_TABLE()
		
	public:
		AttribObjType_PANEL(wxWindow * parent, wxAuiManager * AuiManager);
		~AttribObjType_PANEL();
		
		bool UpdateObjectPointList(DataBaseTM * pDB);
		bool UpdateObjectPolyList (DataBaseTM * pDB);
		bool UpdateObjectLineList (DataBaseTM * pDB);
		
		// init list with databasae
		void SetDataBaseToList (DataBaseTM * pDB);
		
		// update  btn
		void SetAttributeBtn (int nbfeatures, bool editmode);
		void SetInfoBtn (int nbfeatures);
		
		// select correct panel
		void SetVisibleNotebook (TOC_GENERIC_NAME notebooktype);
		TOC_GENERIC_NAME GetVisibleNotebook ();
		
		// get selected values
		int GetSelectedValues (TOC_GENERIC_NAME panel_name,
							   wxArrayLong & values,
							   bool panel_freq = false);
		void SetSelectedValues (TOC_GENERIC_NAME panel_name,
								const wxArrayLong & values,
								bool panel_freq = false);
		
		bool IsAutoDisplayAttributeChecked();
		bool IsEmptyListValuesRequired(){return m_EmptyListAfterAttributes;}
		bool IsFullAttributionEnabled(){return m_EnableFullAttribution;}
		
		void EmptyListValues();
		
		//  menu event function
		void OnDisplayAttributesAuto(wxCommandEvent & event);
		void OnEmptyListAffterAttributes (wxCommandEvent & event);
		void OnFullAttribution (wxCommandEvent & event);
		
		// controls
		tmCheckListBoxRank * m_pObjList_PT;
		tmCheckListBoxRank * m_pObjList_PLG;
		tmCheckListBoxRank * m_pObjList_L_Freq;
		tmCheckListBoxRank * m_pObjList_L_NoFreq;

		
	};

#endif
