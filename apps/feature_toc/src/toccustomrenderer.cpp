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
//  if (m_layer_name != data.m_layer_name) {
//    return false;
//  }
//  if (m_image_index != data.m_image_index) {
//    return false;
//  }
//  if (m_is_visible != data.m_is_visible) {
//    return false;
//  }
//  if (m_is_editing != data.m_is_editing) {
//    return false;
//  }
  return true;
}
wxString tocRendererData::GetType() const{
  return "tocrenderdata";
}

// This renderer can be either activatable or editable, for demonstration
// purposes. In real programs, you should select whether the user should be
// able to activate or edit the cell and it doesn't make sense to switch
// between the two -- but this is just an example, so it doesn't stop us.
tocRenderer::tocRenderer(wxDataViewCellMode mode) : wxDataViewCustomRenderer("string", mode, wxALIGN_LEFT) {
  tocRendererData * my_data = new tocRendererData();
  wxVariant myVariant (my_data);

  //tocRendererData my_data;
  //wxAny my_any = my_data;
  //my_any.GetAs(&myVariant);

  // my_any.operator=(myVariant);
}

bool tocRenderer::Render(wxRect rect, wxDC *dc, int state) {
  int margin = 5;
  dc->DrawBitmap(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_check_on, wxSize(16, 16)).GetBitmap(wxSize(16, 16)),
                 rect.GetX(), rect.GetY());
  dc->DrawBitmap(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_folder, wxSize(16, 16)).GetBitmap(wxSize(16, 16)),
                 rect.GetX() + margin + 16, rect.GetY());

  wxRect my_text_rect = rect;
  my_text_rect.SetX(rect.GetX() + (2 * margin) + 32);
  my_text_rect.SetWidth(dc->GetTextExtent(m_layer_name).GetWidth());

  dc->DrawBitmap(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_pen, wxSize(16, 16)).GetBitmap(wxSize(16, 16)),
                 my_text_rect.GetX() + my_text_rect.GetWidth() + margin, rect.GetY());

  RenderText(m_layer_name,
             0,  // no offset
             my_text_rect, dc, state);
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
  m_layer_name = value.GetString();
  return true;
}

bool tocRenderer::GetValue(wxVariant &WXUNUSED(value)) const {
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
