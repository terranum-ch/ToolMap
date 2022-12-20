/***************************************************************************
 tmaattribctrls.h
 Controls for the tmAAttribTree
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
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

#ifndef _TM_AATTRIBCTRLS_H_
#define _TM_AATTRIBCTRLS_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/spinctrl.h>  // for spin ctrl

#include "../core/projectdefmemory.h"  // for fields definitions
// #include <wx/generic/spinctrg.h> // for float spin ctrl
#include <wx/datectrl.h>  // for date picker ctrl

const wxString LABELDEFAULT = _T("MyLabel");

/***************************************************************************/ /**
  @brief Abstract class for tmAAttribTree Items
  @details This may be used as the abstract base class for creating items to use
  into the tmAAttribTree. This way, all items share the same methods for
  accessing and setting data without regarding the item type (text, choice,
  etc.) This abstract class is used by :
  - tmAAttribCtrlText
  - tmAAttribCtrlChoice
  - tmAAttribCtrlInteger
  @author Lucien Schreiber (c) CREALP 2009
  @date 08 March 2009
  *******************************************************************************/
class tmAAttribCtrl : public wxPanel {
  private:
  protected:
    wxStaticText* m_Label;

  public:
    tmAAttribCtrl() {
        m_Label = nullptr;
    }

    tmAAttribCtrl(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                  long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrl")) {
        ;
    }

    virtual void Create(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                        long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrl")) = 0;

    ~tmAAttribCtrl() {
        ;
    }

    // label operations
    virtual void SetLabel(const wxString& labeltext);

    virtual wxString GetLabel() const;

    // control operations
    virtual void SetControlValue(const wxString& value) = 0;

    virtual wxString GetControlValue() = 0;

    virtual void SetEmptyValue() {
        ;
    }
};

// Creating a list of Ctrls
class tmAAttribCtrlStruct : public wxObject {
  public:
    tmAAttribCtrlStruct() {
        m_Ctrl = nullptr;
    }

    tmAAttribCtrl* m_Ctrl;
};

WX_DECLARE_OBJARRAY(tmAAttribCtrlStruct, tmAAttribCtrlArray);

class tmFullSelectTextCtrl : public wxTextCtrl {
  private:
    bool m_bDoSelectAll;

    void OnFocus(wxFocusEvent& event);

    void OnIdle(wxIdleEvent& event);

    DECLARE_EVENT_TABLE();

  protected:
  public:
    tmFullSelectTextCtrl();

    tmFullSelectTextCtrl(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& value = wxEmptyString,
                         const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0,
                         const wxValidator& validator = wxDefaultValidator, const wxString& name = wxTextCtrlNameStr);

    void Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0,
                const wxValidator& validator = wxDefaultValidator, const wxString& name = wxTextCtrlNameStr);

    ~tmFullSelectTextCtrl();
};

class tmFullSelectSpinCtrl : public wxSpinCtrl {
  private:
    bool m_bDoSelectAll;

    void OnFocus(wxFocusEvent& event);

    void OnLooseFocus(wxFocusEvent& event);

    void OnIdle(wxIdleEvent& event);

    DECLARE_EVENT_TABLE();

  protected:
  public:
    tmFullSelectSpinCtrl();

    tmFullSelectSpinCtrl(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& value = wxEmptyString,
                         const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                         long style = wxSP_ARROW_KEYS, int min = 0, int max = 100, int initial = 0,
                         const wxString& name = _T("tmFullSelectSpinCtrl"));

    void Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                long style = wxSP_ARROW_KEYS, int min = 0, int max = 100, int initial = 0,
                const wxString& name = _T("tmFullSelectSpinCtrl"));

    ~tmFullSelectSpinCtrl();
};

/***************************************************************************/ /**
  @brief tmAAttribTree item of type TextCtrl
  @details This control define a panel with a static text SetLabel(), GetLabel()
  and a textctrl. Use SetControlValue(), GetControlValue() to access the
  textctrl.
  @author Lucien Schreiber (c) CREALP 2009
  @date 08 March 2009
  *******************************************************************************/
class tmAAttribCtrlText : public tmAAttribCtrl {
  private:
    tmFullSelectTextCtrl* m_Control;
    wxStaticText* m_ControlInfo;

    void SetProperties(const ProjectDefMemoryFields& fieldinfo);

  protected:
  public:
    tmAAttribCtrlText();

    tmAAttribCtrlText(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                      long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrlText"));

    void Create(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrlText"));

    ~tmAAttribCtrlText();

    // control operations
    virtual void SetControlValue(const wxString& value);

    virtual wxString GetControlValue();
};

class tmAAttribCtrlInteger : public tmAAttribCtrl {
  private:
    tmFullSelectSpinCtrl* m_Control;

    void SetProperties(const ProjectDefMemoryFields& fieldinfo);

  protected:
  public:
    tmAAttribCtrlInteger();

    tmAAttribCtrlInteger(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                         const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                         long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrlInteger"));

    void Create(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrlInteger"));

    ~tmAAttribCtrlInteger();

    // control operations
    virtual void SetControlValue(const wxString& value);

    virtual wxString GetControlValue();

    virtual void SetEmptyValue();
};

class tmAAttribCtrlFloat : public tmAAttribCtrl {
  private:
    tmFullSelectTextCtrl* m_Control;
    wxStaticText* m_ControlInfo;

    void SetProperties(const ProjectDefMemoryFields& fieldinfo);

  protected:
  public:
    tmAAttribCtrlFloat();

    tmAAttribCtrlFloat(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                       const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                       long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrlFloat"));

    void Create(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrlFloat"));

    ~tmAAttribCtrlFloat();

    // control operations
    virtual void SetControlValue(const wxString& value);

    virtual wxString GetControlValue();

    virtual void SetEmptyValue();
};

const unsigned int AATTRIB_ENUM_RADIO_NUMBER = 3;

class tmAAttribCtrlEnum : public tmAAttribCtrl {
  private:
    wxChoice* m_ControlChoice;
    wxRadioButton* m_ControlRadios[AATTRIB_ENUM_RADIO_NUMBER];
    int m_iNumRadios;
    ProjectDefMemoryFields m_Field;

    // private init function
    void InitMemberValues();

    bool IsChoiceList();

    // control creating
    wxSizer* CreateChoiceControl(const PrjMemFieldCodedValArray& valarray);

  protected:
  public:
    tmAAttribCtrlEnum();

    tmAAttribCtrlEnum(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                      long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrlEnum"));

    void Create(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrlEnum"));

    ~tmAAttribCtrlEnum();

    // control operations
    virtual void SetControlValue(const wxString& value);

    virtual wxString GetControlValue();

    virtual void SetEmptyValue();
};

class tmAAttribCtrlDate : public tmAAttribCtrl {
  private:
    wxDatePickerCtrl* m_Control;

  protected:
  public:
    tmAAttribCtrlDate();

    tmAAttribCtrlDate(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                      long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrlDate"));

    void Create(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrlDate"));

    ~tmAAttribCtrlDate();

    // control operations
    virtual void SetControlValue(const wxString& value);

    virtual wxString GetControlValue();
};

class tmAAttribCtrlSafeDate : public tmAAttribCtrl {
  private:
    tmFullSelectTextCtrl* m_Control;
    wxStaticText* m_ControlInfo;

    void SetProperties(const ProjectDefMemoryFields& fieldinfo);

  protected:
  public:
    tmAAttribCtrlSafeDate();

    tmAAttribCtrlSafeDate(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                          const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                          long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrlSafeDate"));

    void Create(wxWindow* parent, const ProjectDefMemoryFields& fieldinfo, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL, const wxString& name = _T("tmAAttribCtrlSafeDate"));

    ~tmAAttribCtrlSafeDate();

    // control operations
    virtual void SetControlValue(const wxString& value);

    virtual wxString GetControlValue();

    virtual void SetEmptyValue();
};

#endif
