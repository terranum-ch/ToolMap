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

};

#endif //FEATURE_TOC_TOCCTRL_H
