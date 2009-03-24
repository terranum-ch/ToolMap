/***************************************************************************
								tmattributiondata.h
                    Main class for attribution process
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

// comment doxygen

#include "tmattributiondata.h"



/***************************************************************************//**
 @brief Default constructor
 @details Use the tmAttributionData::Create() function after for two step
 construction
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionData::tmAttributionData()
{
	InitMemberValues();
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionData::~tmAttributionData()
{

}



/***************************************************************************//**
 @brief Member values initialisation
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
void tmAttributionData::InitMemberValues()
{
	m_TableName = wxEmptyString;
	m_SelIDs = NULL;
}


void tmAttributionData::Create (wxArrayLong * selected,DataBaseTM * database)
{
	if (selected)
		m_SelIDs = selected;
	
	if (database)
		m_pDB = database;
	
}


tmAttributionData::tmAttributionData(wxArrayLong * selected,DataBaseTM * database)
{
	InitMemberValues();
	Create(selected, database);	
}



/***************************************************************************//**
 @brief Verify validity of member objects
 @return true if all objects are valid and correctly initialized,
 false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
bool tmAttributionData::IsValid ()
{
	bool bRetVal = true;
	if (!m_SelIDs)
		bRetVal = false;
	
	if (!m_pDB)
		bRetVal = false;
	
	if (m_TableName == wxEmptyString)
		bRetVal = false;
	
	wxASSERT_MSG(bRetVal, _T("Error, tmAttributionData wasn't correctly initialized"));
	
	return bRetVal;
}



/***************************************************************************//**
 @brief Create MySQL statement for attribution
 @details This function may be used for preparing generic statement for
 attribution for TOC_NAME_LINES, TOC_NAME_LABELS and TOC_NAME_POINTS. For
 TOC_NAME_ANNOTATION it won't work.
 @param statement String for storing statement
 @param tablename name of the database table to insert into (generic_aat, etc.)
 @param checkedVal Adress of an array containing all checked IDs
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
void tmAttributionData::PrepareAttributionStatement (wxString & statement,
													const wxString & tablename,
													 wxArrayLong * checkedVal)
{
	statement.Clear();
	
	// clean before inserting
	PrepareCleaningStatement(statement, tablename);
	
	wxString sTmp = _T("INSERT INTO ") + tablename + _T(" VALUES (%d, %d); ");
	
	unsigned int geomNumber = m_SelIDs->GetCount();
	unsigned int valNumber = checkedVal->GetCount();

	// loop for all selected geometry
	for (unsigned int geom= 0; geom< geomNumber; geom++)
	{
		
		// loop for values 
		for (unsigned int val = 0; val < valNumber; val++)
				statement.Append(wxString::Format(sTmp, checkedVal->Item(val),
												  m_SelIDs->Item(geom)));
	
	}
	
}


/***************************************************************************//**
 @brief Create MySQL statement for attribution
 @details This function may be used for preparing generic statement for
 cleaning, a.k.a as removing attribution for TOC_NAME_LINES, TOC_NAME_LABELS
 and TOC_NAME_POINTS. For TOC_NAME_ANNOTATION it won't work.
 @param statement String for storing statement
 @param tablename name of the database table to insert into (generic_aat, etc.)
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
void tmAttributionData::PrepareCleaningStatement (wxString & statement,
												  const wxString & tablename)
{
	statement.Clear();
	wxString sTmp = _T("DELETE FROM ") + tablename + _T(" WHERE OBJECT_GEOM_ID=%d; "); 
	
	for (unsigned int i = 0; i<m_SelIDs->GetCount(); i++)
		statement.Append(wxString::Format(sTmp, m_SelIDs->Item(i)));
	
}



/***************************************************************************//**
 @brief Create MySQL statement for getting info
 @details Returning info works only for one selected feature so we get selected
 values only for the first selected values.
 @param statement a place for storing the statement
 @param tablename Name of the table : see #TABLE_NAME_GIS_ATTRIBUTION
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 November 2008
 *******************************************************************************/
void tmAttributionData::PrepareGetInfoStatement (wxString & statement, const wxString & tablename)
{
	statement.Clear();
	wxString sTmp = _T("SELECT OBJECT_VAL_ID FROM ") + tablename + 
					_T(" WHERE OBJECT_GEOM_ID=%d; ");
	
	statement.Append(wxString::Format(sTmp, m_SelIDs->Item(0)));
}



/***************************************************************************//**
 @brief Create MySQL statement for getting info
 @details Get info for all selected object
 @param statement used for storing the returned statement
 @param tablename the tablename to use
 @author Lucien Schreiber (c) CREALP 2009
 @date 02 March 2009
 *******************************************************************************/
void tmAttributionData::PrepareGetInfoMultipleStatement (wxString & statement, const wxString & tablename)
{
	wxASSERT (m_SelIDs);
	
	statement = _T("SELECT OBJECT_GEOM_ID, OBJECT_VAL_ID from ") + tablename +
	_T(" WHERE OBJECT_GEOM_ID IN (");
	
	for(unsigned int i = 0; i< m_SelIDs->GetCount();i++)
	{
		statement.Append(wxString::Format(_T("%d,"), m_SelIDs->Item(i)));
	}
	statement.RemoveLast(1);
	statement.Append(_T(") ORDER BY OBJECT_GEOM_ID, OBJECT_VAL_ID;"));
						 
}



/***************************************************************************//**
 @brief Set Basic attribution
 @details This function attributes the selected object with the specified
 values. This works in two steps : First we delete all values for selected
 object and then we insert specified values for selected objects
 This function is mainly used for attribution by shortcuts
 @param values An array of long containing the values for attribution
 @return  Return true if all works correctly, otherwise false
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 December 2008
 *******************************************************************************/
bool tmAttributionData::SetAttributeBasicValues(wxArrayLong * values)
{
	wxASSERT (!m_TableName.IsEmpty());
	wxString myClearStatement;
	wxString myAttribStatement;
	
	PrepareCleaningStatement(myClearStatement, m_TableName);
	PrepareAttributionStatement(myAttribStatement, m_TableName, values);
	
	wxASSERT (m_pDB);
	if (!m_pDB->DataBaseQueryNoResult(myClearStatement + myAttribStatement))
	{
		wxLogDebug(_T("Error trying to attributes objects : %s"),
				   m_pDB->DataBaseGetLastError().c_str());
		return false;
		
	}
	
	return true;
}



/***************************************************************************//**
 @brief Copy attribution between objects
 @details Use tmAttributionData::Create for setting database and the 
 Multiples OID of the objects we copy attributes to
 @param copyfrom OID of the object we copy attributes from
 @return  true if copy succeed, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 16 February 2009
 *******************************************************************************/
bool tmAttributionData::CopyAttributesBasic (const long & copyfrom)
{
	
	wxASSERT (!m_TableName.IsEmpty());
	wxASSERT (IsValid());
	if (IsValid()==false)
		return false;
	
	wxArrayLong myAttribValues;
	if (GetInfoBasicValues(copyfrom, myAttribValues)== false)
		return false;
	
	if (myAttribValues.GetCount() == 0)
	{
		wxLogDebug(_T("No attribution to copy"));
		return false;
	}
	
	if (SetAttributeBasicValues(&myAttribValues)== false)
		return false;
	
	return true;
}


/***************************************************************************//**
 @brief Get basic attribution values for passed ID
 @param selected The feature ID we want attribution for
 @param values The returned attribution values
 @return true if attributions where returned, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 13 February 2009
 *******************************************************************************/
bool tmAttributionData::GetInfoBasicValues (const long & selected, 
												wxArrayLong & values)
{
	// checking
	wxASSERT (IsValid());
	if (!IsValid())
		return false;
	
	// getting values
	wxString sStatement = _T("");
	
	m_SelIDs->Insert(selected,0);
	PrepareGetInfoStatement(sStatement, m_TableName);
	m_SelIDs->RemoveAt(0);
	
	if (!m_pDB->DataBaseQuery(sStatement))
	{
		wxLogDebug(_T("Error getting info : %s"),
				   m_pDB->DataBaseGetLastError().c_str());
		return false;
	}
	
	
	long mySelTemp = wxNOT_FOUND;
	while (1)
	{
		mySelTemp = m_pDB->DataBaseGetNextResultAsLong();
		if (mySelTemp == wxNOT_FOUND)
			break;
		values.Add(mySelTemp);
	}
	
	return true;
}



/***************************************************************************//**
 @brief Get basic attributions for selected objets
 @param values object of type : #tmAttributionBasic Array containing all ids and
 values for selected objects
 @return  true if attribution was retrieved, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 02 March 2009
 *******************************************************************************/
bool tmAttributionData::GetInfoBasicArray(tmAttributionBasicArray & values)
{
	wxASSERT(IsValid());
	wxString sStatement = wxEmptyString;
	
	//if (m_SelIDs
	PrepareGetInfoMultipleStatement(sStatement, m_TableName);

	if (!m_pDB->DataBaseQuery(sStatement))
	{
		wxLogDebug(_T("Error getting info : %s"),
				   m_pDB->DataBaseGetLastError().c_str());
		return false;
	}
	
	
	values.Clear();
	tmAttributionBasic myAttrib;
	wxArrayLong myRetValues;
	bool bGetNextResult = true;
	for (unsigned int i = 0; i<m_SelIDs->GetCount();i++)
	{
		myAttrib.m_Oid = m_SelIDs->Item(i);
		while (1)
		{
			if (bGetNextResult)
			{
				myRetValues.Clear();
				m_pDB->DataBaseGetNextResultAsLong(myRetValues);
			}
				
			if (myRetValues.GetCount() != 2)
			{
				break;
			}
			
			if (myRetValues.Item(0)== myAttrib.m_Oid)
			{
				if (myRetValues.Item(1) != 0)
					myAttrib.m_Values.Add(myRetValues.Item(1));
			}
			else
			{
				bGetNextResult = false;
				break;
			}
			bGetNextResult = true;
			
		}
		
		values.Add(myAttrib);
		myAttrib.m_Values.Clear();
	}
	return true;
}



/***************************************************************************//**
 @brief Check array for similar attribution
 @details The array store the OID and an array of attribution. This function
 return true if all OID share the same attribution. Used mainly for lines
 merging.
 @param values an tmAttributionBasicArray containing OID and attribution values
 @return  true if all OID share the same attribution
 @author Lucien Schreiber (c) CREALP 2009
 @date 02 March 2009
 *******************************************************************************/
bool tmAttributionData::IsAttributionSimilar (const tmAttributionBasicArray & values)
{
	// rapid check for same number of attributions
	unsigned int NbAttribution = values.Item(0).m_Values.GetCount();
	unsigned int i = 0;
	for (i = 1; i< values.GetCount();i++)
	{
		if (values.Item(i).m_Values.GetCount() != NbAttribution)
			return false;
	}
	
	// if rapid check passed, check values
	wxArrayLong * myBaseValue = &(values.Item(0).m_Values);
	wxASSERT (myBaseValue);
	for(i=1;i<values.GetCount();i++)
	{
		wxArrayLong * myAttribValues = &(values.Item(i).m_Values);
		wxASSERT (myAttribValues);
		for (unsigned int j = 0; j < myAttribValues->GetCount();j++)
		{
			if (myBaseValue->Item(j) != myAttribValues->Item(j))
				return false;
		}
		
	}
	return true;
}


/***************************************************************************//**
 @brief Get Basic attribution string from database
 @param myAttribObj object of type #tmAttributionBasic
 @param txtvalues If function return true, This array will be filled with basic
 attribution strings
 @return  true if attribution values returned succesfully, false otherwise. If true is
 returned, txtvalues may still be empty = no attributions.
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 March 2009
 *******************************************************************************/
bool tmAttributionData::GetBasicNameFromID (const tmAttributionBasic & myAttribObj, 
											wxArrayString & txtvalues)
{
	// check for no attributions
	if (myAttribObj.m_Values.GetCount() == 0)
	{
		txtvalues.Clear();
		return true;
	}
	
	wxString sSentence = _T("SELECT OBJECT_DESC FROM ") + TABLE_NAME_OBJECTS +
	_T(" WHERE OBJECT_ID IN (");
	
	for (unsigned int i = 0; i< myAttribObj.m_Values.GetCount();i++)
	{
		sSentence.Append(wxString::Format(_T("%d,"), myAttribObj.m_Values.Item(i)));
	}
	sSentence.RemoveLast(1);
	sSentence.Append(_T(");"));
	
	wxASSERT (m_pDB);
	if (!m_pDB->DataBaseQuery(sSentence))
	{
		wxLogDebug(_T("Error getting name from id %s, %s"),
				   m_pDB->DataBaseGetLastError().c_str(),
				   sSentence.c_str());
		return false;
	}
	
	wxString myTmpBuffer;
	txtvalues.Clear();
	while(1)
	{
		if (!m_pDB->DataBaseGetNextResult(myTmpBuffer))
		{
			break;
		}
		txtvalues.Add(myTmpBuffer);
	}
	
	return true;
}



/***************************************************************************//**
 @brief Get Concatened basic attribution values for object
 @param myAttrib Contain attribution for all selected objects
 @param concatenedattrib an array of string containing all attribution basic
 values concatenated
 @return true if function return successfully, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 March 2009
 *******************************************************************************/
bool tmAttributionData::GetConcatenedBasicName (const tmAttributionBasicArray & myAttrib,
												wxArrayString & concatenedattrib)
{
	wxArrayString myResults;
	wxString myConcat;
	for (unsigned int i = 0; i< myAttrib.GetCount(); i++)
	{
		if (!GetBasicNameFromID(myAttrib.Item(i), myResults))
		{
			return false;
		}
		
		if (myResults.GetCount() > 0)
		{
			myConcat = myResults.Item(0);
			for (unsigned int j = 1; j< myResults.GetCount(); j++)
			{
				myConcat.Append(_T(", ") + myResults.Item(j));
			}
			concatenedattrib.Add(myConcat);
			
		}
		else
		{
			concatenedattrib.Add(_("No attribution"));	
		}
		
		
	}
	
	return true;
}



/***************************************************************************//**
 @brief Get Layer attributed for selected geometry
 @details When doing advanced attribution, we should know what are the layers we
 should display control for.
 @param geomid the object id we search attribution layers
 @param layersid an (empty) wxArrayLong used by this function for storing
 returned layers id
 @param layertype one of the following value :
 - LAYER_SPATIAL_LINE
 - LAYER_SPATIAL_POINT
 - LAYER_SPATIAL_POLYGON
 @param tablename the name of the table containing the basic attribution.
 @return  true if layers where returned, false otherwise (no basic attribution
 for exemple or error in the query)
 @author Lucien Schreiber (c) CREALP 2009
 @date 16 March 2009
 *******************************************************************************/
bool tmAttributionData::PrepareGetAttributionLayersID (const long & geomid, 
													   wxArrayLong & layersid,
													   const wxString & tablename,
													   int layertype )
{
	wxASSERT (layersid.GetCount() == 0);
	wxASSERT (!tablename.IsEmpty());
	wxASSERT (layertype >= LAYER_SPATIAL_LINE && layertype < LAYER_SPATIAL_POLYGON);
	
	wxString sTmp = _T("SELECT l.THEMATIC_LAYERS_LAYER_INDEX FROM %s l LEFT")
					_T(" JOIN (%s a, %s t) ON (l.OBJECT_ID = a.OBJECT_VAL_ID ")
					_T("AND t.LAYER_INDEX=l.THEMATIC_LAYERS_LAYER_INDEX) WHERE")
					_T(" a.OBJECT_GEOM_ID = %d AND t.TYPE_CD=%d ORDER BY l.THEMATIC_LAYERS_LAYER_INDEX;");
	
	wxString sSentence = wxString::Format(sTmp,
										  TABLE_NAME_OBJECTS.c_str(),
										  tablename.c_str(),
										  TABLE_NAME_LAYERS.c_str(),
										  geomid,
										  layertype);
	wxASSERT (m_pDB);
	if (!m_pDB->DataBaseQuery(sSentence, true))
		return false;
	
	long myLayerTemp = wxNOT_FOUND;
	while(1)
	{
		myLayerTemp = m_pDB->DataBaseGetNextResultAsLong();
		if (myLayerTemp == wxNOT_FOUND)
			break;
		layersid.Add(myLayerTemp);
	}
	
	if (layersid.GetCount() > 0)
		return true;
	
	wxLogDebug(_T("No basic attribution, advanced attribution not availlable"));
	return false;
}



/***************************************************************************//**
 @brief Prepare advanced attribution statement
 @param statement adress of the statement to fill
 @param layer pointer to a valid ProjectDefMemoryLayer (don't destroy)
 @param values Array of string (not all values will be used)
 @param startvalues Where to start in the values array.
 @return  the number of fields used (used for recursivity)
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 March 2009
 *******************************************************************************/
int tmAttributionData::PrepareAAttribStatement (wxString & statement,
												ProjectDefMemoryLayers * layer,
												const wxArrayString & values,
												int startvalues)
{
	
	
	return 0;
}



