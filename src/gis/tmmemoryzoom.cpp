/***************************************************************************
								tmmemoryzoom.cpp
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

#include "tmmemoryzoom.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY (tmArrayZoomExtent);

tmZoomExtent::tmZoomExtent() 
{
	m_TopLeftPosition = wxPoint2DDouble(0.0,0.0);
	m_ZoomFactor = wxNOT_FOUND;
}

tmZoomExtent::~tmZoomExtent() 
{
}


tmZoomExtent::tmZoomExtent(double top, double left, double pixelsize)
{
	m_TopLeftPosition = wxPoint2DDouble(top,left);
	m_ZoomFactor = pixelsize;
}


tmMemoryZoomManager::tmMemoryZoomManager(int maxsize)
{
	m_MaxSize = maxsize;
	m_ZoomExtents.Clear();
}



tmMemoryZoomManager::~tmMemoryZoomManager() 
{
	m_ZoomExtents.Clear();
}

bool tmMemoryZoomManager::Add(const tmZoomExtent & value) 
{
	m_ZoomExtents.Add(value);
	return true;
}

bool tmMemoryZoomManager::Add(double top, double left, double zoomfactor) 
{
	tmZoomExtent myExtent (top, left, zoomfactor);
	return Add(myExtent);
}

int tmMemoryZoomManager::GetCount() 
{
	return m_ZoomExtents.GetCount();
}

tmZoomExtent tmMemoryZoomManager::GetPrevious() {
}

/*
void tmLayerManager::_ZoomChanged() {
}

void tmLayerManager::OnZoomPrevious(const wxCommandEvent & event) {
}
*/
