/***************************************************************************
								aattribbatch_dlg.h
						Display dialog for batch advanced attribution
                             -------------------
    copyright            : (C) 2010 CREALP Lucien Schreiber 
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

#ifndef _AATTRIB_BATCH_DLG_H_
#define _AATTRIB_BATCH_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/statline.h>
#include "../core/tmaattribbatchmanager.h"
#include "../gis/tmaattribctrls.h"

///////////////////////////////////////////////////////////////////////////

#define ID_DLGAATB_LIST_TYPE 22450
#define ID_DLGAATB_LIST_FIELDS 22451


/***************************************************************************//**
 @brief tmAAttribInfo Panel
 @author Lucien Schreiber (c) CREALP 2010
 @date 29 June 2010
 *******************************************************************************/
class tmAAttribInfoPanel : public tmAAttribCtrl
{
private:
    wxStaticText *m_ControlInfo;

protected:
public:
    tmAAttribInfoPanel();

    tmAAttribInfoPanel(wxWindow *parent,
                       const ProjectDefMemoryFields &fieldinfo,
                       wxWindowID id = wxID_ANY,
                       const wxPoint &pos = wxDefaultPosition,
                       const wxSize &size = wxDefaultSize,
                       long style = wxTAB_TRAVERSAL,
                       const wxString &name = _T("tmAAttribInfoPanel"))
    {
        tmAAttribInfoPanel::Create(parent, fieldinfo, id, pos, size, style, name);
    }

    void Create(wxWindow *parent,
                const ProjectDefMemoryFields &fieldinfo,
                wxWindowID id = wxID_ANY,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL,
                const wxString &name = _T("tmAAttribInfoPanel"))
    {
        wxPanel::Create(parent, id, pos, size, style, name);
        wxBoxSizer *bSizer24;
        bSizer24 = new wxBoxSizer(wxHORIZONTAL);
        m_Label = new wxStaticText(this, wxID_ANY,
                                   _("Hint: "),
                                   wxDefaultPosition,
                                   wxDefaultSize, 0);
        m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
        bSizer24->Add(m_Label, 0, wxRIGHT | wxTOP | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 5);

        wxBoxSizer *bSizer25 = new wxBoxSizer(wxHORIZONTAL);

        m_ControlInfo = new wxStaticText(this, wxID_ANY,
                                         _("Select an object type then a field to be able to set value to selected objects"));
        m_ControlInfo->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
        bSizer25->Add(m_ControlInfo, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
        bSizer24->Add(bSizer25, 1, wxALL | wxALIGN_CENTER_VERTICAL, 2);

        SetSizer(bSizer24);
        Layout();
        bSizer24->Fit(this);
    }

    ~tmAAttribInfoPanel()
    { ; }


    // control operations
    virtual void SetControlValue(const wxString &value)
    { ; }

    virtual wxString GetControlValue()
    { return wxEmptyString; }
};


///////////////////////////////////////////////////////////////////////////////
/// Class AAttribBatch_DLG
///////////////////////////////////////////////////////////////////////////////
class AAttribBatch_DLG : public wxDialog
{
DECLARE_EVENT_TABLE()
private:
    wxListBox *m_ListType;
    wxListBox *m_ListFields;
    //wxPanel* m_PanelValue;
    wxStdDialogButtonSizer *m_Btn;
    wxButton *m_BtnSave;
    wxButton *m_BtnApply;
    wxButton *m_BtnCancel;
    wxBoxSizer *m_CtrlSizer;
    tmAAttribCtrl *m_ValueCtrl;

    wxArrayLong m_ArrayIds;
    PrjMemFieldArray m_Fields;
    tmAAttribBatchManager *m_pBatchManager;


    void _SetControl(tmAAttribCtrl *ctrl);

    void OnTypeChange(wxCommandEvent &event);

    void OnFieldsChange(wxCommandEvent &event);

    void OnApply(wxCommandEvent &event);

    void OnSave(wxCommandEvent &event);


public:

    AAttribBatch_DLG(wxWindow *parent, tmAAttribBatchManager *manager,
                     wxWindowID id = wxID_ANY,
                     const wxString &title = _("Object attribute (multiple features)"),
                     const wxPoint &pos = wxDefaultPosition,
                     const wxSize &size = wxSize(550, 350),
                     long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

    ~AAttribBatch_DLG();

    virtual bool TransferDataToWindow();

};

#endif 
