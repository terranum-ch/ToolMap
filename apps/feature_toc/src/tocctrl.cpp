//
// Created by Lucien Schreiber on 27.09.22.
//

#include "tocctrl.h"


TocCtrl::TocCtrl(wxWindow *parent, wxWindowID id) : wxTreeListCtrl(parent, id) {
    AppendColumn(wxT("Column1"), wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_RESIZABLE);
    AppendColumn(wxT("Column2"), wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_RESIZABLE);
}