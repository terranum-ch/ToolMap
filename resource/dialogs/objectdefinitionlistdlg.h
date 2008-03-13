/////////////////////////////////////////////////////////////////////////////
// Name:        objectdefinitionlistdlg.h
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Thu 13 Mar 11:22:07 2008
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _OBJECTDEFINITIONLISTDLG_H_
#define _OBJECTDEFINITIONLISTDLG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/valtext.h"
#include "wx/statline.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_OBJECTDEFINITIONLISTDLG 10030
#define ID_DLGODD_LIST_LYR_NAME 10265
#define ID_DLGODD_FREQUENCY 10266
#define SYMBOL_OBJECTDEFINITIONLISTDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL|wxTAB_TRAVERSAL
#define SYMBOL_OBJECTDEFINITIONLISTDLG_TITLE _("Edit object definition")
#define SYMBOL_OBJECTDEFINITIONLISTDLG_IDNAME ID_OBJECTDEFINITIONLISTDLG
#define SYMBOL_OBJECTDEFINITIONLISTDLG_SIZE wxSize(400, 300)
#define SYMBOL_OBJECTDEFINITIONLISTDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * ObjectDefinitionListDlg class declaration
 */

class ObjectDefinitionListDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ObjectDefinitionListDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ObjectDefinitionListDlg();
    ObjectDefinitionListDlg( wxWindow* parent, wxWindowID id = SYMBOL_OBJECTDEFINITIONLISTDLG_IDNAME, const wxString& caption = SYMBOL_OBJECTDEFINITIONLISTDLG_TITLE, const wxPoint& pos = SYMBOL_OBJECTDEFINITIONLISTDLG_POSITION, const wxSize& size = SYMBOL_OBJECTDEFINITIONLISTDLG_SIZE, long style = SYMBOL_OBJECTDEFINITIONLISTDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_OBJECTDEFINITIONLISTDLG_IDNAME, const wxString& caption = SYMBOL_OBJECTDEFINITIONLISTDLG_TITLE, const wxPoint& pos = SYMBOL_OBJECTDEFINITIONLISTDLG_POSITION, const wxSize& size = SYMBOL_OBJECTDEFINITIONLISTDLG_SIZE, long style = SYMBOL_OBJECTDEFINITIONLISTDLG_STYLE );

    /// Destructor
    ~ObjectDefinitionListDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin ObjectDefinitionListDlg event handler declarations

////@end ObjectDefinitionListDlg event handler declarations

////@begin ObjectDefinitionListDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ObjectDefinitionListDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ObjectDefinitionListDlg member variables
    wxTextCtrl* m_DLGODD_Code;
    wxTextCtrl* m_DLGODD_Description;
    wxChoice* m_DLGODD_List_Lyr_Name;
    wxRadioBox* m_DLGODD_Frequency;
////@end ObjectDefinitionListDlg member variables
};

#endif
    // _OBJECTDEFINITIONLISTDLG_H_
