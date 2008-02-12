/***************************************************************************
								db_project.h.h
		deals with the creation and modification of a database project
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


#include "db_project.h"


DatabaseNewPrj::DatabaseNewPrj()
{
	InitMembersValues();
}


DatabaseNewPrj::DatabaseNewPrj(PrjDefMemManage * myPrjDefinition)
{
	InitMembersValues();
	SetPrjDefMemory(myPrjDefinition);
}


DatabaseNewPrj::~DatabaseNewPrj()
{

}


void DatabaseNewPrj::InitMembersValues()
{
	pPrjDefinition = NULL;
}

bool DatabaseNewPrj::IsPrjDefMemorySet()
{
	if (pPrjDefinition != NULL)
		return TRUE;
	return FALSE;
}


bool DatabaseNewPrj::CreateEmptyProject()      
{
	// check for PrjDefMemory is stored in memory 
	wxASSERT_MSG(IsPrjDefMemorySet(), _T("Use SetPrjDefMemory first..."));
	if (IsPrjDefMemorySet())
	{
		// show the path and name...
		wxLogDebug(_T("Path : %s \n Name : %s"),pPrjDefinition->m_PrjPath.c_str(), pPrjDefinition->m_PrjName.c_str());
		
		// create the database
		if (DataBaseCreateNew(pPrjDefinition->m_PrjPath, 
							  pPrjDefinition->m_PrjName))
		{
			wxLogMessage(_("Creating the \"%s\" database"), pPrjDefinition->m_PrjName.c_str());
			
			
			// create the default tables
			if (CreateEmptyTMDatabase())
			{
				wxLogDebug(_("Default tables created in the database"));
				return TRUE;
			}
		
		
		}
		wxLogDebug(_T("Error creating the database"));
		
		
	}
	return FALSE; // no project created....
}

bool DatabaseNewPrj::PassProjectDataToDB()
{
	unsigned int indexLayer = 0;
	unsigned int indexObject = 0;
	unsigned int indexField = 0;
	bool bReturnValue = TRUE;
	
	wxArrayString myDbgCharacterVar;
	
	//// show character sets  // FOR DEBUGING
	//int iRetour = DataBaseQuery(_T("SHOW VARIABLES LIKE 'character_set_\%'"));
	//while(1)
	//{
	//	myDbgCharacterVar = DataBaseGetNextResult();
	//	if (myDbgCharacterVar.GetCount() > 0)
	//	{
	//		wxLogDebug(_T("%s | %s"),myDbgCharacterVar.Item(0).c_str(), myDbgCharacterVar.Item(1));
	//	}
	//	else
	//		break;
	//}



	/// adding layers, and for each layer add object and fields related to this layer
	for (indexLayer = 0; indexLayer<pPrjDefinition->m_PrjLayerArray->GetCount();indexLayer++)
	{
		ProjectDefMemoryLayers * myMemLayers = 	pPrjDefinition->FindLayer(indexLayer);
		if (myMemLayers != NULL)
		{
			if (AddLayer(myMemLayers))
			{
				
				// Adding Objects 
				for (indexObject = 0;indexObject < myMemLayers->m_pLayerObjectArray->GetCount(); indexObject++)
				{
					ProjectDefMemoryObjects * myMemObj = pPrjDefinition->FindObject(indexObject);
					if (myMemObj != NULL)
						bReturnValue &= AddObject(myMemObj);
				}
				
				// adding fields
				for (indexField = 0; indexField < myMemLayers->m_pLayerFieldArray->GetCount(); indexField++)
				{
					ProjectDefMemoryFields * myMemField = pPrjDefinition->FindField(indexField);
					if (myMemField != NULL)
						bReturnValue &= AddField(myMemField);
				}
				
			}
			wxLogDebug(_T("Adding layer OK"));
		}
	}


	//// show values passed  // FOR DEBUGING
	//int iRetour = DataBaseQuery(_T("SELECT (LAYER_NAME) FROM THEMATIC_LAYERS"));
	//while(1)
	//{
	//	myDbgCharacterVar = DataBaseGetNextResult();
	//	if (myDbgCharacterVar.GetCount() > 0)
	//	{
	//		wxLogDebug(_T("layer values : %s"),myDbgCharacterVar.Item(0).c_str());
	//	}
	//	else
	//		break;
	//}


	
	return bReturnValue;
}




