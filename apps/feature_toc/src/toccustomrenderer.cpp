#include "toccustomrenderer.h"

#include "bitmaps.h"

tocRendererData::tocRendererData() {
  m_layer_name = wxEmptyString;
  m_image_index = 0;
  m_is_editing = false;
  m_is_visible = true;
}

tocRendererData *tocRendererData::Clone() const {
  tocRendererData *new_data = new tocRendererData();
  new_data->m_is_editing = m_is_editing;
  new_data->m_is_visible = m_is_visible;
  new_data->m_image_index = m_image_index;
  new_data->m_layer_name = m_layer_name;
  return new_data;
}

bool tocRendererData::Eq(wxVariantData &data) const {
  tocRendererData *my_data = (tocRendererData *)&data;
  if (m_layer_name != my_data->m_layer_name) {
    return false;
  }
  if (m_image_index != my_data->m_image_index) {
    return false;
  }
  if (m_is_visible != my_data->m_is_visible) {
    return false;
  }
  if (m_is_editing != my_data->m_is_editing) {
    return false;
  }
  return true;
}

wxString tocRendererData::GetType() const {
  return "tocrendererdata";
}

tocRendererData::~tocRendererData() {}

// This renderer can be either activatable or editable, for demonstration
// purposes. In real programs, you should select whether the user should be
// able to activate or edit the cell and it doesn't make sense to switch
// between the two -- but this is just an example, so it doesn't stop us.
tocRenderer::tocRenderer(wxDataViewCellMode mode) : wxDataViewCustomRenderer("tocrendererdata", mode, wxALIGN_LEFT) {

}

bool tocRenderer::Render(wxRect rect, wxDC *dc, int state) {
  // rectangle and margin
  int margin = 5;
  wxRect rect_checkbox (rect.GetX(), rect.GetY(), 16, 16);
  wxRect rect_layer_type (rect_checkbox.GetX() + rect_checkbox.GetWidth() + margin, rect.GetY(), 16, 16);
  wxRect rect_text (rect_layer_type.GetX() + rect_layer_type.GetWidth() + margin, rect.GetY(),dc->GetTextExtent(m_layer_name).GetWidth(), rect.GetHeight()) ;
  wxRect rect_edit (rect_text.GetX() + rect_text.GetWidth() + margin, rect.GetY(), 16, 16);

  // checkbox image
  wxBitmap bmp_checkbox;
  if (m_is_visible) {
    bmp_checkbox = wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_check_on, wxSize(16, 16)).GetBitmap(wxSize(16, 16));
  } else {
    bmp_checkbox = wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_check_off, wxSize(16, 16)).GetBitmap(wxSize(16, 16));
  }

  // layer type image
  wxBitmap bmp_layer_type;
  switch (m_image_index) {
    case 0: // folder
      bmp_layer_type = wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_folder, wxSize(16, 16)).GetBitmap(wxSize(16, 16));
      break;
    case 1: // shapefile
      bmp_layer_type = wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_shapefile, wxSize(16, 16)).GetBitmap(wxSize(16, 16));
      break;
    case 2: // shapefile
      bmp_layer_type = wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_database, wxSize(16, 16)).GetBitmap(wxSize(16, 16));
      break;
    case 3: // shapefile
      bmp_layer_type = wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_image, wxSize(16, 16)).GetBitmap(wxSize(16, 16));
      break;
    default:
      wxLogError(_("Unsupported bitmap index!"));
  }

  // drawing
  dc->DrawBitmap(bmp_checkbox, rect_checkbox.GetX(), rect_checkbox.GetY());
  dc->DrawBitmap(bmp_layer_type, rect_layer_type.GetX(), rect_layer_type.GetY());
  RenderText(m_layer_name, 0, rect_text, dc, state);

  if (m_is_editing){
    dc->DrawBitmap(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_pen, wxSize(16, 16)).GetBitmap(wxSize(16, 16)),
                   rect_edit.GetX(), rect_edit.GetY());
  }
  return true;
}

bool tocRenderer::ActivateCell(const wxRect &WXUNUSED(cell), wxDataViewModel *WXUNUSED(model),
                               const wxDataViewItem &WXUNUSED(item), unsigned int WXUNUSED(col),
                               const wxMouseEvent *mouseEvent) {
  wxString position;
  if (mouseEvent)
    position = wxString::Format("via mouse at %d, %d", mouseEvent->m_x, mouseEvent->m_y);
  else
    position = "from keyboard";
  wxLogMessage("tocRenderer ActivateCell() %s", position);
  return false;
}

wxSize tocRenderer::GetSize() const {
  return GetView()->FromDIP(wxSize(60, 20));
}

bool tocRenderer::SetValue(const wxVariant &value) {
  tocRendererData *data = (tocRendererData *)value.GetData();
  m_layer_name = data->m_layer_name;
  m_image_index = data->m_image_index;
  m_is_editing = data->m_is_editing;
  m_is_visible = data->m_is_visible;
  return true;
}

bool tocRenderer::GetValue(wxVariant &value) const {
  tocRendererData *data = new tocRendererData();
  data->m_layer_name = m_layer_name;
  data->m_image_index = m_image_index;
  data->m_is_visible = m_is_visible;
  data->m_is_editing = m_is_editing;
  value.SetData(data);
  return true;
}

#if wxUSE_ACCESSIBILITY
virtual wxString GetAccessibleDescription() const wxOVERRIDE {
  return m_layer_name;
}
#endif  // wxUSE_ACCESSIBILITY

bool tocRenderer::HasEditorCtrl() const {
  return true;
}

wxWindow *tocRenderer::CreateEditorCtrl(wxWindow *parent, wxRect labelRect, const wxVariant &value) {
  wxTextCtrl *text =
      new wxTextCtrl(parent, wxID_ANY, value, labelRect.GetPosition(), labelRect.GetSize(), wxTE_PROCESS_ENTER);
  text->SetInsertionPointEnd();

  return text;
}

bool tocRenderer::GetValueFromEditorCtrl(wxWindow *ctrl, wxVariant &value) {
  wxTextCtrl *text = wxDynamicCast(ctrl, wxTextCtrl);
  if (!text) return false;

  value = text->GetValue();

  return true;
}
