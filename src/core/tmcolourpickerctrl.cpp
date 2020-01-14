/***************************************************************************
 tmcolourpickerctrl.cpp
 Create a nice colour picker even on mac
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// comment doxygen

#include "tmcolourpickerctrl.h"

#if (__WXMAC__)  // MAC SPECIFIC COLOUR PICKER

/***************************************************************************/ /**
  @brief Simple contructor
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 August 2008
  *******************************************************************************/
tmColourPickerCtrl::tmColourPickerCtrl() {
  InitMemberValues();
}

/***************************************************************************/ /**
  @brief Constructor
  @details Under mac OS, a wxBitmapButton is created and a bitmap of size :
  #tmCOLOURPICKERCTRL_SIZE is used for displaying color. Under windows the
  default wxColourPickerCtrl is used
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 August 2008
  *******************************************************************************/
tmColourPickerCtrl::tmColourPickerCtrl(wxWindow* parent, wxWindowID id, const wxColour& colour, const wxPoint& pos,
                                       const wxSize& size) {
  InitMemberValues();
  m_Colour = colour;
  wxBitmapButton::Create(parent, id, CreateColourBitmap(colour), pos, size);
  // connecting event
  Connect(id, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(tmColourPickerCtrl::OnChooseColour));
}

/***************************************************************************/ /**
  @brief Destructor
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 August 2008
  *******************************************************************************/
tmColourPickerCtrl::~tmColourPickerCtrl() {}

/***************************************************************************/ /**
  @brief Init private member values
  @details Actually, only #m_Colour is initialised with a black value.
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 August 2008
  *******************************************************************************/
void tmColourPickerCtrl::InitMemberValues() {
  m_Colour = wxColour(*wxBLACK);
}

/***************************************************************************/ /**
  @brief Create a bitmap of specified colour
  @details #tmCOLOURPICKERCTRL_SIZE variable is used for the bitmap size.
  @param col the desired colour for the bitmap
  @return  a bitmap of size #tmCOLOURPICKERCTRL_SIZE filled with colour col.
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 August 2008
  *******************************************************************************/
wxBitmap tmColourPickerCtrl::CreateColourBitmap(const wxColour& col) {
  wxBitmap myBitmap(tmCOLOURPICKERCTRL_SIZE.GetWidth(), tmCOLOURPICKERCTRL_SIZE.GetHeight());
  wxMemoryDC memdc;
  memdc.SelectObject(myBitmap);
  memdc.SetBrush(wxBrush(col));
  memdc.SetPen(wxPen(col));
  memdc.DrawRectangle(0, 0, tmCOLOURPICKERCTRL_SIZE.GetWidth(), tmCOLOURPICKERCTRL_SIZE.GetHeight());
  memdc.SelectObject(wxNullBitmap);
  return myBitmap;
}

/***************************************************************************/ /**
  @brief Set the colour
  @param col New colour for the control
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 August 2008
  *******************************************************************************/
void tmColourPickerCtrl::SetColour(const wxColour& col) {
  m_Colour = col;
  SetBitmapLabel(CreateColourBitmap(col));
}

/***************************************************************************/ /**
  @brief Called when user press the button for changing colour
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 August 2008
  *******************************************************************************/
void tmColourPickerCtrl::OnChooseColour(wxCommandEvent& event) {
  wxColour selcol = wxGetColourFromUser(this, m_Colour);
  if (!selcol.IsOk()) {
    wxLogDebug(_T("Error selecting color, color invalid"));
    return;
  }
  SetColour(selcol);
}

#else  // STANDARD COLOUR PICKER WINDOWS & LINUX

/***************************************************************************/ /**
  @brief Linux, windows constructor
  @details Used only in conditionnal compilation and create a wxColourPickerCtrl
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 August 2008
  *******************************************************************************/
tmColourPickerCtrl::tmColourPickerCtrl(wxWindow *parent, wxWindowID id, const wxColour &colour, const wxPoint &pos,
                                       const wxSize &size) {
  wxColourPickerCtrl::Create(parent, id, colour, pos, size);
}

#endif
