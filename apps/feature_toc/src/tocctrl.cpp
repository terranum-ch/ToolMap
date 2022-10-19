//
// Created by Lucien Schreiber on 27.09.22.
//

#include "tocctrl.h"

#include "bitmaps.h"


// ----------------------------------------------------------------------------
// MyCustomRenderer
// ----------------------------------------------------------------------------

class MyCustomRenderer : public wxDataViewCustomRenderer {
 public:
  // This renderer can be either activatable or editable, for demonstration
  // purposes. In real programs, you should select whether the user should be
  // able to activate or edit the cell and it doesn't make sense to switch
  // between the two -- but this is just an example, so it doesn't stop us.
  explicit MyCustomRenderer(wxDataViewCellMode mode) : wxDataViewCustomRenderer("string", mode, wxALIGN_LEFT) {}

  virtual bool Render(wxRect rect, wxDC *dc, int state) wxOVERRIDE {
    int margin = 5;

    dc->DrawBitmap(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_check_on, wxSize(16,16)).GetBitmap(wxSize(16,16)), rect.GetX(), rect.GetY());
    dc->DrawBitmap(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_folder, wxSize(16,16)).GetBitmap(wxSize(16,16)), rect.GetX()+margin+16, rect.GetY());
    dc->SetBrush(*wxLIGHT_GREY_BRUSH);
    dc->SetPen(*wxTRANSPARENT_PEN);

    wxRect my_text_rect = rect;
    my_text_rect.SetX(rect.GetX() + (2*margin) + 32);
    my_text_rect.SetWidth(dc->GetTextExtent(m_value).GetWidth());

    //rect.Deflate(2);
    //dc->DrawRoundedRectangle(rect, 5);

    RenderText(m_value,
               0,  // no offset
               my_text_rect, dc, state);
    return true;
  }

  virtual bool ActivateCell(const wxRect &WXUNUSED(cell), wxDataViewModel *WXUNUSED(model),
                            const wxDataViewItem &WXUNUSED(item), unsigned int WXUNUSED(col),
                            const wxMouseEvent *mouseEvent) wxOVERRIDE {
    wxString position;
    if (mouseEvent)
      position = wxString::Format("via mouse at %d, %d", mouseEvent->m_x, mouseEvent->m_y);
    else
      position = "from keyboard";
    wxLogMessage("MyCustomRenderer ActivateCell() %s", position);
    return false;
  }

  virtual wxSize GetSize() const wxOVERRIDE {
    return GetView()->FromDIP(wxSize(60, 20));
  }

  virtual bool SetValue(const wxVariant &value) wxOVERRIDE {
    m_value = value.GetString();
    return true;
  }

  virtual bool GetValue(wxVariant &WXUNUSED(value)) const wxOVERRIDE {
    return true;
  }

#if wxUSE_ACCESSIBILITY
  virtual wxString GetAccessibleDescription() const wxOVERRIDE {
    return m_value;
  }
#endif  // wxUSE_ACCESSIBILITY

  virtual bool HasEditorCtrl() const wxOVERRIDE {
    return true;
  }

  virtual wxWindow *CreateEditorCtrl(wxWindow *parent, wxRect labelRect, const wxVariant &value) wxOVERRIDE {
    wxTextCtrl *text =
        new wxTextCtrl(parent, wxID_ANY, value, labelRect.GetPosition(), labelRect.GetSize(), wxTE_PROCESS_ENTER);
    text->SetInsertionPointEnd();

    return text;
  }

  virtual bool GetValueFromEditorCtrl(wxWindow *ctrl, wxVariant &value) wxOVERRIDE {
    wxTextCtrl *text = wxDynamicCast(ctrl, wxTextCtrl);
    if (!text) return false;

    value = text->GetValue();

    return true;
  }

 private:
  wxString m_value;
};



TocCtrl::TocCtrl(wxWindow *parent, wxWindowID id)
    : wxDataViewCtrl(parent, id, wxDefaultPosition, wxDefaultSize,  wxDV_SINGLE | wxDV_NO_HEADER) {
  // Setting model
  wxObjectDataPtr<wxDataViewModel> my_model(new TocCtrlModel);
  wxDataViewCtrl::AssociateModel(my_model.get());
  EnableDragSource(wxDF_UNICODETEXT);
  EnableDropTarget(wxDF_UNICODETEXT);

  // Column definition

  MyCustomRenderer *cr = new MyCustomRenderer(wxDATAVIEW_CELL_ACTIVATABLE);
  wxDataViewColumn *column5 =
      new wxDataViewColumn("custom", cr, 0, wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
  wxDataViewCtrl::AppendColumn(column5);

//  auto *renderer = new wxDataViewIconTextRenderer();
//  auto *col1 = new wxDataViewColumn("Layer", renderer, 0, FromDIP(150), wxALIGN_LEFT);
//  wxDataViewCtrl::AppendColumn(col1);
//
//  auto * check_renderer = new wxDataViewCheckIconTextRenderer();
//  auto * col2 = new wxDataViewColumn("Status", check_renderer, 1, FromDIP(80), wxALIGN_LEFT);
//  wxDataViewCtrl::AppendColumn(col2);

  // events
//  this->Bind(wxEVT_DATAVIEW_ITEM_BEGIN_DRAG, &TocCtrl::on_dragndrop_begin, this);
//  this->Bind(wxEVT_DATAVIEW_ITEM_DROP_POSSIBLE, &TocCtrl::on_dragndrop_possible, this);
//  this->Bind(wxEVT_DATAVIEW_ITEM_DROP, &TocCtrl::on_dragndrop_drop, this);
  this->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &TocCtrl::on_value_changed, this);

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
  wxLogWarning("Test begin");
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

void TocCtrl::on_dragndrop_possible(wxDataViewEvent &event) {
  wxLogWarning("Test drop possible");
  if (event.GetDataFormat() != wxDF_UNICODETEXT)
    event.Veto();
  else
    event.SetDropEffect(wxDragMove); // check 'move' drop effect
}

void TocCtrl::on_dragndrop_drop(wxDataViewEvent &event) {
  wxDataViewItem item( event.GetItem() );

  if (event.GetDataFormat() != wxDF_UNICODETEXT)
  {
    event.Veto();
    return;
  }

  // Note that instead of recreating a new data object here we could also
  // retrieve the data object from the event, using its GetDataObject()
  // method. This would be more efficient as it would avoid copying the text
  // one more time, but would require a cast in the code and we don't really
  // care about efficiency here.
  wxTextDataObject obj;
  obj.SetData( wxDF_UNICODETEXT, event.GetDataSize(), event.GetDataBuffer() );

  if ( item.IsOk() )
  {
    wxLogWarning(_("Dropping..."));
  }

}

void TocCtrl::on_value_changed(wxDataViewEvent &event) {
  wxLogWarning("Value changed !");
}
