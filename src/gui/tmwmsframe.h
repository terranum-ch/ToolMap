//
// Created by lsc on 26.05.25.
//

#ifndef TMWMSFRAME_H
#define TMWMSFRAME_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/listctrl.h>
#include <wx/srchctrl.h>
#include <wx/filename.h>

///////////////////////////////////////////////////////////////////////////////
/// Class tmWMSBrowserFrame
///////////////////////////////////////////////////////////////////////////////
class tmWMSBrowserFrame : public wxDialog {
  private:
  protected:
    wxStaticText* m_staticText1 = nullptr;
    wxComboBox* m_ctrl_wms_url = nullptr;
    wxStaticText* m_staticText3 = nullptr;
    wxComboBox* m_ctrl_wms_lang = nullptr;
    wxButton* m_ctrl_btn_wms_load_layers = nullptr;
    wxListCtrl* m_ctrl_layer_list = nullptr;
    wxButton* m_btn_export = nullptr;
    wxStaticText* m_staticText2 = nullptr;
    wxSearchCtrl* m_ctrl_search = nullptr;

    // Virtual event handlers, override them in your derived class
    virtual void OnBtnLoadLayers(wxCommandEvent& event);
    virtual void OnBtnExport(wxCommandEvent& event);
    void _create_controls();

    // list functions
    void add_layer_to_list(const wxString& layer_name, const wxString& layer_title, const wxString& layer_abstract,
                           int layer_index);

  public:
    tmWMSBrowserFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("WMS Browser"),
                      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                      long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
};

#endif  // TMWMSFRAME_H
