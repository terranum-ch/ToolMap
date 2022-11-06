#include "bitmaps.h"
#include "tocctrlmodel.h"
#include "tocrenderer.h"

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
  // loading images
  // supporting dark / white bitmaps
  wxString my_black_names[] = {feature_toc_bitmaps::toc_folder, feature_toc_bitmaps::toc_shapefile,
                               feature_toc_bitmaps::toc_database, feature_toc_bitmaps::toc_image, feature_toc_bitmaps::toc_web,
                               feature_toc_bitmaps::toc_check_on, feature_toc_bitmaps::toc_check_off, feature_toc_bitmaps::toc_pen };
  wxArrayString my_bitmaps_name(sizeof (my_black_names) / sizeof (wxString), my_black_names);

  wxSystemAppearance sys_app = wxSystemSettings::GetAppearance();
  if (sys_app.IsDark()) {
    wxLogDebug("Dark mode found!");
    wxString my_white_name [] = {feature_toc_bitmaps::w_toc_folder, feature_toc_bitmaps::w_toc_shapefile,
                                feature_toc_bitmaps::w_toc_database, feature_toc_bitmaps::w_toc_image, feature_toc_bitmaps::w_toc_web,
                                feature_toc_bitmaps::w_toc_check_on, feature_toc_bitmaps::w_toc_check_off, feature_toc_bitmaps::w_toc_pen };
    my_bitmaps_name = wxArrayString(sizeof (my_white_name) / sizeof (wxString), my_white_name);
  }

  m_image_list.Create(16,16, true, sizeof(my_black_names) / sizeof (wxString));
  for (int i = 0; i<my_bitmaps_name.GetCount(); i++) {
    m_image_list.Add(wxBitmapBundle::FromSVG(my_bitmaps_name[i], wxSize(16, 16)).GetBitmap(wxSize(16, 16)));
  }
}

bool tocRenderer::Render(wxRect rect, wxDC *dc, int state) {
  // rectangle and margin
  int margin = 5;
  wxRect rect_checkbox(rect.GetX(), rect.GetY(), 16, 16);
  wxRect rect_layer_type(rect_checkbox.GetX() + rect_checkbox.GetWidth() + margin, rect.GetY(), 16, 16);
  wxRect rect_text(rect_layer_type.GetX() + rect_layer_type.GetWidth() + margin, rect.GetY(),
                   dc->GetTextExtent(m_layer_name).GetWidth(), rect.GetHeight());
  wxRect rect_edit(rect_text.GetX() + rect_text.GetWidth() + margin, rect.GetY(), 16, 16);

  // checkbox image
  wxASSERT(m_image_list.GetImageCount() > 0);

  wxBitmap bmp_checkbox;
  if (m_is_visible) {
    bmp_checkbox = m_image_list.GetBitmap(5);
  } else {
    bmp_checkbox = m_image_list.GetBitmap(6);
  }

  // layer type image
  wxBitmap bmp_layer_type;
  switch (m_image_index) {
    case 0:  // folder
      bmp_layer_type = m_image_list.GetBitmap(0);
      break;
    case 1:  // shapefile
      bmp_layer_type =
          wxBitmapBundle::FromSVG(feature_toc_bitmaps::w_toc_shapefile, wxSize(16, 16)).GetBitmap(wxSize(16, 16));
      break;
    case 2:  // database
      bmp_layer_type =
          wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_database, wxSize(16, 16)).GetBitmap(wxSize(16, 16));
      break;
    case 3:  // image
      bmp_layer_type =
          wxBitmapBundle::FromSVG(feature_toc_bitmaps::w_toc_image, wxSize(16, 16)).GetBitmap(wxSize(16, 16));
      break;
    default:
      wxLogError(_("Unsupported bitmap index!"));
  }

  // drawing
  dc->DrawBitmap(bmp_checkbox, rect_checkbox.GetX(), rect_checkbox.GetY());
  dc->DrawBitmap(bmp_layer_type, rect_layer_type.GetX(), rect_layer_type.GetY());
  RenderText(m_layer_name, 0, rect_text, dc, state);

  if (m_is_editing) {
    dc->DrawBitmap(wxBitmapBundle::FromSVG(feature_toc_bitmaps::w_toc_pen, wxSize(16, 16)).GetBitmap(wxSize(16, 16)),
                   rect_edit.GetX(), rect_edit.GetY());
  }
  return true;
}

bool tocRenderer::ActivateCell(const wxRect &cell, wxDataViewModel *model, const wxDataViewItem &item,
                               unsigned int WXUNUSED(col), const wxMouseEvent *mouseEvent) {
  wxRect rect_checkbox(0, 0, 16, 16);
  auto *my_model = (TocCtrlModel *)model;

  bool check = false;
  if (rect_checkbox.Contains(mouseEvent->GetPosition())) {
    if (my_model->IsChecked(item)) {
      wxLogMessage("It's checked");
    } else {
      check = true;
      wxLogMessage("It's not checked!");
    }
    my_model->SetChecked(item, check);
    my_model->ItemChanged(item);
  }

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
  auto *data = (tocRendererData *)value.GetData();
  m_layer_name = data->m_layer_name;
  m_image_index = data->m_image_index;
  m_is_editing = data->m_is_editing;
  m_is_visible = data->m_is_visible;
  return true;
}

bool tocRenderer::GetValue(wxVariant &value) const {
  auto *data = new tocRendererData();
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
