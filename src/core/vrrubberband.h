/***************************************************************************
 vrrubberband.h
 -------------------
 copyright : (C) 2010 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _VRRUBBERBAND_H
#define _VRRUBBERBAND_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/overlay.h>

class vrRubberBand {
 private:
  wxWindow *m_Window;
  wxPoint m_PointStart;
  wxPoint m_PointEnd;
  wxOverlay m_Overlay;

  wxRect _CreateRect();

 public:
  vrRubberBand(wxWindow *window);

  virtual ~vrRubberBand();

  void SetPointFirst(const wxPoint &pt);

  void SetPointLast(const wxPoint &pt);

  wxRect GetRect();

  bool IsValid();

  bool IsPositive();

  void Update();
};

#endif
