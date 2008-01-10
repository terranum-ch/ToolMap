/***************************************************************************
							prjdefmemmanage.cpp
                    Display context help for a dialog
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


#include "prjdefmemmanage.h"


PrjDefMemManage::PrjDefMemManage()
{
	// pass default values to members
	InitDefaultValues();
}

PrjDefMemManage::~PrjDefMemManage()
{
	// destroy the array and free all memory
	if (m_PrjLayerArray->GetCount())
	{
		m_PrjLayerArray->Clear();
		wxLogDebug(_T("Clearing LayerArray of all data"));
	}
	delete m_PrjLayerArray;
}


void PrjDefMemManage::InitDefaultValues()
{
	m_PrjPath		= _T("");
	m_PrjName		= _T("");
	m_PrjUnitType	= UNIT_METERS;
	m_PrjProjType	= PROJ_NOPROJ;
	m_PrjLayerArray = new PrjMemLayersArray();
	
	m_pActiveLayer = NULL;
}



/*********************** LAYER FUNCTIONS **************************/
ProjectDefMemoryLayers * PrjDefMemManage::AddLayer()
{
	// create an empty layer object and attach it to the
	// project layer array
	ProjectDefMemoryLayers * myNewLayerObj = new ProjectDefMemoryLayers();
	
	// attach it, don't delete this myNewLayerObj, it is now part of the
	// array and will only be deleted if not used in the RemoveLayer method.
	m_PrjLayerArray->Add(myNewLayerObj);
	wxLogDebug(_T("Array Size : Layer = %d"),m_PrjLayerArray->GetCount());
	
	// set the active layer
	SetActiveLayer(myNewLayerObj);
	
	return myNewLayerObj; // number of layers
}


int PrjDefMemManage::RemoveLayer(int iIndex)
{
	if (iIndex == -1)
		iIndex = m_PrjLayerArray->GetCount() - 1;
	
	// be sure that iIndex isn't smaller than 0
	wxASSERT_MSG (iIndex >= 0, _T("Array index smaller than 0"));
	
	//ProjectDefMemoryLayers * item = m_PrjLayerArray[iIndex];
	//delete item;
	m_PrjLayerArray->RemoveAt(iIndex);
	
	// set null for active layer
	SetActiveLayer(NULL);
	
	return m_PrjLayerArray->GetCount(); // number of layers
}



bool PrjDefMemManage::RemoveLayer(const wxString & layerName)
{
	// search this item in the array for the good layer name.
	for (unsigned int i=0; i < m_PrjLayerArray->GetCount(); i++)
	{
		if (m_PrjLayerArray->Item(i).m_LayerName == layerName)
		{
			//item found, delete item;
			m_PrjLayerArray->RemoveAt(i);
			// set null for active layer
			SetActiveLayer(NULL);
			return TRUE;
		}
	}
	return FALSE; // item not deleted
}



ProjectDefMemoryLayers * PrjDefMemManage::FindLayer(const wxString  & layerName)
{
	// search this item in the array for the good layer name.
	for (unsigned int i=0; i < m_PrjLayerArray->GetCount(); i++)
	{
		if (m_PrjLayerArray->Item(i).m_LayerName == layerName)
		{
			wxLogDebug(_T("Object found in Layer array in position : %d"), i);
			// set active layer
			SetActiveLayer(&(m_PrjLayerArray->Item(i)));
			return &(m_PrjLayerArray->Item(i));
		}
	}
	
	return NULL; // nothing found... check for null pointer
}



/*********************** OBJECTS FUNCTIONS **************************/
ProjectDefMemoryObjects * PrjDefMemManage::AddObject()
{
	// create an empty object and attach it
	ProjectDefMemoryObjects * myNewObjectObj = new ProjectDefMemoryObjects();
	
	ProjectDefMemoryLayers * layer;
	layer = GetActiveLayer();
	if (layer != NULL)
	{
		
		layer->m_pLayerObjectArray->Add(myNewObjectObj);
		wxLogDebug(_T("Array Size : Object = %d"),layer->m_pLayerObjectArray->GetCount());
		
		return myNewObjectObj; // pointer to the added object.
	}
	return NULL;
}


int	PrjDefMemManage::RemoveObject (int iIndex)
{
	// get the active layer
	ProjectDefMemoryLayers * layer = GetActiveLayer();
	
	if (iIndex == -1)
		iIndex = layer->m_pLayerObjectArray->GetCount() - 1;
	
	// be sure that iIndex isn't smaller than 0
	wxASSERT_MSG (iIndex >= 0, _T("Array index smaller than 0"));
	
	layer->m_pLayerObjectArray->RemoveAt(iIndex);
	
	
	return layer->m_pLayerObjectArray->GetCount(); // number of objects
	
}


bool PrjDefMemManage::RemoveObject(const wxString & ObjectName)
{
	// get the active layer
	ProjectDefMemoryLayers * layer = GetActiveLayer();
	
	// search this item in the array for the good layer name.
	for (unsigned int i=0; i < layer->m_pLayerObjectArray->GetCount(); i++)
	{
		if (layer->m_pLayerObjectArray->Item(i).m_ObjectName == ObjectName)
		{
			wxLogDebug(_T("Object found in object array in position : %d"), i);
			layer->m_pLayerObjectArray->RemoveAt(i);
			return TRUE;
		}
	}
	
	return FALSE; // nothing deleted.
}


ProjectDefMemoryObjects *	PrjDefMemManage::FindObject(const wxString & ObjectName)
{
	// get the active layer
	ProjectDefMemoryLayers * layer = GetActiveLayer();
	
	// search this item in the array for the good layer name.
	for (unsigned int i=0; i < layer->m_pLayerObjectArray->GetCount(); i++)
	{
		if (layer->m_pLayerObjectArray->Item(i).m_ObjectName == ObjectName)
		{
			wxLogDebug(_T("Object found in object array in position : %d"), i);
			return &(layer->m_pLayerObjectArray->Item(i));
		}
	}
	
	return NULL; // nothing found... check for null pointer
}

