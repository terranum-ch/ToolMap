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

private:

};

#endif //FEATURE_TOC_TOCCTRL_H
