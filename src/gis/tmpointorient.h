/***************************************************************************
 tmpointorient.h
                    for dealing with point orientation
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
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

#ifndef _TM_POINT_ORIENT_H_
#define _TM_POINT_ORIENT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/graphics.h>

class tmPointOrient : public wxObject {
 private:
  wxPoint m_StartPoint;
  wxPoint m_EndPoint;

  void InitMemberValues();

 protected:
 public:
  // ctor - dtor
  tmPointOrient();

  ~tmPointOrient();

  // setter - getter
  void SetStartPoint(const wxPoint &start);

  void SetEndPoint(const wxPoint &end);

  int GetOrientationInt();

  double GetOrientationDouble();

  // checks
  virtual bool IsValid();
};

#endif
