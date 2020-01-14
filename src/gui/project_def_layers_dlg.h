/***************************************************************************
 project_def_layers_dlg.h
 Display Project Definition layers dialog (step 2 of a project definition)
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber 
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


#ifndef PROJECT_DEF_LAYERS_DLG_H
#define PROJECT_DEF_LAYERS_DLG_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include "listgenreport.h"
#include "wx/notebook.h"
#include "wxflatbutton.h"
#include "project_def_fields_dlg.h"
#include "wx/statline.h"        // for static line in ProjectDefLayersEditObjectDlg
#include "../core/textparser.h" // for text parsing during importation
#include <wx/filedlg.h>            // file selector dialog
#include "../core/prjdefmemmanage.h"

class ProjectDefFieldDlg;

class ProjectDefLayersDlg;

class ProjectDefLayersEditObjectDlg;

#define ID_DLGPDL 10007
#define ID_DLGPDL_LAYER_TYPE 10000
#define ID_DLGPDL_LAYER_NAME 10001
#define ID_NOTEBOOK 10002
#define ID_DLGPDL_PANEL_OBJ 10003
#define ID_DLGPDL_OBJ_LIST 10009
#define ID_DLGPDL_OBJECT_ADD 10010
#define ID_DLGPDL_OBJECT_REMOVE 10225
#define ID_DLGPDL_OBJECT_IMPORT 10251
#define ID_DLGPDL_PANEL_FIELDS 10008
#define ID_DLGPDL_FIELDS_LIST 10220
#define ID_DLGPDL_FIELD_ADD 10005
#define ID_DLGPDL_FIELD_REMOVE 10006
#define ID_DLGPDL_CHK_ORIENTATION 10290
#define SYMBOL_PROJECTDEFLAYERSDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PROJECTDEFLAYERSDLG_TITLE _("Thematic layer definition")
#define SYMBOL_PROJECTDEFLAYERSDLG_IDNAME ID_DLGPDL
#define SYMBOL_PROJECTDEFLAYERSDLG_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTDEFLAYERSDLG_POSITION wxDefaultPosition

class ProjectDefLayersObjectList : public ListGenReportWithDialog
{
private:
    // void OnMySelectionChange (wxListEvent & event);
    wxChoice *m_ChoiceToChange;
    int m_SpatialType;


    ProjectDefMemoryObjects *m_ObjectObj;
    PrjDefMemManage *m_pPrjDefinition;

    virtual void AfterAdding(bool bRealyAddItem);

    virtual void AfterEditing(bool bRealyEdited);

    virtual void BeforeAdding();

    virtual void BeforeDeleting();

    virtual void BeforeEditing();

    virtual void AddingValueToArray(wxArrayString &myImportedValues);

public:
    static const int ID_PARAMLIST;

    ProjectDefLayersObjectList(wxWindow *parent, wxWindowID id, wxSize size);

    ~ProjectDefLayersObjectList();

    PrjMemObjectsArray *m_ObjectsArray;

    void SetSpatialType(int spatialtype)
    { m_SpatialType = spatialtype; }

    void PassPrjDefToList(PrjDefMemManage *myPrjMemManage)
    { m_pPrjDefinition = myPrjMemManage; }

};


class ProjectDefLayersFieldsList : public ListGenReportWithDialog
{
private:
    bool m_bIsModeEditing;

    PrjDefMemManage *m_pPrjDefinition;
    ProjectDefMemoryFields *m_FieldsObj;
    wxCheckBox *m_ChkOrientation;


    virtual void AfterAdding(bool bRealyAddItem);

    virtual void AfterEditing(bool bRealyEdited);

    virtual void BeforeAdding();

    virtual void BeforeDeleting();

    virtual void BeforeEditing();
    //virtual void AddingValueToArray (wxArrayString & myImportedValues);


    // pointer to the parent dialog
    ProjectDefLayersDlg *m_DlgParent;

    wxChoice *m_ChoiceToChange;

    // event functions
    void OnItemSelectChange(wxListEvent &event);

DECLARE_EVENT_TABLE();

public:
    static const int ID_PARAMLIST;

    ProjectDefLayersFieldsList(wxWindow *parent, wxWindowID id,
                               wxSize size, ProjectDefLayersDlg *myDlg);

    ~ProjectDefLayersFieldsList();

    void PassPrjDefToList(PrjDefMemManage *myPrjMemManage)
    { m_pPrjDefinition = myPrjMemManage; }


    void SetListCtrls(wxCheckBox *pChkOrientation = NULL)
    { m_ChkOrientation = pChkOrientation; }

    void SetOrientation(int orientation, const int &index);

    bool IsOrientationAllowed(int index);
    // int GetParamType (wxString myTextParam);
    //
    // wxString GetParamType (int imyType);
    //
    // void AddItemToParamList(wxString myValue, wxString myComment, ACOMMENTPOSITION type);
    // void SetChoiceList (wxChoice * myChoiceList);
    // DECLARE_EVENT_TABLE();
};


////@begin control identifiers
#define ID_PROJECTDEFLAYERSEDITOBJECTDLG 10117
#define ID_DLGEO_CODE 10259
#define ID_DLGEO_VALUE 10260
#define ID_STATICLINE2 10261
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_TITLE _("Edit Object Kind")
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_IDNAME ID_PROJECTDEFLAYERSEDITOBJECTDLG
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_POSITION wxDefaultPosition
////@end control identifiers



class ProjectDefLayersEditObjectDlg : public wxDialog
{
private:
    void OnTextChange(wxCommandEvent &event);


    /// pointer to an "object" object, use
    /// the SetMemoryObjectObject() function to fill it
    /// one can also uses the m_CodedValObj and the
    /// SetMemoryCodedValueObject() corresponding function
    ProjectDefMemoryObjects *m_ObjectObj;
    ProjectDefMemoryFieldsCodedVal *m_CodedValObj;

    void _SetValidator();

DECLARE_DYNAMIC_CLASS(ProjectDefLayersEditObjectDlg);
DECLARE_EVENT_TABLE();

public:
    /// Constructors
    ProjectDefLayersEditObjectDlg();

    ProjectDefLayersEditObjectDlg(wxWindow *parent, wxWindowID id = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_IDNAME,
                                  const wxString &caption = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_TITLE,
                                  const wxPoint &pos = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_POSITION,
                                  const wxSize &size = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_SIZE,
                                  long style = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_STYLE);

    /// Creation
    bool Create(wxWindow *parent, wxWindowID id = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_IDNAME,
                const wxString &caption = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_TITLE,
                const wxPoint &pos = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_POSITION,
                const wxSize &size = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_SIZE,
                long style = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_STYLE);

    /// Destructor
    ~ProjectDefLayersEditObjectDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateDlgControls();

    /// Data transfert process, function called automatically
    virtual bool TransferDataToWindow();

    virtual bool TransferDataFromWindow();

    void SetMemoryObjectObject(ProjectDefMemoryObjects *myObjectObj)
    { m_ObjectObj = myObjectObj; }

    void SetMemoryCodedValObject(ProjectDefMemoryFieldsCodedVal *myCodedValObj)
    { m_CodedValObj = myCodedValObj; }

    wxTextCtrl *m_DlgEO_Code;
    wxTextCtrl *m_DlgEO_Value;
    wxButton *m_DlgEO_OK_Btn;
};


class ProjectDefLayersDlg : public wxDialog
{
private:
    wxString m_DlgPDL_Contour_Prefix;

    ProjectDefFieldDlg *m_FieldDialog;
    ProjectDefMemoryLayers *m_LayersObj;

    PrjDefMemManage *m_pPrjDefinition;


    void OnAddField(wxCommandEvent &event);

    void OnRemoveField(wxCommandEvent &event);

    void OnAddObject(wxCommandEvent &event);

    void OnRemoveObject(wxCommandEvent &event);

    void OnImportObject(wxCommandEvent &event);

    void OnChangeOrientation(wxCommandEvent &event);

    void OnSelectLayerType(wxCommandEvent &event);

    void OnLayerNameChange(wxCommandEvent &event);

    void UpdateDefaultPolygonBorderName(const wxString &name);


    bool m_bIsModeEditing;
    bool m_bIsModeAddingEditing;

    void DisableControlsForEdition();

    void ActivateOrientation();


DECLARE_DYNAMIC_CLASS(ProjectDefLayersDlg);
DECLARE_EVENT_TABLE();

public:
    /// Constructors
    ProjectDefLayersDlg();

    ProjectDefLayersDlg(wxWindow *parent,
                        PrjDefMemManage *pPrjDef,
                        bool isEditingMode = FALSE,
                        bool isAddingMode = FALSE,
                        wxWindowID id = SYMBOL_PROJECTDEFLAYERSDLG_IDNAME,
                        const wxString &caption = SYMBOL_PROJECTDEFLAYERSDLG_TITLE,
                        const wxPoint &pos = SYMBOL_PROJECTDEFLAYERSDLG_POSITION,
                        const wxSize &size = SYMBOL_PROJECTDEFLAYERSDLG_SIZE,
                        long style = SYMBOL_PROJECTDEFLAYERSDLG_STYLE);

    /// Creation
    bool Create(wxWindow *parent, wxWindowID id = SYMBOL_PROJECTDEFLAYERSDLG_IDNAME,
                const wxString &caption = SYMBOL_PROJECTDEFLAYERSDLG_TITLE,
                const wxPoint &pos = SYMBOL_PROJECTDEFLAYERSDLG_POSITION,
                const wxSize &size = SYMBOL_PROJECTDEFLAYERSDLG_SIZE,
                long style = SYMBOL_PROJECTDEFLAYERSDLG_STYLE);

    /// Destructor
    ~ProjectDefLayersDlg();

    // contain the list of fields
    PrjMemFieldArray m_FieldArray;

    /// Initialises member variables
    void Init();

    bool IsEditMode()
    { return m_bIsModeEditing; }

    /// Creates the controls and sizers
    void CreateControls();

    //void RemoveObjFromArray();

    void SetMemoryLayersObject(ProjectDefMemoryLayers *myLayersObj)
    { m_LayersObj = myLayersObj; }

    /// Transfer data, is called automaticaly
    /// when the dialog is disabled
    virtual bool TransferDataFromWindow();

    virtual bool TransferDataToWindow();

    /// controls variables
    wxChoice *m_DlgPDL_Layer_Type;
    wxTextCtrl *m_DlgPDL_Layer_Name;
    wxPanel *m_DlgPDL_Panel_Obj;
    ProjectDefLayersObjectList *m_DlgPDL_Object_List;
    wxPanel *m_DlgPDL_Panel_Fields;
    ProjectDefLayersFieldsList *m_DlgPDL_Fields_List;
    wxCheckBox *m_DlgPDL_Orientation_FLD;

    // control for line constructing polygons
    wxStaticBoxSizer *m_DlgPDL_Contour_Static;
    wxTextCtrl *m_DlgPDL_Contour_Name;


};


#endif
