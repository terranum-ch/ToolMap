#include "bitmaps.h"

wxString
    Bitmaps::SvgToolbar[] = {
    // SELECT
    R"(<svg xmlns="http://www.w3.org/2000/svg" height="40" width="40"><path d="m12.792 24.667 4.125-5.792h8.416L12.792 9.042Zm10.416 11.875-6.125-13.125L10 33.333v-30l23.333 18.334H21.208l6 13Zm-6.291-17.667Z" style="fill:%s"/></svg>)",
    // ZOOM_FIT
    R"(<svg xmlns="http://www.w3.org/2000/svg" height="40" width="40"><path d="M5 35v-9.708h2.792v4.958l5.833-5.833 1.958 1.958-5.833 5.833h4.958V35Zm20.292 0v-2.792h4.958l-5.792-5.791 1.959-1.959 5.791 5.792v-4.958H35V35ZM13.583 15.542 7.792 9.75v4.958H5V5h9.708v2.792H9.75l5.792 5.791Zm12.834 0-1.959-1.959 5.792-5.791h-4.958V5H35v9.708h-2.792V9.75Z" style="fill:%s"/></svg>)",
};

wxString Bitmaps::SvgLogo = "";
wxString Bitmaps::ColorBlack = "#444444";
wxString Bitmaps::ColorWhite = "#FFFFFF";

wxString Bitmaps::GetColor() {
  wxSystemAppearance s = wxSystemSettings::GetAppearance();
  if (s.IsDark()) {
    return Bitmaps::ColorWhite;
  }
  return Bitmaps::ColorBlack;
};

wxBitmap Bitmaps::GetToolbarBitmap(Bitmaps::ID_TOOLBAR id, const wxSize& size) {
  wxString my_bmp = wxString::Format(Bitmaps::SvgToolbar[id], GetColor());
  return wxBitmapBundle::FromSVG(my_bmp, size).GetBitmap(size);
}

wxBitmap Bitmaps::GetLogo(const wxSize& size) {
  return wxBitmapBundle::FromSVG(Bitmaps::SvgLogo, size).GetBitmap(size);
}
