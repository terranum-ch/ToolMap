/***************************************************************************
 tmmemoryzoom.cpp
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

#include "tmmemoryzoom.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(tmArrayZoomExtent);

/***************************************************************************/ /**
  tmZoomExtent
 *******************************************************************************/
tmZoomExtent::tmZoomExtent() {
  m_TopLeftPosition = wxPoint2DDouble(0.0, 0.0);
  m_ZoomFactor = wxNOT_FOUND;
}

tmZoomExtent::~tmZoomExtent() {}

tmZoomExtent::tmZoomExtent(double top, double left, double pixelsize) {
  m_TopLeftPosition = wxPoint2DDouble(top, left);
  m_ZoomFactor = pixelsize;
}

bool tmZoomExtent::operator==(const tmZoomExtent &zoom) const {
  if (m_TopLeftPosition != zoom.m_TopLeftPosition) return false;

  if (wxIsSameDouble(m_ZoomFactor, zoom.m_ZoomFactor) == false) return false;

  return true;
}

bool tmZoomExtent::IsOk() {
  if (m_TopLeftPosition == wxPoint2DDouble(0.0, 0.0)) return false;

  if (m_ZoomFactor == wxNOT_FOUND) return false;

  return true;
}

/***************************************************************************/ /**
  tmMemoryZoomManager
 *******************************************************************************/
tmMemoryZoomManager::tmMemoryZoomManager(int maxsize) {
  m_MaxSize = maxsize;
  m_ZoomExtents.Clear();
}

tmMemoryZoomManager::~tmMemoryZoomManager() {
  m_ZoomExtents.Clear();
}

bool tmMemoryZoomManager::Add(const tmZoomExtent &value) {
  m_ZoomExtents.Add(value);

  // only m_Maxsize item are allowed
  int myDiff = GetCount() - m_MaxSize;
  if (myDiff > 0) {
    m_ZoomExtents.RemoveAt(0, myDiff);
  }
  return true;
}

bool tmMemoryZoomManager::Add(double top, double left, double zoomfactor) {
  tmZoomExtent myExtent(top, left, zoomfactor);
  return Add(myExtent);
}

int tmMemoryZoomManager::GetCount() {
  int iUsableNumer = m_ZoomExtents.GetCount() - 1;
  if (iUsableNumer < 0) iUsableNumer = 0;

  return iUsableNumer;
}

/***************************************************************************/ /**
 @brief Return previous zoom level
 @details This function return the previous zoom level, once returned the zoom
  level isn't anymore accessible
 @param extent will be filled with the previous zoom level if true is returned
 @return  true if there is some zoom level availlable
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 octobre 2009
 *******************************************************************************/
bool tmMemoryZoomManager::GetPrevious(tmZoomExtent &extent) {
  if (GetCount() < 1) return false;

  extent = m_ZoomExtents.Item(m_ZoomExtents.GetCount() - 2);
  m_ZoomExtents.RemoveAt(m_ZoomExtents.GetCount() - 1, 1);
  return true;
}

void tmMemoryZoomManager::Clear() {
  m_ZoomExtents.Clear();
}

/*
void tmLayerManager::_ZoomChanged() {
}

void tmLayerManager::OnZoomPrevious(const wxCommandEvent & event) {
}
*/
