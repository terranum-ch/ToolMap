#include "tocctrl.h"

#include "bitmaps.h"
#include "toccustomrenderer.h"

TocCtrl::TocCtrl(wxWindow *parent, wxWindowID id)
    : wxDataViewCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxDV_NO_HEADER) {
  // Setting model
  wxObjectDataPtr<wxDataViewModel> my_model(new TocCtrlModel);
  wxDataViewCtrl::AssociateModel(my_model.get());
  EnableDragSource(wxDF_UNICODETEXT);
  EnableDropTarget(wxDF_UNICODETEXT);

  // Column definition
  auto *cr = new tocRenderer(wxDATAVIEW_CELL_ACTIVATABLE);
  auto *column5 = new wxDataViewColumn("custom", cr, 0, wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
  wxDataViewCtrl::AppendColumn(column5);

  // events
  this->Bind(wxEVT_DATAVIEW_ITEM_BEGIN_DRAG, &TocCtrl::on_dragndrop_begin, this);
  this->Bind(wxEVT_DATAVIEW_ITEM_DROP_POSSIBLE, &TocCtrl::on_dragndrop_possible, this);
  this->Bind(wxEVT_DATAVIEW_ITEM_DROP, &TocCtrl::on_dragndrop_drop, this);
  this->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &TocCtrl::on_value_changed, this);
}

void TocCtrl::add_test_data() {
  ExpandAll();
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
  wxDataViewItem item(event.GetItem());

  // only allow drags for item, not containers
  if (GetModel()->IsContainer(item)) {
    wxLogMessage("Forbidding starting dragging");
    event.Veto();
    return;
  }

  auto *node = (TocCtrlModelNode *)item.GetID();
  auto *obj = new wxTextDataObject;
  obj->SetText(node->m_title);
  event.SetDataObject(obj);
  event.SetDragFlags(wxDrag_AllowMove);  // allows both copy and move

  wxLogMessage("Starting dragging \"%s\"", node->m_title);
}

void TocCtrl::on_dragndrop_possible(wxDataViewEvent &event) {
  if (event.GetDataFormat() != wxDF_UNICODETEXT)
    event.Veto();
  else
    event.SetDropEffect(wxDragMove);  // check 'move' drop effect
}

void TocCtrl::on_dragndrop_drop(wxDataViewEvent &event) {
  wxDataViewItem item(event.GetItem());

  if (event.GetDataFormat() != wxDF_UNICODETEXT) {
    event.Veto();
    return;
  }

  // Note that instead of recreating a new data object here we could also
  // retrieve the data object from the event, using its GetDataObject()
  // method. This would be more efficient as it would avoid copying the text
  // one more time, but would require a cast in the code and we don't really
  // care about efficiency here.
  wxTextDataObject obj;
  obj.SetData(wxDF_UNICODETEXT, event.GetDataSize(), event.GetDataBuffer());

  auto *my_model = dynamic_cast<TocCtrlModel *>(GetModel());
  if (item.IsOk()) {
    if (my_model->IsContainer(item)) {
      wxLogMessage("Container");
      // wxLogMessage("Text '%s' dropped in container '%s' (proposed index = %i)",
      //              obj.GetText(), my_model->GetTitle(item), event.GetProposedDropIndex());
    } else
      wxLogMessage("Item");
    // wxLogMessage("Text '%s' dropped on item '%s'", obj.GetText(), my_model->GetTitle(item));
  } else
    wxLogMessage("Background");
  // wxLogMessage("Text '%s' dropped on background (proposed index = %i)", obj.GetText(), event.GetProposedDropIndex());
}

void TocCtrl::on_value_changed(wxDataViewEvent &event) {
  wxLogMessage("Value changed !");
}

void TocCtrl::ExpandAll() {
  wxDataViewItem my_root = dynamic_cast<TocCtrlModel*>(GetModel())->GetRoot();
  wxDataViewItemArray my_root_childs;
  GetModel()->GetChildren(my_root, my_root_childs);
  for (int i = 0; i < my_root_childs.GetCount(); i++){
    Expand(my_root_childs[i]);
  }
}
