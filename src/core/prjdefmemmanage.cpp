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
	m_PrjAuthors	= _T("");
	m_PrjSummary	= _T("");
	m_PrjUnitType	= UNIT_METERS;
	m_PrjProjType	= PROJ_NOPROJ;
	m_PrjLayerArray = new PrjMemLayersArray();
	
	m_pActiveLayer = NULL;
	m_pActiveField = NULL;
	m_iActualObj = 0;
	m_iActualField = 0;
	m_iActualCodedVal = 0;
	m_iActualLayers = 0;
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
	//wxLogDebug(_T("Array Size : Layer = %d"),m_PrjLayerArray->GetCount());
	
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
			//wxLogDebug(_T("Object found in Layer array in position : %d"), i);
			// set active layer
			SetActiveLayer(&(m_PrjLayerArray->Item(i)));
			return &(m_PrjLayerArray->Item(i));
		}
	}
	wxLogDebug(_T("Object not found in Layer array"));
	return NULL; // nothing found... check for null pointer
}


ProjectDefMemoryLayers * PrjDefMemManage::FindLayer(unsigned int iIndex)
{
	if (iIndex <= m_PrjLayerArray->GetCount())
	{
		SetActiveLayer(&(m_PrjLayerArray->Item(iIndex)));
		return &(m_PrjLayerArray->Item(iIndex));
	}
	
	return NULL; // out of the bound
}

/***************************************************************************//**
 @brief Find field based on real ID
 @param iIndex The Real ID for the field
 @return  A valid pointer to a #ProjectDefMemoryFields if field is found and
 NULL otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 June 2008
 *******************************************************************************/
ProjectDefMemoryLayers * PrjDefMemManage::FindLayerByRealID (unsigned int iIndex)
{
	// search this item in the array for the good layer name.
	for (unsigned int i=0; i < m_PrjLayerArray->GetCount(); i++)
	{
		if (m_PrjLayerArray->Item(i).m_LayerID == (signed int) iIndex)
		{
			//wxLogDebug(_T("Object found in Layer array in position : %d"), i);
			// set active layer
			SetActiveLayer(&(m_PrjLayerArray->Item(i)));
			return &(m_PrjLayerArray->Item(i));
		}
	}
	wxLogDebug(_T("Object not found in Layer array"));
	return NULL; // nothing found... check for null pointer
}



ProjectDefMemoryLayers * PrjDefMemManage::GetNextLayer()
{
	
	// deal with the increment of the retruned item
	if (m_iActualLayers >= GetCountLayers())
	{
		m_iActualLayers = 0;
	}
	
	ProjectDefMemoryLayers * layer = &(m_PrjLayerArray->Item(m_iActualLayers));
	
	// set the active layer
	SetActiveLayer(layer);
	
	// increment the object returned
	m_iActualLayers ++;
	
	return layer;	
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
		//wxLogDebug(_T("Array Size : Object = %d"),layer->m_pLayerObjectArray->GetCount());
		
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
			//wxLogDebug(_T("Object found in object array in position : %d"), i);
			layer->m_pLayerObjectArray->RemoveAt(i);
			return TRUE;
		}
	}
	wxLogDebug(_T("Object not found in object array"));
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
			//wxLogDebug(_T("Object found in object array in position : %d"), i);
			return &(layer->m_pLayerObjectArray->Item(i));
		}
	}
	
	return NULL; // nothing found... check for null pointer
}


ProjectDefMemoryObjects *	PrjDefMemManage::FindObject(unsigned int iIndex)
{
	// get the active layer
	ProjectDefMemoryLayers * layer = GetActiveLayer();
	
	if (iIndex <= layer->m_pLayerObjectArray->GetCount())
	{
		return &(layer->m_pLayerObjectArray->Item(iIndex));
	}

	return NULL; // nothing found... check for null pointer
	
}


ProjectDefMemoryObjects * PrjDefMemManage::GetNextObjects()
{
	// get the active layer
	ProjectDefMemoryLayers * layer = GetActiveLayer();
	
	// deal with the increment of the retruned item
	if (m_iActualObj >= GetCountObject())
	{
		m_iActualObj = 0;
	}
	
	ProjectDefMemoryObjects * object = &(layer->m_pLayerObjectArray->Item(m_iActualObj));
	
	// increment the object returned
	m_iActualObj ++;
	
	return object;
}


int	PrjDefMemManage::GetCountObject()
{
	// get the active layer
	ProjectDefMemoryLayers * layer = GetActiveLayer();
	
	return layer->m_pLayerObjectArray->GetCount();
}




/*********************** FIELDS FUNCTIONS **************************/
ProjectDefMemoryFields * PrjDefMemManage::AddField ()
{
	// create an empty object and attach it
	ProjectDefMemoryFields * myNewFieldObj = new ProjectDefMemoryFields();
	
	ProjectDefMemoryLayers * layer;
	layer = GetActiveLayer();
	if (layer != NULL)
	{
		
		layer->m_pLayerFieldArray.Add(myNewFieldObj);
		//wxLogDebug(_T("Array Size : Field = %d"),layer->m_pLayerFieldArray->GetCount());
		
		// set the active field
		SetActiveField(myNewFieldObj);
		
		return myNewFieldObj; // pointer to the added field.
	}
	return NULL;
	
}


int	PrjDefMemManage::RemoveField (int iIndex)
{
	// get the active layer
	ProjectDefMemoryLayers * layer = GetActiveLayer();
	
	if (iIndex == -1)
		iIndex = layer->m_pLayerFieldArray.GetCount() - 1;
	
	// be sure that iIndex isn't smaller than 0
	wxASSERT_MSG (iIndex >= 0, _T("Array index smaller than 0"));
	
	ProjectDefMemoryFields * myField = layer->m_pLayerFieldArray.Item(iIndex);
	layer->m_pLayerFieldArray.Detach(iIndex);
	wxDELETE(myField);
	
	// set null for active field
	SetActiveField(NULL);
		
	return layer->m_pLayerFieldArray.GetCount(); // number of objects	
}



bool PrjDefMemManage::RemoveField(const wxString & FieldName)
{
	// get the active layer
	ProjectDefMemoryLayers * layer = GetActiveLayer();
	
	// search this item in the array for the good layer name.
	for (unsigned int i=0; i < layer->m_pLayerFieldArray.GetCount(); i++)
	{
		if (layer->m_pLayerFieldArray.Item(i)->m_Fieldname == FieldName)
		{
			//wxLogDebug(_T("Object found in Field array in position : %d"), i);
			RemoveField(i);
			return true;
		}
	}
	
	wxLogDebug(_T("Object not found in Field array"));
	return false; // nothing deleted.
}


ProjectDefMemoryFields * PrjDefMemManage::FindField(const wxString & FieldName)
{
	// get the active layer
	ProjectDefMemoryLayers * layer = GetActiveLayer();
	
	// search this item in the array for the good layer name.
	for (unsigned int i=0; i < layer->m_pLayerFieldArray.GetCount(); i++)
	{
		if (layer->m_pLayerFieldArray.Item(i)->m_Fieldname == FieldName)
		{
			//wxLogDebug(_T("Object found in Field array in position : %d"), i);
			SetActiveField(layer->m_pLayerFieldArray.Item(i));
			return layer->m_pLayerFieldArray.Item(i);
		}
	}
	
	return NULL; // nothing found, return null pointer.
}





ProjectDefMemoryFields * PrjDefMemManage::FindField(unsigned int iIndex)
{
	// get the active layer
	ProjectDefMemoryLayers * layer = GetActiveLayer();
	
	
	if (iIndex <= layer->m_pLayerFieldArray.GetCount())
	{
		SetActiveField(layer->m_pLayerFieldArray.Item(iIndex));
		return layer->m_pLayerFieldArray.Item(iIndex);
	}
	
	
	return NULL; // nothing found, return null pointer.
	
}


ProjectDefMemoryFields *PrjDefMemManage::GetNextField()
{
	// get the active layer
	ProjectDefMemoryLayers * layer = GetActiveLayer();
	
	// deal with the increment of the retruned item
	if (m_iActualField >= GetCountFields())
	{
		m_iActualField = 0;
	}
	
	ProjectDefMemoryFields * field = layer->m_pLayerFieldArray.Item(m_iActualField);
	
	// increment the object returned
	m_iActualField ++;
	
	return field;
	
}



int	PrjDefMemManage::GetCountFields()
{
	// get the active layer
	ProjectDefMemoryLayers * layer = GetActiveLayer();
	
	return layer->m_pLayerFieldArray.GetCount();
}




/*********************** FIELDS CODED VALUES FUNCTIONS **************************/
ProjectDefMemoryFieldsCodedVal * PrjDefMemManage::AddCodedValue()
{
	// create an empty object and attach it
	ProjectDefMemoryFieldsCodedVal * myNewCodedValObj = new ProjectDefMemoryFieldsCodedVal();
	
	ProjectDefMemoryFields * field;
	field = GetActiveField();
	if (field != NULL)
	{
		field->m_pCodedValueArray.Add(myNewCodedValObj);	
		//wxLogDebug(_T("Array Size : Coded Value = %d"),field->m_pCodedValueArray->GetCount());
		
		return myNewCodedValObj; // pointer to the added field.
	}
	return NULL;
}



int	PrjDefMemManage::RemoveCodedValue (int iIndex)
{
	// get the active field
	ProjectDefMemoryFields * field = GetActiveField();
	
	if (iIndex == -1)
		iIndex = field->m_pCodedValueArray.GetCount() - 1;
	
	// be sure that iIndex isn't smaller than 0
	wxASSERT_MSG (iIndex >= 0, _T("Array index smaller than 0"));
	
	ProjectDefMemoryFieldsCodedVal * myVal = field->m_pCodedValueArray.Item(iIndex);
	field->m_pCodedValueArray.Detach(iIndex);
	wxDELETE(myVal);
		
	return field->m_pCodedValueArray.GetCount(); // number of coded values
}


bool PrjDefMemManage::RemoveCodedValue (const wxString & ValueName)
{
	// get the active field
	ProjectDefMemoryFields * field = GetActiveField();
	
	// search this item in the array for the good layer name.
	for (unsigned int i=0; i < field->m_pCodedValueArray.GetCount(); i++)
	{
		if (field->m_pCodedValueArray.Item(i)->m_ValueName == ValueName)
		{
			wxLogDebug(_T("Coded Value found in array in position : %d"), i);
			RemoveCodedValue(i);
			return true;
		}
	}
	
	return FALSE; // nothing deleted.
}


ProjectDefMemoryFieldsCodedVal * PrjDefMemManage::FindCodedValue(const wxString & ValueName, int & IndexPos)
{
	// get the active field
	ProjectDefMemoryFields * field = GetActiveField();
	
	// search this item in the array for the good layer name.
	for (unsigned int i=0; i < field->m_pCodedValueArray.GetCount(); i++)
	{
		if (field->m_pCodedValueArray.Item(i)->m_ValueName == ValueName)
		{
			wxLogDebug(_T("Coded Value found in array in position : %d"), i);
			IndexPos = i;
			return field->m_pCodedValueArray.Item(i);
		}
	}
	
	return NULL; // nothing found, return null pointer.
}


ProjectDefMemoryFieldsCodedVal *PrjDefMemManage::GetNextCodedValue()
{
	// get the active field
	ProjectDefMemoryFields * field = GetActiveField();
	
	// deal with the increment of the retruned item
	if (m_iActualCodedVal >= GetCountCodedValue())
	{
		m_iActualCodedVal = 0;
	}
	
	ProjectDefMemoryFieldsCodedVal * CodedVal = field->m_pCodedValueArray.Item(m_iActualCodedVal);
	
	// increment the object returned
	m_iActualCodedVal ++;
	
	return CodedVal;
}


int	PrjDefMemManage::GetCountCodedValue()
{
	// get the active field
	ProjectDefMemoryFields * field = GetActiveField();
	
	return field->m_pCodedValueArray.GetCount();
}



/*********************** SCALE  FUNCTIONS **************************/
ProjectDefMemoryScale * PrjDefMemManage::AddScale(const long & scale, const int & DBindex)
{
	ProjectDefMemoryScale * mytempScale = new ProjectDefMemoryScale();
	mytempScale->m_ScaleValue = scale;
	mytempScale->m_DBScaleID = DBindex;
	m_ScaleArray.Add(mytempScale);
	
	return mytempScale;
}



ProjectDefMemoryScale * PrjDefMemManage::FindScale (const long & oldscale)
{
	
	// search the array
	for (unsigned int i= 0; i<m_ScaleArray.GetCount(); i++)
	{
		if (m_ScaleArray.Item(i).m_ScaleValue == oldscale)
		{
			return &(m_ScaleArray.Item(i));
		}
	}
	return NULL;
}


bool PrjDefMemManage::RemoveScale (const long & oldscale)
{
	
	// search this item in the array for the good scale value.
	for (unsigned int i=0; i < m_ScaleArray.GetCount(); i++)
	{
		if (m_ScaleArray.Item(i).m_ScaleValue == oldscale)
		{
			//wxLogDebug(_T("Object found in scale array in position : %d"), i);
			m_ScaleArray.RemoveAt(i);
			return TRUE;
		}
	}
	
	return FALSE; // nothing deleted.
}


/***************************************************************************//**
 @brief Overloading = operator
 @author Lucien Schreiber (c) CREALP 2009
 @date 17 March 2009
 *******************************************************************************/
PrjDefMemManage & PrjDefMemManage::operator=(const PrjDefMemManage & source)
{
	m_PrjPath = source.m_PrjPath;
	m_PrjName = source.m_PrjName;
	m_PrjAuthors = source.m_PrjAuthors;
	 m_PrjSummary = source.m_PrjSummary;
	m_PrjUnitType = source.m_PrjUnitType;
	 m_PrjProjType = source.m_PrjProjType;
	
	// arrays not copied (used only for deleting)
	m_StoreDeleteIDObj.Clear();
	m_StoreDeleteScale.Clear();
	m_StoreDeleteLayers.Clear();
	
	// copy scale array
	m_ScaleArray.Clear();
	for (unsigned int i = 0; i<source.m_ScaleArray.GetCount();i++)
		m_ScaleArray.Add(source.m_ScaleArray.Item(i));
	
	// copy layers
	ProjectDefMemoryLayers * myLayer = NULL;
	m_PrjLayerArray->Clear();
	for (unsigned int j = 0; j<source.m_PrjLayerArray->GetCount();j++)
	{
		myLayer = AddLayer();
		*myLayer = source.m_PrjLayerArray->Item(j);
		
		//*myLayer = source.m_PrjLayerArray->Item(j);
		//m_PrjLayerArray->Add(myLayer);
	}
		
	// private members
	m_pActiveLayer = NULL;
	if (source.m_pActiveLayer != NULL)
	{
		//wxLogDebug(_T("Finding Layer %s"), source.m_pActiveLayer->m_LayerName);
		m_pActiveLayer = FindLayer(source.m_pActiveLayer->m_LayerName);
		wxASSERT (m_pActiveLayer);
	}
	
	m_pActiveField = NULL;
	if (source.m_pActiveField != NULL)
	{
		m_pActiveField = FindField(source.m_pActiveField->m_Fieldname);
		/*wxASSERT_MSG (m_pActiveField,
					  wxString::Format(_T("Finding Field Error %s in active layer : %s"), 
									   source.m_pActiveField->m_Fieldname.c_str(),
									   m_pActiveLayer->m_LayerName.c_str()));*/
	}
	
	m_iActualObj = source.m_iActualObj;
	m_iActualField = source.m_iActualField;
	m_iActualCodedVal = source.m_iActualCodedVal;
	m_iActualLayers = source.m_iActualLayers;
	
	return *this;
}


