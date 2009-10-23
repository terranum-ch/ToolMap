/***************************************************************************
								tmmemoryzoom.h
                    	Store Previous Zoom in memory 
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
    email                : lucien.schreiber at crealp dot vs dot ch
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






class tmZoomExtent 
{
  public:
    wxPoint2DDouble m_TopLeftPosition;
    double m_ZoomFactor;

    tmZoomExtent();
	tmZoomExtent(double top, double left, double pixelsize);
    ~tmZoomExtent();

};
// Creating a list of MemoryFields
WX_DECLARE_OBJARRAY(tmZoomExtent, tmArrayZoomExtent);


class tmMemoryZoomManager : public wxEvtHandler 
{
private:
    tmArrayZoomExtent m_ZoomExtents;
    int m_MaxSize;
	
  public:
    tmMemoryZoomManager(int maxsize = 50);
    ~tmMemoryZoomManager();

    bool Add(const tmZoomExtent & value);

    bool Add(double top, double left, double zoomfactor);

    int GetCount();

    tmZoomExtent GetPrevious();




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
