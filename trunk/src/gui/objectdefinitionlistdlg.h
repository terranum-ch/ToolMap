/***************************************************************************
 objectdefinitionlistdlg.h
 Contain description of class used for ProjectEditObjectDefinitionDLG
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

#ifndef _OBJECTDEFINITIONLISTDLG_H_
#define _OBJECTDEFINITIONLISTDLG_H_

#include "wx/valtext.h"
#include "wx/statline.h"
//#include "listgenreport.h"				// for base listgenreport definition
#include "listgenreport_status.h"		// for list with status 
#include "../core/tmvalidator.h"		// for controls validation
#include "../core/projectdefmemory.h"
#include "../core/prjdefmemmanage.h"

class DataBaseTM;

#define ID_OBJECTDEFINITIONLISTDLG 10030
#define ID_DLGODD_LIST_LYR_NAME 10265
#define ID_DLGODD_FREQUENCY 10266
#define SYMBOL_OBJECTDEFINITIONLISTDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_OBJECTDEFINITIONLISTDLG_TITLE _("Edit Object Kind")
#define SYMBOL_OBJECTDEFINITIONLISTDLG_IDNAME ID_OBJECTDEFINITIONLISTDLG
#define SYMBOL_OBJECTDEFINITIONLISTDLG_SIZE wxSize(400, 300)
#define SYMBOL_OBJECTDEFINITIONLISTDLG_POSITION wxDefaultPosition


/******************************** EDIT OBJECT DEFINITION DLG ************************/ 
class ObjectDefinitionListDlg: public wxDialog
{
private:
	int m_ParentListType;
	DataBaseTM * m_pDatabase;
	
	// Controls identifiers
	wxTextCtrl* m_DLGODD_Code;
    wxTextCtrl* m_DLGODD_Description;
    wxChoice* m_DLGODD_List_Lyr_Name;
    wxRadioBox* m_DLGODD_Frequency;
	wxButton * m_DLGODD_SaveBtn;
	
	// event function
	void OnIdleActivateOKButton (wxIdleEvent & event);
	
	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();
	
	void _SetValidator();
	
	// used for data transfert
	ProjectDefMemoryObjects * m_ObjectObj;
	
    DECLARE_DYNAMIC_CLASS( ObjectDefinitionListDlg )
    DECLARE_EVENT_TABLE()
    
public:
    
	ObjectDefinitionListDlg();
    ObjectDefinitionListDlg( wxWindow* parent,
							int iListlayerType,
							DataBaseTM * DBHandler,
							wxWindowID id = SYMBOL_OBJECTDEFINITIONLISTDLG_IDNAME, 
							const wxString& caption = SYMBOL_OBJECTDEFINITIONLISTDLG_TITLE,
							const wxPoint& pos = SYMBOL_OBJECTDEFINITIONLISTDLG_POSITION,
							const wxSize& size = SYMBOL_OBJECTDEFINITIONLISTDLG_SIZE,
							long style = SYMBOL_OBJECTDEFINITIONLISTDLG_STYLE );
	~ObjectDefinitionListDlg();
    
    
	bool Create( wxWindow* parent,
				wxWindowID id = SYMBOL_OBJECTDEFINITIONLISTDLG_IDNAME,
				const wxString& caption = SYMBOL_OBJECTDEFINITIONLISTDLG_TITLE,
				const wxPoint& pos = SYMBOL_OBJECTDEFINITIONLISTDLG_POSITION,
				const wxSize& size = SYMBOL_OBJECTDEFINITIONLISTDLG_SIZE,
				long style = SYMBOL_OBJECTDEFINITIONLISTDLG_STYLE );
    
    void Init();
    
    void CreateControls();
	
	void SetMemoryObjectObject(ProjectDefMemoryObjects * myObjectObj){m_ObjectObj = myObjectObj;}
	
};


/******************************** LIST OF OBJECT DEFINITION  ************************/
class ObjectDefinitionList : public ListGenReportWithStatus
{
private:
    PRJDEF_LAYERS_TYPE m_layertype;
    DataBaseTM * m_DBHandler;
    wxCheckBox * m_CheckBox;
    wxChoice   * m_ChoiceLayer;
    int m_ListSpatType;
    
    // get a memory layer object for storing changes 
    PrjDefMemManage * m_MemoryObject;
    ProjectDefMemoryObjects * m_ObjectObj;
    
    // index for adding items
    static int iAddItems;
    
    // init list with database values
    bool SetListText (int ilayertype);
    wxCheckBoxState GetFreqStatus (int iIndex = -1);
    wxString	GetLayerStatus (int iIndex = -1);
    
    
    // derived functions to implement 
    virtual void BeforeAdding();
    virtual void AfterAdding (bool bRealyAddItem);
    virtual void BeforeDeleting ();
    virtual void BeforeEditing ();
    virtual void AfterEditing (bool bRealyEdited);
    
    bool m_bAscending [4];
    virtual void OnSortColumns(wxListEvent & event);
    
    
    // event functions
    void OnItemSelectChange (wxListEvent & event);
    DECLARE_EVENT_TABLE()
    
public:
    /// Constructor
    ObjectDefinitionList(wxWindow * parent,
                         wxWindowID id,
                         PrjDefMemManage * memory,
                         PRJDEF_LAYERS_TYPE paneltype,
                         DataBaseTM * database,
                         wxArrayString * pColsName, 
                         wxArrayInt * pColsSize=NULL,
                         wxSize size = wxDefaultSize);
    
    /// Destructor
    ~ObjectDefinitionList(); 
    
    void SetObjectToList (ProjectDefMemoryObjects * object, int iIndex = -1);
    void GetObjectFromList (ProjectDefMemoryObjects * object, int iIndex);
    
    void SetListSpatialType (int iSpatialType) {m_ListSpatType = iSpatialType;}
    
    //virtual int ImportParsedFileToListCtrl(const wxString & filename, 
    //									   const int & FilterIndex);
    
    // pass pointer to ctrls to the list
    void SetListCtrls (wxChoice * layerchoice ,wxCheckBox * checkboxfreq = NULL);
    
    void SetFreqStatus (int frequency, wxArrayLong * iIndexes);
    void SetLayerStatus (const wxString & layer, wxArrayLong * iIndexes);
    
    
};


#endif

