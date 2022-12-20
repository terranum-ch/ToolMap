#include "tocrenderer.h"

#include "tocctrlmodel.h"
#include "../gui/bitmaps.h"

tocRendererData::tocRendererData() {
    m_layer_name = wxEmptyString;
    m_ImageIndex = 0;
    m_IsEditing = false;
    m_IsVisible = true;
}

tocRendererData* tocRendererData::Clone() const {
    tocRendererData* new_data = new tocRendererData();
    new_data->m_IsEditing = m_IsEditing;
    new_data->m_IsVisible = m_IsVisible;
    new_data->m_ImageIndex = m_ImageIndex;
    new_data->m_layer_name = m_layer_name;
    return new_data;
}

bool tocRendererData::Eq(wxVariantData& data) const {
    tocRendererData* my_data = (tocRendererData*)&data;
    if (m_layer_name != my_data->m_layer_name) {
        return false;
    }
    if (m_ImageIndex != my_data->m_ImageIndex) {
        return false;
    }
    if (m_IsVisible != my_data->m_IsVisible) {
        return false;
    }
    if (m_IsEditing != my_data->m_IsEditing) {
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
// able to activate or edit the cell, and it doesn't make sense to switch
// between the two -- but this is just an example, so it doesn't stop us.
tocRenderer::tocRenderer(wxDataViewCellMode mode, wxDataViewTreeCtrl* parent)
    : wxDataViewCustomRenderer("tocrendererdata", mode, wxALIGN_LEFT) {
    m_ParentCtrl = parent;
    _CreateBitmaps();
}

void tocRenderer::_CreateBitmaps() {  // loading images supporting dark / white bitmaps
    m_ImageList.Create(16, 16, true, 8);
    m_ImageList.Add(Bitmaps::GetTocBitmap(Bitmaps::ID_TOC::FOLDER));
    m_ImageList.Add(Bitmaps::GetTocBitmap(Bitmaps::ID_TOC::SHAPEFILE));
    m_ImageList.Add(Bitmaps::GetTocBitmap(Bitmaps::ID_TOC::DATABASE));
    m_ImageList.Add(Bitmaps::GetTocBitmap(Bitmaps::ID_TOC::IMAGE));
    m_ImageList.Add(Bitmaps::GetTocBitmap(Bitmaps::ID_TOC::WEB));
    m_ImageList.Add(Bitmaps::GetTocBitmap(Bitmaps::ID_TOC::CHECK_ON));
    m_ImageList.Add(Bitmaps::GetTocBitmap(Bitmaps::ID_TOC::CHECK_OFF));
    m_ImageList.Add(Bitmaps::GetTocBitmap(Bitmaps::ID_TOC::PEN));
}

bool tocRenderer::Render(wxRect rect, wxDC* dc, int state) {
    // rectangle and margin
    int margin = 5;
    wxRect rect_checkbox(rect.GetX(), rect.GetY(), 16, 16);
    wxRect rect_layer_type(rect_checkbox.GetX() + rect_checkbox.GetWidth() + margin, rect.GetY(), 16, 16);
    wxRect rect_text(rect_layer_type.GetX() + rect_layer_type.GetWidth() + margin, rect.GetY(),
                     dc->GetTextExtent(m_LayerName).GetWidth(), rect.GetHeight());
    wxRect rect_edit(rect_text.GetX() + rect_text.GetWidth() + margin, rect.GetY(), 16, 16);

    // checkbox image
    wxASSERT(m_ImageList.GetImageCount() > 0);
    wxBitmap bmp_checkbox;
    if (m_IsVisible) {
        bmp_checkbox = m_ImageList.GetBitmap(5);
    } else {
        bmp_checkbox = m_ImageList.GetBitmap(6);
    }

    // layer type image
    wxBitmap bmp_layer_type;
    switch (m_ImageIndex) {
        case 0:  // folder
            bmp_layer_type = m_ImageList.GetBitmap(0);
            break;
        case 1:  // shapefile
            bmp_layer_type = m_ImageList.GetBitmap(1);
            break;
        case 2:  // database
            bmp_layer_type = m_ImageList.GetBitmap(2);
            break;
        case 3:  // image
            bmp_layer_type = m_ImageList.GetBitmap(3);
            break;
        case 4:  // web image
            bmp_layer_type = m_ImageList.GetBitmap(4);
            break;
        default:
            wxLogError(_("Unsupported bitmap index!"));
    }

    // drawing
    dc->DrawBitmap(bmp_checkbox, rect_checkbox.GetX(), rect_checkbox.GetY());
    dc->DrawBitmap(bmp_layer_type, rect_layer_type.GetX(), rect_layer_type.GetY());
    RenderText(m_LayerName, 0, rect_text, dc, state);

    if (m_IsEditing) {
        m_ImageList.Draw(7, *dc, rect_edit.GetX(), rect_edit.GetY());
    }
    return true;
}

bool tocRenderer::ActivateCell(const wxRect& cell, wxDataViewModel* model, const wxDataViewItem& item,
                               unsigned int WXUNUSED(col), const wxMouseEvent* mouseEvent) {
    wxString debug_position = "from keyboard";
    auto* my_model = (TocCtrlModel*)model;
    bool check = false;

    if (mouseEvent) {
        debug_position = wxString::Format("via mouse at %d, %d", mouseEvent->m_x, mouseEvent->m_y);
        wxRect rect_checkbox(0, 0, 16, 16);

        if (rect_checkbox.Contains(mouseEvent->GetPosition())) {
            if (!my_model->IsChecked(item)) {
                check = true;
            }
            my_model->SetChecked(item, check);
            my_model->ItemChanged(item);
            // send activate event to the parent, not needed when using keyboard
            wxASSERT(m_ParentCtrl);
            wxDataViewEvent my_activate_event(wxEVT_DATAVIEW_ITEM_ACTIVATED, m_ParentCtrl, item);
            m_ParentCtrl->GetEventHandler()->ProcessEvent(my_activate_event);
        }
    } else {
        if (!my_model->IsChecked(item)) {
            check = true;
        }
        my_model->SetChecked(item, check);
        my_model->ItemChanged(item);
    }

    wxLogMessage("tocRenderer ActivateCell() %s", debug_position);
    return false;
}

wxSize tocRenderer::GetSize() const {
    return GetView()->FromDIP(wxSize(60, 20));
}

bool tocRenderer::SetValue(const wxVariant& value) {
    auto* data = (tocRendererData*)value.GetData();
    m_LayerName = data->m_layer_name;
    m_ImageIndex = data->m_ImageIndex;
    m_IsEditing = data->m_IsEditing;
    m_IsVisible = data->m_IsVisible;
    return true;
}

bool tocRenderer::GetValue(wxVariant& value) const {
    auto* data = new tocRendererData();
    data->m_layer_name = m_LayerName;
    data->m_ImageIndex = m_ImageIndex;
    data->m_IsVisible = m_IsVisible;
    data->m_IsEditing = m_IsEditing;
    value.SetData(data);
    return true;
}

#if wxUSE_ACCESSIBILITY
// virtual wxString tocRenderer::GetAccessibleDescription() const wxOVERRIDE {
//   return m_LayerName;
// }
#endif  // wxUSE_ACCESSIBILITY

bool tocRenderer::HasEditorCtrl() const {
    return true;
}

wxWindow* tocRenderer::CreateEditorCtrl(wxWindow* parent, wxRect labelRect, const wxVariant& value) {
    wxTextCtrl* text = new wxTextCtrl(parent, wxID_ANY, value, labelRect.GetPosition(), labelRect.GetSize(),
                                      wxTE_PROCESS_ENTER);
    text->SetInsertionPointEnd();

    return text;
}

bool tocRenderer::GetValueFromEditorCtrl(wxWindow* ctrl, wxVariant& value) {
    wxTextCtrl* text = wxDynamicCast(ctrl, wxTextCtrl);
    if (!text) return false;

    value = text->GetValue();
    return true;
}
