#ifndef FEATURE_TOC_TOCCTRL_H
#define FEATURE_TOC_TOCCTRL_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP

#include "wx/wx.h"

#endif

#include <wx/dataview.h>

#include "tocctrlmodel.h"

class TocCtrl : public wxDataViewCtrl {
public:
    TocCtrl(wxWindow *parent, wxWindowID id);

    void add_test_data();

//    wxTreeListItem add_group(wxTreeListItem parent, const wxString & label);
//    wxTreeListItem add_layer(wxTreeListItem parent, const wxString &label, int type);
private:
    void on_dragndrop_begin(wxDataViewEvent & event);
    void on_dragndrop_possible(wxDataViewEvent & event);
    void on_dragndrop_drop(wxDataViewEvent & event);
    void on_value_changed(wxDataViewEvent & event);

};

#endif //FEATURE_TOC_TOCCTRL_H