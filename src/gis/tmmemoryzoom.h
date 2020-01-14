/***************************************************************************
 tmmemoryzoom.h
                     Store Previous Zoom in memory
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

#ifndef _TM_MEMORYZOOM_H_
#define _TM_MEMORYZOOM_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include <wx/graphics.h>

/***************************************************************************//**
@brief tmZoomExtent
@details Contain a zoom level
@author Lucien Schreiber (c) CREALP 2009
@date 26 octobre 2009
 *******************************************************************************/
class tmZoomExtent
{
public:
    wxPoint2DDouble m_TopLeftPosition;
    double m_ZoomFactor;

    tmZoomExtent();

    tmZoomExtent(double top, double left, double pixelsize);

    ~tmZoomExtent();

    bool operator==(const tmZoomExtent &zoom) const;

    bool IsOk();
};

WX_DECLARE_OBJARRAY(tmZoomExtent, tmArrayZoomExtent);


/***************************************************************************//**
@brief tmMemoryZoomManager
@details Manage previous zoom level
@author Lucien Schreiber (c) CREALP 2009
@date 26 octobre 2009
 *******************************************************************************/
class tmMemoryZoomManager : public wxEvtHandler
{
private:
    tmArrayZoomExtent m_ZoomExtents;
    int m_MaxSize;

public:
    tmMemoryZoomManager(int maxsize = 50);

    ~tmMemoryZoomManager();

    bool Add(const tmZoomExtent &value);

    bool Add(double top, double left, double zoomfactor);

    int GetCount();

    bool GetPrevious(tmZoomExtent &extent);

    void Clear();

};


//TODO: Ajouter dans tmLayerManager
/*
class tmLayerManager {
  private:
    tmMemoryZoomManager * m_ZoomManager;

    void _ZoomChanged();


  public:
    void OnZoomPrevious(const wxCommandEvent & event);

};*/
#endif
