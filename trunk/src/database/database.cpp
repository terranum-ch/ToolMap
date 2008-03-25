/***************************************************************************
								database.cpp
				Basic function for connecting a MySQL embedded database 
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

/*!
    @file database.cpp
    @brief code for the class DataBase
	
	The Database class is used for Opening, creating and processing request
	to a MySQL embedded Database.
    @author Lucien Schreiber (c) CREALP 2007
*/


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "database.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "database.h"
#include <wx/filename.h> // to create the database path and name.


// static members
bool DataBase::bIsLibInit = FALSE;



//----------------------------------------------------------------------------
// DataBase
//----------------------------------------------------------------------------
DataBase::DataBase()
{
	IsDatabaseOpen = FALSE;
}

DataBase::~DataBase()
{
	
}



bool DataBase::DataBaseOpen (wxString path, enum Lang_Flag flag)
{
	bool Bsucces = FALSE;
	
	// conversion from path, return values in m_DBName and m_DBPath
	DataBaseConvertFullPath(path);
	
	// converting the path for being compatible with mysql
	// converting only in windows
	wxString myCorrectPathName = DataBaseConvertMYSQLPath (m_DBPath);
	
	wxString datadir = _T("--datadir=") + myCorrectPathName;
	
	// convertion to char *.... no other way ?
	int iLen = datadir.Len();
	char * stemps = new char[iLen+1];
	for (int i =0;i<=iLen;i++)
	{
		stemps[i] = datadir.GetChar(i);
	}
	
#ifdef __WINDOWS__
	 char *server_args[] = 
	{
		"this_program",       /* this string is not used */
		stemps,
		"--language=./share/english",
		//"--skip-plugin-innodb", // remove this line if comiled without innodb...
		"--port=3309",
		"--character-sets-dir=./share/charsets",
		"--default-character-set=utf8"
	};
#else
	char *server_args[] = 
	{
		"this_program",       /* this string is not used */
		stemps,
		//"--language=./share/english",
		//"--skip-plugin-innodb",
		"--port=3309",
		//"--character-sets-dir=./share/charsets",
		//"--default-character-set=cp1250"
	};
#endif
	
	 char *server_groups[] = {
		"embedded",
		"server",
		"this_program_SERVER",
		(char *)NULL
	};

	
	int num_elements = (sizeof(server_args) / sizeof(char *));

	
	if(mysql_library_init(num_elements, server_args, server_groups)==0)
	{
		// the lib was initialised so we must end the lib when quitting the program
		bIsLibInit = TRUE;
		
		pMySQL = mysql_init(NULL);
		 mysql_options(pMySQL, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);

		wxLogDebug(m_DBName);
		if(mysql_real_connect(pMySQL,NULL,NULL,NULL,(const char *)m_DBName.mb_str(wxConvUTF8),
							  3309,NULL,CLIENT_MULTI_STATEMENTS))
		{
			// change character set...
					
			IsDatabaseOpen = TRUE;
			if(DataBaseSetCharacterSet(flag))
				Bsucces = TRUE;
		}	
		
	}
	
	// if something goes wrong we return FALSE
	//delete [] server_args;
	//delete [] server_groups;
	delete [] stemps;
	return Bsucces;
	
}

bool DataBase::DataBaseClose()
{
	wxLogDebug(_T("Closing database"));
	//mysql_thread_end();	
	//mysql_library_end();
	IsDatabaseOpen = FALSE;
	return TRUE;
}


/***************************************************************************//**
 @brief End the library
 @details Call this function just before quitting the program to unload the
 mysql embedded library.
 @author Lucien Schreiber (c) CREALP 2007
 @date 11 March 2008
 *******************************************************************************/
void DataBase::DataBaseCloseLibrary()
{
	// close the library if the lib was used
	// then we must quit the program.
	if (bIsLibInit)
		mysql_library_end();
}


bool DataBase::DataBaseIsOpen()
{
return IsDatabaseOpen;
}

wxArrayString DataBase::DataBaseListTables()
{
	MYSQL_RES *results;
	MYSQL_ROW record;
	wxArrayString myStingArray;
	
	if(mysql_query(pMySQL,"SHOW TABLES;")==0)
	{
		results = mysql_store_result(pMySQL);
		
		while((record = mysql_fetch_row(results)))
		{
			
			myStingArray.Add(wxString(record[0],wxConvUTF8));
		}
		
		// clean
		mysql_free_result(results);
		
	}
	return myStingArray;
}


wxString DataBase::DatabaseGetVersion()
{
	wxString myVersionString;
	myVersionString = wxString(mysql_get_client_info(),wxConvUTF8);
	return myVersionString;
}



wxString DataBase::DataBaseGetLastError()
{
	wxString reterr = wxString::FromAscii(mysql_error(pMySQL));
	return reterr;
	
}


wxArrayString DataBase::DatabaseListFields(wxString sTable)
{
	MYSQL_RES *results;
	MYSQL_ROW record;
	wxArrayString myStingArray;
	wxString myQuery = _T("SHOW COLUMNS FROM ") + sTable;
	
	if(mysql_query(pMySQL,(const char *)myQuery.mb_str(wxConvUTF8))==0)
	{
		results = mysql_store_result(pMySQL);
		while((record = mysql_fetch_row(results)))
		{
			myStingArray.Add(wxString(record[0], wxConvUTF8));
		}
		
		
		
		// clean
		mysql_free_result(results);
		
	}
	return myStingArray;

}

bool DataBase::DataBaseGetAllTableContent(wxString sTable)
{
	m_resultNumber = 0;
	wxString myQuery = _T("SELECT * FROM ") + sTable;
	
	if (mysql_query(pMySQL, (const char *)myQuery.mb_str(wxConvUTF8))==0) 
	{
		pResults = mysql_store_result(pMySQL);
		m_resultNumber = mysql_field_count(pMySQL);
		return TRUE;
	}
	return FALSE;
}


wxArrayString DataBase::DataBaseGetNextResult()
{
	MYSQL_ROW record;
	wxArrayString myRowResult;
	
	if (m_resultNumber > 0 && pResults != NULL)
	{
		record = mysql_fetch_row(pResults);
		if(record != NULL)
		{
			for (int i = 0; i<m_resultNumber; i++) 
			{
				myRowResult.Add(wxString ( record[i], wxConvUTF8));
			}
		}
		else 
		{
			// clean
			m_resultNumber=0;
			mysql_free_result(pResults);
			pResults = NULL;
		}		
	}

	return myRowResult;
}


bool DataBase::DataBaseGetNextResult(wxString & result)
{
	MYSQL_ROW record;
	
	if (m_resultNumber > 0 && pResults != NULL)
	{
		record = mysql_fetch_row(pResults);
		if(record != NULL)
		{
			result = wxString ( record[0], wxConvUTF8);
			return TRUE;
		}
		else 
		{
			// clean
			m_resultNumber=0;
			mysql_free_result(pResults);
			pResults = NULL;
		}		
	}
	return FALSE;
}


unsigned long * DataBase::DataBaseGetNextRowResult (MYSQL_ROW & row)
{
	// check for results and return raw row result :-)
	if (m_resultNumber > 0 && pResults != NULL)
	{
		row = mysql_fetch_row(pResults);
		if(row != NULL)
		{
			return  mysql_fetch_lengths( pResults );
		}
		else 
		{
			// clean
			m_resultNumber=0;
			mysql_free_result(pResults);
			pResults = NULL;
		}
	}
	return NULL;
}



bool DataBase::DataBaseTableExist(const wxString & tableName)
{
	MYSQL_ROW record;
	// look for an existing table
	wxString sSentence = wxString::Format (_T("SHOW TABLES  LIKE  '%s'"), tableName.c_str());
	

	if (DataBaseQuery(sSentence))
	{
		// if the query has passed...
		// then we check the results
		record = mysql_fetch_row(pResults);
		if(record != NULL)
		{
			return TRUE;
		}
	}
	return FALSE;
	
	
}






int DataBase::DataBaseGetResultAsInt()
{
	MYSQL_ROW record;
	int iReturnedValue = -1;
	
	if (m_resultNumber > 0 && pResults != NULL)
	{
		record = mysql_fetch_row(pResults);
		if(record != NULL)
		{
			 iReturnedValue =  atoi(record[0]);
		}
		else 
		{
			// clean
			m_resultNumber=0;
			mysql_free_result(pResults);
			pResults = NULL;
		}		
	}
	
	return iReturnedValue;

}


double DataBase::DataBaseGetResultAsDouble()
{
	//double dReturnedValue = -1.0;
	
	//if (DataBaseHasResult())
	//{
	//	dReturnedValue = atof(m_Result[1]);
	//	sqlite3_free_table(m_Result);
	//}
	//return dReturnedValue;
	return 0;
}



bool DataBase::DataBaseIsTableEmpty(const wxString & tableName)
{
	wxString sSentence = wxString::Format(_T("SELECT * FROM %s"), tableName.c_str());
	
	// return TRUE if the sentence was OK and
	// return no results (table is empty)
	if (DataBaseQuery(sSentence))
		if(DataBaseHasResult() == FALSE)
			return TRUE;
	
	// table is not empty
	wxLogDebug (_T("Table [%s] is not empty or request error"), tableName.c_str());
	return FALSE;
}



bool DataBase::DataBaseQueryNoResult(wxString myQuery)
{
	MYSQL_RES *results;
	int iRetour = mysql_query(pMySQL, (const char*)myQuery.mb_str(wxConvUTF8) );
	results = mysql_store_result(pMySQL);
	mysql_free_result(results);
	
	if (iRetour == 0)
		return TRUE;
	
	return FALSE;
}

bool DataBase::DataBaseQuery(const wxString & myQuery)
{
	pResults = NULL;
	int iRetour = mysql_query(pMySQL, (const char*)myQuery.mb_str(wxConvUTF8) );
	if (iRetour == 0) 
	{
		pResults = mysql_store_result(pMySQL);
		m_resultNumber = mysql_field_count(pMySQL);
		return TRUE;
	}
	return FALSE;
}


bool DataBase::DataBaseHasResult ()
{
	
	if (pResults != NULL)
	{
		// if we have some results ?
		if( mysql_num_rows(pResults) > 0)
			return TRUE;
	}
	return FALSE;
}

long DataBase::DatabaseGetCountResults()
{
	if (pResults != NULL && mysql_num_fields(pResults) > 0)
	{
		return mysql_num_rows(pResults);
	}
	return 0;
}


void DataBase::DataBaseDestroyResults ()
{
	if (pResults != NULL && DataBaseHasResult())
	{
		mysql_free_result(pResults);
		pResults = NULL;
	}
}



int DataBase::DataBaseQueryMultiple (const wxString & myQuery)
{
	int iReturnValue = 0;
	
	if (iReturnValue == 0)
	{
		iReturnValue = mysql_query(pMySQL,(const char*)myQuery.mb_str(wxConvUTF8));
	}
	
	return iReturnValue;
}


int DataBase::DataBaseQueryReal (wxString myQuery)
{
	MYSQL_RES *results;
	//const char * mySentence = myQuery.c_str(); //mb_str(wxConvUTF8);

	int iRetour = mysql_query(pMySQL,(const char*)myQuery.mb_str(wxConvUTF8));
	results = mysql_store_result(pMySQL);
	mysql_free_result(results);
	return iRetour;

}

bool DataBase::DataBaseConvertFullPath(wxString fullpath)
{
	wxArrayString myDirArray;
	
	wxFileName dirname = wxFileName(fullpath,wxEmptyString);
	
	//int iNumDir = dirname.GetDirCount();
	myDirArray = dirname.GetDirs();
	m_DBName = myDirArray.Last();
	dirname.RemoveLastDir(); 
	m_DBPath = dirname.GetPath();
	
	if (m_DBPath.IsEmpty() || m_DBName.IsEmpty())
	{
		return FALSE;
	}
	return TRUE;
}


wxString DataBase::DatabaseGetCharacterSet()
{
	// MY_CHARSET_INFO cs;
	// mysql_get_character_set_info(pMySQL,&cs);
	 
	// wxString sCharName(cs.csname, wxConvUTF8);
	 
	 // compatibility with mysql 4...
	 wxString sCharName(mysql_character_set_name (pMySQL), wxConvUTF8);
	 return sCharName;

}

wxString DataBase::DataBaseGetName()
{
	return m_DBName;
}

wxString DataBase::DataBaseGetPath()
{
	return m_DBPath;
}

bool DataBase::DataBaseCreateNew(wxString DataBasePath, wxString DataBaseName,enum Lang_Flag Flag)
{
	bool BSucces = FALSE;

	// converting the path for being compatible with mysql
	// converting only in windows
	wxString myCorrectPathName = DataBaseConvertMYSQLPath (DataBasePath);
	
	wxString datadir = _T("--datadir=") + myCorrectPathName;

	// convertion to char *.... no other way ?
	int iLen = datadir.Len();
	char * stemps = new char[iLen+1];
	for (int i =0;i<=iLen;i++)
	{
		stemps[i] = datadir.GetChar(i);
	}


/*	if we uses the pre-compiled MySQL from MySQL A.B.
	then we need to skip the innodb engine and we
	alsa have to specify a path for language and
	character-sets even if we don't use it because
	of utf. Otherwise it leads to a crash without
	futher explanation.
*/
#ifdef __WINDOWS__
	char *server_args[] = 
	{
		"this_program",       /* this string is not used */
		stemps,
		"--language=./share/english",
		"--skip-plugin-innodb",//"--skip-innodb", // dosen't exist in 5.1 --> lead to a crash
		"--port=3309",
		"--character-sets-dir=./share/charsets",
		"--character_set_server=utf8"
		//"--default-collation=utf8"
	};
/*	Those server_args could be used for home-made
	MySQL libs (unix and mac) without innodb engine
	and with default character-set set to utf8
*/
#else	
	char *server_args[] = 
	{
		"this_program",       /* this string is not used */
		stemps, // "--language=./share/english", // not needed if home-made mySQL
		"--port=3309" //,"--character-sets-dir=./share/charsets"
	};
#endif
	
	
	char *server_groups[] = {
		"embedded",
		"server",
		"this_program_SERVER",
		(char *)NULL
	};

	
	
	int num_elements = (sizeof(server_args) / sizeof(char *));
	
	int ierror = mysql_library_init(num_elements, server_args, server_groups);
	if(ierror==0)
	{
		pMySQL = mysql_init(NULL);	
		 mysql_options(pMySQL, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);

		
		if(mysql_real_connect(pMySQL,NULL,NULL,NULL,NULL,3309,NULL,0))
		{
			wxString myDBName (DataBaseName);
			myDBName.Prepend(_T("CREATE DATABASE "));
			myDBName.Append (_T(" DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"));
			
			if(mysql_query(pMySQL,(const char *)myDBName.mb_str(wxConvUTF8)) ==0)
			{
				// connect to the database
				if(mysql_real_connect(pMySQL,NULL,NULL,NULL,
					(const char *)DataBaseName.mb_str(wxConvUTF8),3309,NULL,CLIENT_MULTI_STATEMENTS))
				{		
					m_DBPath = DataBasePath;
					m_DBName = DataBaseName;
					IsDatabaseOpen = TRUE;
					if(DataBaseSetCharacterSet(Flag))
						BSucces = TRUE;
				}
			}
		}	
		
	}
	// if something goes wrong
	delete stemps;
	return BSucces;
}


wxArrayString DataBase::DataBaseCutRequest (wxString theRequest)
{
	wxArrayString myArrayRequest;
	myArrayRequest = wxStringTokenize (theRequest,_T(";"));
	return myArrayRequest;
}

wxString DataBase::DataBaseConvertMYSQLPath(wxString originalPath)
{
	wxArrayString myNewName;
	wxString myReturnPath;
	wxFileName myOriginalName = wxFileName(originalPath,wxEmptyString);
	
	// get the separator
	wxString mySeparator = myOriginalName.GetPathSeparator();
	
	// if separator != '/' we process the filename
	if ( mySeparator != _T("/") )
	{
		myNewName = wxStringTokenize(originalPath,_T("\\"));
		for (int i=0; i < (int)myNewName.Count(); i++)
		{
		myReturnPath += myNewName.Item(i);
		myReturnPath += _T("/");	
		}
		
	}
	else 
	{
		return originalPath;
	}

	return myReturnPath;
}


wxString DataBase::DataBaseGetSize (int iPrecision)
{
	wxLongLong myBigSize;
	double dMegaBytes = 0.0;
	
	wxFileName myDataBasePath = wxFileName(m_DBPath,wxEmptyString);
	myDataBasePath.AppendDir(m_DBName);
	wxString myDataBasePathName = myDataBasePath.GetPath();
	if (wxDir::Exists(myDataBasePathName))
	{
		// compute the size
		myBigSize = wxDir::GetTotalSize(myDataBasePathName);
		// modifiy the size to be MB
		dMegaBytes =  (myBigSize.ToDouble() / 1024) / 1024;
		return  wxString::Format(_T("%.*f [MB]"),iPrecision,dMegaBytes);
	}
	
	return _("The Directory dosen't exist.");
	
}

bool DataBase::DataBaseSetCharacterSet (enum Lang_Flag myFlag)
{
	wxString myRequest;

	switch(myFlag)
	{

	case LANG_LATIN1:
		myRequest = _T("SET CHARACTER SET 'latin1'");
		break;

	default:
	case LANG_UTF8:
		myRequest = _T("SET CHARACTER SET 'utf8'");
		break;
	};

	if(mysql_query(pMySQL,(const char*)myRequest.mb_str(wxConvUTF8))==0)
	{
		return TRUE;
	}


	return FALSE;
}

