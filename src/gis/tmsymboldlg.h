/***************************************************************************
 tmsymboldlg.h
 Display Symbology dialog
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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


#ifndef _TM_SYMBOLDLG_H_
#define _TM_SYMBOLDLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include "wx/notebook.h"
#include "wx/spinctrl.h"
#include <wx/clrpicker.h>        // colour picker used in all child
#include "../core/tmcolourpickerctrl.h"    // colour picker
#include "../core/tmsliderwithtxt.h"    // slider with textctrl
#include "tmsymbolpendef.h"        // pen and brush defintions
#include <wx/html/htmlwin.h>    // html window for metadata
#include "tmlayerpropertiesdef.h" // for LoadQueries


const int ID_LINESSYMBOLOGY = 10079;
//const int ID_SYMDLG_NOTEBOOK = 10116;
const int ID_SYMDLG_PANEL = 10159;
//const int ID_SYMDLG_INFOTXT = 10215;


#define SYMBOL_TMSYMBOLDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_TMSYMBOLDLG_TITLE _("Symbology")
#define SYMBOL_TMSYMBOLDLG_IDNAME ID_LINESSYMBOLOGY
#define SYMBOL_TMSYMBOLDLG_SIZE wxSize(400, 300)
#define SYMBOL_TMSYMBOLDLG_POSITION wxDefaultPosition

class DataBaseTM;


class tmSymbolDLG : public wxDialog
{
private:
    DataBaseTM *m_pDB;

    void _Init();

    void CreateControlsBasic();

DECLARE_DYNAMIC_CLASS(tmSymbolDLG)

protected:
    wxPanel *m_SymbolPanel;

    //wxHtmlWindow * m_MetaDataWnd;
    void SetSizeHint();

    wxArrayLong m_QueriesId;

    void _LoadQueries(wxChoice *choicectrl, TOC_GENERIC_NAME toctarget);


public:
    /// Constructors
    tmSymbolDLG();

    ~tmSymbolDLG();

    tmSymbolDLG(wxWindow *parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
                const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE,
                const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION,
                const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE,
                long style = SYMBOL_TMSYMBOLDLG_STYLE);

    bool Create(wxWindow *parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
                const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE,
                const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION,
                const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE,
                long style = SYMBOL_TMSYMBOLDLG_STYLE);

    void SetDatabase(DataBaseTM *database)
    { m_pDB = database; }

    //void SetMetaData (const wxString & metadata);
    //void CreateControlsLine();
    /*void CreateControlsPoint();
    void CreateControlsPolygon();
    void CreateControlsRaster();*/


};


#endif
