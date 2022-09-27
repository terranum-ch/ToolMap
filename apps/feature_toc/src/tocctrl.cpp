//
// Created by Lucien Schreiber on 27.09.22.
//

#include "tocctrl.h"
#include "bitmaps.h"

TocCtrl::TocCtrl(wxWindow *parent, wxWindowID id) : wxTreeListCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxTL_CHECKBOX | wxTL_SINGLE) {
    AppendColumn(wxT("Column1"), wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_RESIZABLE);

    wxImageList * mylist = new wxImageList(16,16, true, 2);
    mylist->Add(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_folder, wxSize(16,16)).GetBitmap(wxSize(16,16)));
    mylist->Add(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_file, wxSize(16,16)).GetBitmap(wxSize(16,16)));
    AssignImageList(mylist);
}

void TocCtrl::add_test_data() {
    wxTreeListItem group = AppendItem(GetRootItem(), "Group");
    SetItemImage(group, 0);
    wxTreeListItem item = AppendItem(group, "Shapefile.shp");
    SetItemImage(item, 1);
    wxTreeListItem item2 = AppendItem(group, "Line");
    SetItemImage(item2, 1);

}
