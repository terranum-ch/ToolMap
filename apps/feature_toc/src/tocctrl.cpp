//
// Created by Lucien Schreiber on 27.09.22.
//

#include "tocctrl.h"
#include "bitmaps.h"

TocCtrl::TocCtrl(wxWindow *parent, wxWindowID id) : wxDataViewTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize,
                                                                 wxDV_NO_HEADER | wxDV_SINGLE) {
    // Column definition
    wxDataViewColumn col1("test", new wxDataViewCheckIconTextRenderer(), 0);

    // Image list
    wxImageList *mylist = new wxImageList(16, 16, true, 4);
    mylist->Add(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_folder, wxSize(16, 16)).GetBitmap(wxSize(16, 16)));
    mylist->Add(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_shapefile, wxSize(16, 16)).GetBitmap(wxSize(16, 16)));
    mylist->Add(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_database, wxSize(16, 16)).GetBitmap(wxSize(16, 16)));
    mylist->Add(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_image, wxSize(16, 16)).GetBitmap(wxSize(16, 16)));
    AssignImageList(mylist);

    // events
    Bind(wxEVT_DATAVIEW_ITEM_BEGIN_DRAG, &TocCtrl::on_dragndrop_begin, this);
}

void TocCtrl::add_test_data() {
    wxDataViewItem group = AppendContainer(wxDataViewCtrl::GetTopItem(), "Group");
    wxDataViewItem item = AppendItem(group, "Salut");
    SetItemIcon(item, wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_image, wxSize(16, 16)).GetBitmap(wxSize(16, 16)));

//    wxTreeListItem group = add_group(GetRootItem(), "Group");
//    wxTreeListItem item2 = add_layer(group, "Shapefile.shp", 1);
//    wxTreeListItem group2 = add_group(group, "Project Layers");
//    wxTreeListItem item3 = add_layer(group2, "Line", 2);
//    wxTreeListItem item4 = add_layer(group2, "Points", 2);
//    wxTreeListItem item5 = add_layer(group, "Raster", 3);
}

/*
wxTreeListItem TocCtrl::add_group(wxTreeListItem parent, const wxString &label) {
    wxTreeListItem item = AppendItem(parent, label);
    SetItemImage(item, 0);
    return item;
}

/// Add a layer to the Toc Control
/// \param parent Tree item id of the Parent
/// \param label String to display
/// \param type 1 = shapefile, 2 = database, 3 = image
/// \return id of the newly added layer
wxTreeListItem TocCtrl::add_layer(wxTreeListItem parent, const wxString &label, int type) {
    wxTreeListItem item = AppendItem(parent, label);
    SetItemImage(item, type);
    return item;
}*/

void TocCtrl::on_dragndrop_begin(wxDataViewEvent &event) {
    event.Allow();
//    wxLogError("testing dragndrop");
//    wxDataViewItem dragItem = event.GetItem();
//    if (!dragItem) {
//        event.Veto();
//        return;
//    }
//
//    wxTextDataObject *obj = new wxTextDataObject();
//    event.SetDataObject(obj);
//    event.SetDragFlags(wxDrag_AllowMove);
}
