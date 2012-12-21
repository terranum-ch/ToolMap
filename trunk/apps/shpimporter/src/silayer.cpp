/***************************************************************************
 silayer.cpp
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber
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

#include "silayer.h"

siLayer::siLayer(const wxString & layerpath, DataBase * database) {
}



siLayer::~siLayer() {
}



bool siLayer::LoadFromTxt(const wxString & text) {
    return false;
}



bool siLayer::Process() {
    return false;
}


const siKind siLayer::GetKindRef() {
    return m_Kind;
}

