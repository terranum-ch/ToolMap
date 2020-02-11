/***************************************************************************
 vrprogress.h
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

#ifndef _VRPROGRESS_H_
#define _VRPROGRESS_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/progdlg.h>

#include "../core/tmpercent.h"
#include "gdal_priv.h"

class vrProgress {
 public:
  vrProgress();

  virtual ~vrProgress();
};

int CPL_STDCALL GDALUpdateSimple(double dfComplete, const char *pszMessage, void *pProgressArg);

class vrProgressSimple : public vrProgress {
 private:
  wxProgressDialog *m_ProgressWnd;
  tmPercent m_Percent;
  bool m_Continue;

 public:
  vrProgressSimple(wxWindow *parent, wxString title, wxString message);

  virtual ~vrProgressSimple();

  inline tmPercent *GetPercent();

  inline const bool GetContinue() const;

  void UpdateProgress();
};

inline tmPercent *vrProgressSimple::GetPercent() {
  return &m_Percent;
}

inline const bool vrProgressSimple::GetContinue() const {
  return m_Continue;
}

#endif
