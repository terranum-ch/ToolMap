#include "toccustomrenderer.h"
#include "bitmaps.h"


  // This renderer can be either activatable or editable, for demonstration
  // purposes. In real programs, you should select whether the user should be
  // able to activate or edit the cell and it doesn't make sense to switch
  // between the two -- but this is just an example, so it doesn't stop us.
  MyCustomRenderer::MyCustomRenderer(wxDataViewCellMode mode) : wxDataViewCustomRenderer("string", mode, wxALIGN_LEFT) {

  }

   bool MyCustomRenderer::Render(wxRect rect, wxDC *dc, int state)  {
    int margin = 5;
    dc->DrawBitmap(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_check_on, wxSize(16,16)).GetBitmap(wxSize(16,16)), rect.GetX(), rect.GetY());
    dc->DrawBitmap(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_folder, wxSize(16,16)).GetBitmap(wxSize(16,16)), rect.GetX()+margin+16, rect.GetY());
    dc->SetBrush(*wxLIGHT_GREY_BRUSH);
    dc->SetPen(*wxTRANSPARENT_PEN);

    wxRect my_text_rect = rect;
    my_text_rect.SetX(rect.GetX() + (2*margin) + 32);
    my_text_rect.SetWidth(dc->GetTextExtent(m_layer_name).GetWidth());

    dc->DrawBitmap(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_pen, wxSize(16,16)).GetBitmap(wxSize(16,16)), my_text_rect.GetX() + my_text_rect.GetWidth() + margin, rect.GetY());

    //rect.Deflate(2);
    //dc->DrawRoundedRectangle(rect, 5);

    RenderText(m_layer_name,
               0,  // no offset
               my_text_rect, dc, state);
    return true;
  }

   bool MyCustomRenderer::ActivateCell(const wxRect &WXUNUSED(cell), wxDataViewModel *WXUNUSED(model),
                            const wxDataViewItem &WXUNUSED(item), unsigned int WXUNUSED(col),
                            const wxMouseEvent *mouseEvent)  {
    wxString position;
    if (mouseEvent)
      position = wxString::Format("via mouse at %d, %d", mouseEvent->m_x, mouseEvent->m_y);
    else
      position = "from keyboard";
    wxLogMessage("MyCustomRenderer ActivateCell() %s", position);
    return false;
  }

   wxSize MyCustomRenderer::GetSize() const  {
    return GetView()->FromDIP(wxSize(60, 20));
  }

   bool MyCustomRenderer::SetValue(const wxVariant &value)  {
    m_layer_name = value.GetString();
    return true;
  }

   bool MyCustomRenderer::GetValue(wxVariant &WXUNUSED(value)) const  {
    return true;
  }

#if wxUSE_ACCESSIBILITY
  virtual wxString GetAccessibleDescription() const wxOVERRIDE {
    return m_layer_name;
  }
#endif  // wxUSE_ACCESSIBILITY

   bool MyCustomRenderer::HasEditorCtrl() const  {
    return true;
  }

   wxWindow *MyCustomRenderer::CreateEditorCtrl(wxWindow *parent, wxRect labelRect, const wxVariant &value)  {
    wxTextCtrl *text =
        new wxTextCtrl(parent, wxID_ANY, value, labelRect.GetPosition(), labelRect.GetSize(), wxTE_PROCESS_ENTER);
    text->SetInsertionPointEnd();

    return text;
  }

   bool MyCustomRenderer::GetValueFromEditorCtrl(wxWindow *ctrl, wxVariant &value)  {
    wxTextCtrl *text = wxDynamicCast(ctrl, wxTextCtrl);
    if (!text) return false;

    value = text->GetValue();

    return true;
  }


