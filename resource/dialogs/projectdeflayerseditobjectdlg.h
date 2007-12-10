/////////////////////////////////////////////////////////////////////////////
// Name:        projectdeflayerseditobjectdlg.h
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Mon 10 Dec 15:43:25 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _PROJECTDEFLAYERSEDITOBJECTDLG_H_
#define _PROJECTDEFLAYERSEDITOBJECTDLG_H_

/*!
 * Includes
 */

////@begin includes
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
#define ID_PROJECTDEFLAYERSEDITOBJECTDLG 10117
#define ID_DLGEO_CODE 10259
#define ID_DLGEO_VALUE 10260
#define ID_STATICLINE2 10261
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_TITLE _("Edit Object")
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_IDNAME ID_PROJECTDEFLAYERSEDITOBJECTDLG
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * ProjectDefLayersEditObjectDlg class declaration
 */

class ProjectDefLayersEditObjectDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ProjectDefLayersEditObjectDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ProjectDefLayersEditObjectDlg();
    ProjectDefLayersEditObjectDlg( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_IDNAME, const wxString& caption = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_TITLE, const wxPoint& pos = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_POSITION, const wxSize& size = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_SIZE, long style = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_IDNAME, const wxString& caption = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_TITLE, const wxPoint& pos = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_POSITION, const wxSize& size = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_SIZE, long style = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_STYLE );

    /// Destructor
    ~ProjectDefLayersEditObjectDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin ProjectDefLayersEditObjectDlg event handler declarations

////@end ProjectDefLayersEditObjectDlg event handler declarations

////@begin ProjectDefLayersEditObjectDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ProjectDefLayersEditObjectDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ProjectDefLayersEditObjectDlg member variables
    wxTextCtrl* m_DlgEO_Code;
    wxTextCtrl* m_DlgEO_Value;
////@end ProjectDefLayersEditObjectDlg member variables
};

#endif
    // _PROJECTDEFLAYERSEDITOBJECTDLG_H_
