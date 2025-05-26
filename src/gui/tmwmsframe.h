//
// Created by lsc on 26.05.25.
//

#ifndef TMWMSFRAME_H
#define TMWMSFRAME_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/listctrl.h>
#include <wx/srchctrl.h>

///////////////////////////////////////////////////////////////////////////////
/// Class tmWMSBrowserFrame
///////////////////////////////////////////////////////////////////////////////
class tmWMSBrowserFrame : public wxDialog {
  private:
  protected:
    wxStaticText* m_staticText1;
    wxComboBox* m_ctrl_wms_url;
    wxButton* m_ctrl_btn_wms_load_layers;
    wxListCtrl* m_ctrl_layer_list;
    wxButton* m_btn_export;
    wxStaticText* m_staticText2;
    wxSearchCtrl* m_ctrl_search;

    // Virtual event handlers, override them in your derived class
    virtual void OnBtnExport(wxCommandEvent& event);
    void _create_controls();

  public:
    tmWMSBrowserFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("WMS Browser"),
                      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                      long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
};

#endif  // TMWMSFRAME_H
