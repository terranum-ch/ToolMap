#ifndef FEATURE_TOC_TOCCTRL_H
#define FEATURE_TOC_TOCCTRL_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP

#include "wx/wx.h"

#endif

#include <wx/treelist.h>

class TocCtrl : public wxTreeListCtrl {
public:
    TocCtrl(wxWindow *parent, wxWindowID id);

    void add_test_data();

    wxTreeListItem add_group(wxTreeListItem parent, const wxString & label);
    wxTreeListItem add_layer(wxTreeListItem parent, const wxString &label, int type);
};

#endif //FEATURE_TOC_TOCCTRL_H
