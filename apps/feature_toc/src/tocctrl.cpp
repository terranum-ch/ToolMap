#include "tocctrl.h"

#include "tocbitmaps.h"
#include "tocrenderer.h"

TocCtrl::TocCtrl(wxWindow *parent, wxWindowID id)
    : wxDataViewCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxDV_NO_HEADER) {
  m_drag_node_start = nullptr;
  m_drag_node_end = nullptr;

  // Setting model
  wxObjectDataPtr<wxDataViewModel> my_model(new TocCtrlModel);
  wxDataViewCtrl::AssociateModel(my_model.get());
  EnableDragSource(wxDF_UNICODETEXT);
  EnableDropTarget(wxDF_UNICODETEXT);

  // Column definition
  auto *cr = new tocRenderer(wxDATAVIEW_CELL_ACTIVATABLE, (wxDataViewTreeCtrl *)this);
  auto *column5 = new wxDataViewColumn("custom", cr, 0, wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
  wxDataViewCtrl::AppendColumn(column5);

  // events
  this->Bind(wxEVT_DATAVIEW_ITEM_BEGIN_DRAG, &TocCtrl::OnDragndropBegin, this);
  this->Bind(wxEVT_DATAVIEW_ITEM_DROP_POSSIBLE, &TocCtrl::OnDragndropPossible, this);
  this->Bind(wxEVT_DATAVIEW_ITEM_DROP, &TocCtrl::OnDragndropDrop, this);
  this->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &TocCtrl::OnMouseClick, this);
}

void TocCtrl::add_test_data() {
  TocCtrlModel *model = GetTocModel();
  auto *my_root = (TocCtrlModelNode *)model->GetRoot().GetID();
  auto *group_const = model->NodeAdd(my_root, "Construction");
  auto *line = model->NodeAdd(group_const, "Line", true, 2, true);
  auto *point = model->NodeAdd(group_const, "Point", false, 2, false);

  auto *group_support = model->NodeAdd(my_root, "Support");
  auto *shape = model->NodeAdd(group_support, "Shapefile", false, 1, false);
  auto *raster = model->NodeAdd(group_support, "Raster", true, 3, false);
  auto *web = model->NodeAdd(group_support, "Web raster", true, 4, false);
  ExpandAll();
}

void TocCtrl::OnDragndropBegin(wxDataViewEvent &event) {
  wxDataViewItem item(event.GetItem());
  m_drag_node_start = nullptr;
  m_drag_node_end = nullptr;

  // only allow drags for item, not containers

  if (GetTocModel()->GetRoot() == item) {
    wxLogMessage("Forbidding starting dragging Root");
    event.Veto();
    return;
  }
  auto *node = (TocCtrlModelNode *)item.GetID();
  m_drag_node_start = node;

  auto *obj = new wxTextDataObject;
  obj->SetText(node->m_title);

  event.SetDataObject(obj);
  event.SetDragFlags(wxDrag_AllowMove);  // allows both copy and move

  wxLogMessage("Starting dragging \"%s\"", node->m_title);
}

void TocCtrl::OnDragndropPossible(wxDataViewEvent &event) {
  if (event.GetDataFormat() != wxDF_UNICODETEXT)
    event.Veto();
  else
    event.SetDropEffect(wxDragMove);  // check 'move' drop effect
}

void TocCtrl::OnDragndropDrop(wxDataViewEvent &event) {
  wxDataViewItem target_item(event.GetItem());
  m_drag_node_end = nullptr;

  if (event.GetDataFormat() != wxDF_UNICODETEXT) {
    event.Veto();
    return;
  }

  wxTextDataObject obj;
  obj.SetData(wxDF_UNICODETEXT, event.GetDataSize(), event.GetDataBuffer());
  auto *my_model = GetTocModel();
  m_drag_node_end = TocCtrlModel::ConvertFromDataViewItem(my_model->GetRoot());

  int my_proposed_drop_index = event.GetProposedDropIndex();
  if (target_item.IsOk()) {
    auto *target_node = TocCtrlModel::ConvertFromDataViewItem(target_item);
    m_drag_node_end = target_node;

    // veto if trying to drop on myself
    if (m_drag_node_end == m_drag_node_start) {
      wxLogError("Unable to drop on myself!");
      event.Veto();
      return;
    }

    // veto if trying to drop to a child of the container
    if (m_drag_node_start->IsMyChildren(m_drag_node_end)) {
      wxLogError("Unable to drop on a child!");
      event.Veto();
      return;
    }

    // item dropped between elements
    if (my_model->IsContainer(target_item)) {
      if (my_proposed_drop_index == wxNOT_FOUND) {
        my_proposed_drop_index = 0;
      }
      wxLogMessage("Text '%s' dropped in container '%s' (proposed index = %d)", obj.GetText(),
                   my_model->NodeGetTitle(target_node), my_proposed_drop_index);
    } else {  // item dropped on another item
      TocCtrlModelNodePtrArray my_parent_array = target_node->GetParent()->GetChildren();
      my_proposed_drop_index = 0;
      for (int i = 0; i < my_parent_array.GetCount(); i++) {
        if (my_parent_array[i] == target_node) {
          my_proposed_drop_index = i;
        }
      }
      wxLogMessage("Text '%s' dropped on target_item '%s', index %d", obj.GetText(),
                   my_model->NodeGetTitle(target_node), my_proposed_drop_index);
    }
  } else {  // item dropped on background
    my_proposed_drop_index = wxNOT_FOUND;
    wxLogMessage("Text '%s' dropped on background (proposed index = %d)", obj.GetText(), my_proposed_drop_index);
  }
  my_model->NodeMove(m_drag_node_start, m_drag_node_end, my_proposed_drop_index);
}

void TocCtrl::OnMouseClick(wxDataViewEvent &event) {
  wxLogMessage("Value changed !");
}

void TocCtrl::ExpandAll() {
  wxDataViewItem my_root = dynamic_cast<TocCtrlModel *>(GetModel())->GetRoot();
  wxDataViewItemArray my_root_childs;
  GetModel()->GetChildren(my_root, my_root_childs);
  for (int i = 0; i < my_root_childs.GetCount(); i++) {
    Expand(my_root_childs[i]);
  }
}

void TocCtrl::SetColour(const wxColour &colour_normal, const wxColour &colour_dark) {
  auto renderer = dynamic_cast<tocRenderer *>(wxDataViewCtrl::GetColumn(0)->GetRenderer());
  renderer->SetColour(colour_normal, colour_dark);
  wxWindow::Refresh();
}

wxColour TocCtrl::GetColourNormal() {
  auto renderer = dynamic_cast<tocRenderer *>(wxDataViewCtrl::GetColumn(0)->GetRenderer());
  return renderer->GetColourNormal();
}

wxColour TocCtrl::GetColourDark() {
  auto renderer = dynamic_cast<tocRenderer *>(wxDataViewCtrl::GetColumn(0)->GetRenderer());
  return renderer->GetColourDark();
}

TocCtrlModel *TocCtrl::GetTocModel() {
  return dynamic_cast<TocCtrlModel *>(GetModel());
}
