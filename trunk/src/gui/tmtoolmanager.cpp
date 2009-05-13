/***************************************************************************
								tmtoolmanager.cpp
                    Take control of all tools (Dangling nodes, etc.)
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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



#include "tmtoolmanager.h"



void tmToolManager::InitMemberValues()
{
	m_Parent = NULL;
	m_TOC = NULL;
	m_Selected = NULL;
	m_Renderer = NULL;
	m_Scale = NULL;
	m_pDB = NULL;
}



tmToolManager::tmToolManager(wxWindow * parent,
			  tmTOCCtrl * toc,
			  tmSelectedDataMemory * seldata,
			  tmRenderer * renderer,
			  tmGISScale * scale)
{
	InitMemberValues();
	m_Parent = parent;
	m_TOC = toc;
	m_Selected = seldata;
	m_Renderer = renderer;
	m_Scale = scale;
}



tmToolManager::~tmToolManager()
{


}