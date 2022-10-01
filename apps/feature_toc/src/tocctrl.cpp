//
// Created by Lucien Schreiber on 27.09.22.
//

#include "tocctrl.h"

#include "bitmaps.h"

TocCtrl::TocCtrl(wxWindow *parent, wxWindowID id)
    : wxDataViewCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxDV_NO_HEADER | wxDV_SINGLE) {
  // Setting model
  wxObjectDataPtr<wxDataViewModel> my_model(new TocCtrlModel);
  wxDataViewCtrl::AssociateModel(my_model.get());

  // Column definition
  wxDataViewCheckIconTextRenderer *renderer = new wxDataViewCheckIconTextRenderer();
  wxDataViewColumn *col1 = new wxDataViewColumn("test", renderer, 0, wxDVC_DEFAULT_WIDTH, wxALIGN_LEFT);
  wxDataViewCtrl::AppendColumn(col1);

  // events
  this->Bind(wxEVT_DATAVIEW_ITEM_BEGIN_DRAG, &TocCtrl::on_dragndrop_begin, this, id);
}

void TocCtrl::add_test_data() {
  //    wxDataViewItem group = AppendContainer(wxDataViewCtrl::GetTopItem(), "Group");
  //    wxDataViewItem item = AppendItem(group, "Salut");
  //    SetItemIcon(item,
  //                wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_image, wxSize(16, 16)).GetBitmap(wxSize(16, 16)));

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
  wxDataViewItem item( event.GetItem() );

  // only allow drags for item, not containers
  if (GetModel()->IsContainer( item ) )
  {
    wxLogMessage("Forbidding starting dragging");
    event.Veto();
    return;
  }

  TocCtrlModelNode *node = (TocCtrlModelNode *) item.GetID();
  wxTextDataObject *obj = new wxTextDataObject;
  obj->SetText( node->m_title );
  event.SetDataObject( obj );
  event.SetDragFlags(wxDrag_AllowMove); // allows both copy and move

  wxLogMessage("Starting dragging \"%s\"", node->m_title);
}
