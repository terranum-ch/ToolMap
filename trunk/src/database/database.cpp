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

#include "database-config.h" // for logging
#include <wx/stdpaths.h>	// std path for logging too.



DataBase::DataBase()
{
	m_IsDatabaseOpened		= false;
	m_IsLibraryStarted		= false;
	m_MySQL					= NULL;
	m_MySQLRes				= NULL;
	m_DBName				= wxEmptyString;
	m_DBPath				= wxEmptyString;

}



DataBase::~DataBase()
{
	if (DBResultsNotNull()==true)
		DataBaseClearResults();

	if (m_IsLibraryStarted)
	{
		DBLibraryEnd();
		m_IsLibraryStarted = false;
	}
}



bool DataBase::DBLibraryInit (const wxString & datadir)
{

	// path validity
	wxFileName myValidPath (datadir, _T(""));
	if (myValidPath.IsDirReadable()==false)
	{
		wxLogError(_("Directory : %s doesn't exists or isn't readable"),datadir.c_str());
		return false;
	}

	//init library
	wxString myDatadir = _T("--datadir=") + myValidPath.GetPath(wxPATH_GET_VOLUME,wxPATH_NATIVE);
	char * bufDataDir = new char[myDatadir.Len() * sizeof(wxString)];
	strcpy( bufDataDir, (const char*)myDatadir.mb_str(wxConvUTF8));

#if defined(__WINDOWS__)
	char * mylanguagedir = "--language=./mysql";
#elif defined(__WXMAC__)
	char * mylanguagedir =	"--language=./ToolMap2.app/Contents/mysql";
#elif defined(__WXGTK20__)
	char * mylanguagedir = "--language=./mysql";
#else
	wxASSERT_MSG (0, _T("Check compilation option for MySQL"));
	char * mylanguagedir = "";
#endif

	wxFileName myLogDirName (wxStandardPaths::Get().GetDocumentsDir(),_T("toolmap_mysql_debug_log.txt"));
	wxString myLogDirString = _T("--log=");
	myLogDirString.Append(myLogDirName.GetFullPath());
#if defined (MYSQL_IS_LOGGING)
	char * bufLogPath = new char[myLogDirString.Len() * sizeof(wxString)];
	strcpy(bufLogPath, (const char*)myLogDirString.mb_str(wxConvUTF8));
#endif


	char *server_args[] =
	{
		"this_program",       /* this string is not used */
		bufDataDir,
		mylanguagedir,
		"--port=3309"
#if defined (MYSQL_IS_LOGGING)
		, bufLogPath
#endif
		//"--character-sets-dir=./share/charsets",
		//"--default-character-set=utf8"
	};

	char *server_groups[] =
	{
		"embedded",
		"server",
		"this_program_SERVER",
		(char *)NULL
	};


	int num_elements = (sizeof(server_args) / sizeof(char *));
	int myReturn = mysql_library_init(num_elements, server_args, server_groups);
	
#if defined (MYSQL_IS_LOGGING)
	delete [] bufLogPath; 
#endif	
	
	if (myReturn != 0)
	{
		delete [] bufDataDir;
		DBLogLastError();
		return false;
	}

	delete [] bufDataDir;
	m_MySQL = mysql_init(NULL);
	mysql_options(m_MySQL, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);
	wxLogDebug(_T("Initing MySQL library..."));
	return true;
}



bool DataBase::DBUseDataBase(const wxString & dbname)
{
	char * buf = NULL;
	if (dbname.IsEmpty())
	{
		buf = new char [3];
		strcpy(buf, "");
	}
	else
	{
		buf = new char [dbname.Len() * sizeof(wxString)];
		strcpy( buf, (const char*)dbname.mb_str(wxConvUTF8));
	}

	if(mysql_real_connect(m_MySQL,NULL,NULL,NULL,buf,
						  3309,NULL,CLIENT_MULTI_STATEMENTS) == NULL)
	{
		delete [] buf;
		DBLogLastError();
		return false;
	}

	delete[] buf;
	if (dbname != wxEmptyString)
		wxLogMessage(_("Opening database : ") + dbname);

	return true;
}



void DataBase::DBLibraryEnd ()
{
	m_DBName = wxEmptyString;
	m_DBPath = wxEmptyString;

	wxLogDebug(_T("Ending MySQL library..."));
	mysql_close(m_MySQL);
	mysql_library_end();
}



void DataBase::DBLogLastError ()
{
	wxString myTextError = wxString::FromAscii(mysql_error(m_MySQL));
	wxLogError(_("MySQL Error : %s"), myTextError.c_str());
}


bool DataBase::DataBaseCreateNew(const wxString & datadir, const wxString & name)
{
	if (m_IsLibraryStarted)
	{
		DBLibraryEnd();
		m_IsLibraryStarted=false;
	}

	m_IsLibraryStarted = DBLibraryInit(datadir);
	if (m_IsLibraryStarted == false)
		return false;

	m_IsDatabaseOpened = DBUseDataBase(wxEmptyString);
	if (m_IsDatabaseOpened == false)
		return false;

	wxString myDBNewQuery (name);
	myDBNewQuery.Prepend(_T("CREATE DATABASE "));
	myDBNewQuery.Append (_T(" DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"));
	if(DataBaseQueryNoResults(myDBNewQuery)==false)
	{
		wxLogError(_("Error creating database : ") + name);
		return false;
	}

	m_IsDatabaseOpened = DBUseDataBase(name);
	if (m_IsDatabaseOpened == false)
		return false;

	m_DBName = name;
	m_DBPath = datadir;
	return true;
}


bool DataBase::DataBaseOpen(const wxString & datadir, const wxString & name)
{
	if (m_IsLibraryStarted)
	{
		DBLibraryEnd();
		m_IsLibraryStarted = false;
	}

	m_IsLibraryStarted = DBLibraryInit(datadir);
	if (m_IsLibraryStarted == false)
		return false;

	m_IsDatabaseOpened = DBUseDataBase(name);
	if (m_IsDatabaseOpened == false)
		return false;

	m_DBName = name;
	m_DBPath = datadir;

	return true;
}


bool DataBase::DataBaseDelete()
{
	if (DBIsDataBaseReady() == false)
		return false;

	if (DataBaseQuery(_T("DROP DATABASE ") + DataBaseGetName())==false)
		return false;

	long ldeltables = wxNOT_FOUND;
	if (DataBaseGetNextResult(ldeltables)==true)
		wxLogDebug(_T("%d tables deleted"), ldeltables);

	DataBaseClearResults();
	return true;
}



wxString DataBase::DataBaseGetName ()
{
	return m_DBName;
}


wxString DataBase::DataBaseGetPath ()
{
	return m_DBPath;
}



wxString DataBase::DataBaseGetSize (int precision, const wxString & failmsg)
{
	if (DBIsDataBaseReady() == false)
		return failmsg;

	wxFileName myDBFileName (DataBaseGetPath(), DataBaseGetName());
	if (myDBFileName.IsOk()==false)
		return failmsg;

	wxArrayString mySkipedFiles;
	wxULongLong myDBDirSize = wxDir::GetTotalSize(myDBFileName.GetFullPath(), &mySkipedFiles);
	if (mySkipedFiles.GetCount() > 0)
		wxLogDebug(_T("%d files skipped, size may not be 100% accurate"));


	wxString myDBStringSize = wxFileName::GetHumanReadableSize(myDBDirSize, failmsg);
	return 	myDBStringSize;
}



wxString DataBase::DataBaseGetVersion ()
{
	return wxString(mysql_get_client_info(), wxConvUTF8);
}

/***************************************************************************//**
 @brief Init internal variables for new thread
 @details Call this function to init internal values for each new
 thread.
 @return TRUE if thread init success, FALSE in case of faillure. An
 LogDebug message is also issued
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
bool DataBase::DataBaseThreadInit()
{
	if (mysql_thread_init() == 0)
	{
		return true;
	}
	else
	{
		// try to get the last error
		DBLogLastError();
		return false;
	}
}


/***************************************************************************//**
 @brief End internal variables for new thread
 @details Call this function for each thread you have called
 DataBase::DataBaseNewThreadInit()
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
void DataBase::DataBaseThreadEnd()
{
	mysql_thread_end();
}



bool DataBase::DataBaseHasResults()
{
	if (DBResultsNotNull()==false)
		return false;

	long lrow = 0;
	if (DataBaseGetResultSize(NULL, &lrow)==false)
	{
		wxLogError(_T("Unable to compute number of results"));
	}

	if (lrow <= 0)
	{
		DataBaseClearResults();
		return false;
	}

	return true;
}


void DataBase::DataBaseClearResults()
{
	if (m_MySQLRes != NULL)
	{
		mysql_free_result(m_MySQLRes);
		m_MySQLRes = NULL;
	}
}


bool DataBase::DataBaseGetResultSize (unsigned int * pcols, long * prows)
{
	if (DBIsDataBaseReady()==false)
		return false;

	if (DBResultsNotNull() == false)
		return false;

	if (pcols != NULL)
		*pcols = mysql_num_fields(m_MySQLRes);

	if (prows != NULL)
		*prows = mysql_num_rows(m_MySQLRes);

	return true;
}


bool DataBase::DataBaseGetNextResult(wxString & result)
{
	result = wxEmptyString;

	MYSQL_ROW record = NULL;
	if (DBGetNextRecord(record)==false)
		return false;

	result = wxString ( record[0], wxConvUTF8);
	return true;
}




bool DataBase::DataBaseGetNextResult(wxArrayString & results)
{
	results.Clear();

	MYSQL_ROW record = NULL;
	if (DBGetNextRecord(record)==false)
		return false;

	unsigned int myCols = 0;
	DataBaseGetResultSize(&myCols, NULL);
	if (myCols == 1)
	{
		wxLogDebug(_T("Only one columns returned, use the ")
				   _T("DataBaseGetNextResult(wxString) function instead of this one"));
	}

	for (unsigned int i = 0; i< myCols; i++)
	{
		results.Add(wxString ( record[i], wxConvUTF8));
	}
	return true;
}



bool DataBase::DataBaseGetNextResult(long & result)
{
	result = wxNOT_FOUND;

	MYSQL_ROW record = NULL;
	if (DBGetNextRecord(record)==false)
		return false;

	result = atol(record[0]);
	return true;
}


bool DataBase::DataBaseGetNextResult(wxArrayLong & results)
{
	results.Clear();

	MYSQL_ROW record = NULL;
	if (DBGetNextRecord(record)==false)
		return false;

	unsigned int myCols = 0;
	DataBaseGetResultSize(&myCols, NULL);
	if (myCols == 1)
	{
		wxLogDebug(_T("Only one columns returned, use the ")
				   _T("DataBaseGetNextResult(long) function instead of this one"));
	}

	for (unsigned int i = 0; i< myCols; i++)
	{
		results.Add(atol(record[i]));
	}
	return true;
}


bool DataBase::DataBaseGetNextResult(double & result)
{
	result = 0;

	MYSQL_ROW record = NULL;
	if (DBGetNextRecord(record)==false)
		return false;

	result = atof(record[0]);
	return true;
}



bool DataBase::DataBaseGetNextResult(wxArrayDouble & results)
{
	results.Clear();

	MYSQL_ROW record = NULL;
	if (DBGetNextRecord(record)==false)
		return false;

	unsigned int myCols = 0;
	DataBaseGetResultSize(&myCols, NULL);
	if (myCols == 1)
	{
		wxLogDebug(_T("Only one columns returned, use the ")
				   _T("DataBaseGetNextResult(double) function instead of this one"));
	}

	for (unsigned int i = 0; i< myCols; i++)
	{
		results.Add(atof(record[i]));
	}
	return true;
}




bool DataBase::DataBaseGetNextRowResult(MYSQL_ROW & row, tmArrayULong & lengths)
{
	row = NULL;
	lengths.Clear();


	
	if (DBGetNextRecord(row)==false)
		return false;

	unsigned int myNumFields = mysql_field_count(m_MySQL);
	wxASSERT(myNumFields > 0);
	
	unsigned long * myTempLength = mysql_fetch_lengths(m_MySQLRes);
	wxASSERT(*myTempLength != 0);
	
	for (unsigned int i =  0; i<myNumFields;i++)
		lengths.Add(myTempLength[i]);
	
	//delete [] myTempLength; (not needed ?)
	return true;
}



bool DataBase::DataBaseGetResults(wxArrayString & results)
{
	results.Clear();
	MYSQL_ROW record = NULL;

	while (1)
	{
		if (DBGetNextRecord(record)==false)
			break;

		results.Add(wxString ( record[0], wxConvUTF8));
	}

	DataBaseClearResults();
	switch (results.GetCount())
	{
		case 0:
			return false;
			break;

		case 1:
			wxLogDebug(_T("Only one value returned, maybe should use the")
					   _T(" DataBaseGetNextResult(wxString) function instead ?"));
			break;

		default:
			break;
	}
	return true;
}



bool DataBase::DataBaseGetResults(wxArrayLong & results)
{
	results.Clear();
	MYSQL_ROW record = NULL;

	while (1)
	{
		if (DBGetNextRecord(record)==false)
			break;

		results.Add(atol(record[0]));
	}


	DataBaseClearResults();
	switch (results.GetCount())
	{
		case 0:
			return false;
			break;

		case 1:
			wxLogDebug(_T("Only one value returned, maybe should use the")
					   _T(" DataBaseGetNextResult(long) function instead ?"));
			break;

		default:
			break;
	}
	return true;
}



bool DataBase::DataBaseGetResults(wxArrayDouble & results)
{
	results.Clear();
	MYSQL_ROW record = NULL;

	while (1)
	{
		if (DBGetNextRecord(record)==false)
			break;

		results.Add(atof(record[0]));
	}


	DataBaseClearResults();
	switch (results.GetCount())
	{
		case 0:
			return false;
			break;

		case 1:
			wxLogDebug(_T("Only one value returned, maybe should use the")
					   _T(" DataBaseGetNextResult(double) function instead ?"));
			break;

		default:
			break;
	}
	return true;
}



bool DataBase::DataBaseQueryNoResults(const wxString & query)
{
	if (DBIsDataBaseReady() == false)
		return false;

	if (DBResultsNotNull())
	{
		wxASSERT_MSG(0, _T("Not able to run query, results were not cleared"));
		wxLogDebug(_T("Not able to run query, results were not cleared"));
		return false;
	}

	char * buf = new char[query.Len() * sizeof(wxString)];
	strcpy( buf, (const char*)query.mb_str(wxConvUTF8));
	if (mysql_query(m_MySQL, buf) != 0)
	{
		delete [] buf;
		DBLogLastError();
		return false;
	}

	delete [] buf;
	m_MySQLRes = mysql_store_result(m_MySQL);
	DataBaseClearResults();
	return true;
}



bool DataBase::DataBaseQuery (const wxString & query)
{
	if (DBIsDataBaseReady() == false)
		return false;

	if (DBResultsNotNull())
	{
		wxASSERT_MSG(0, _T("Not able to run query, results were not cleared"));
		wxLogDebug(_T("Not able to run query, results were not cleared"));
		return false;
	}

	char * buf = new char[query.Len() * sizeof(wxString)];
	strcpy( buf, (const char*)query.mb_str(wxConvUTF8));
	if (mysql_query(m_MySQL, buf) != 0)
	{
		delete [] buf;
		DBLogLastError();
		return false;
	}
	delete [] buf;
	m_MySQLRes = mysql_store_result(m_MySQL);
	return true;
}


int DataBase::DataBaseQueriesNumber (const wxString & query)
{
	wxStringTokenizer tokenizer(query, _T(";"), wxTOKEN_DEFAULT);
	return tokenizer.CountTokens();
}

long DataBase::DataBaseGetLastInsertedID()
{
	long myIID = wxNOT_FOUND;
	if (DBIsDataBaseReady()==false)
		return myIID;

	myIID = mysql_insert_id(m_MySQL);
	if (myIID == 0)
		myIID = wxNOT_FOUND;

	return myIID;
}


bool DataBase::DBIsDataBaseReady ()
{
	if (m_IsLibraryStarted == false)
	{
		wxLogError(_("MySQL library not started"));
		return false;
	}

	if (m_IsDatabaseOpened == false)
	{
		wxLogError(_("No database open"));
		return false;
	}
	return true;
}


bool DataBase::DBGetNextRecord (MYSQL_ROW & record)
{
	if (DBIsDataBaseReady()==false)
		return false;

	if (DBResultsNotNull()==false)
		return false;


	record = mysql_fetch_row(m_MySQLRes);
	if (record == NULL)
		return false;

	return true;
}


bool DataBase::DBResultsNotNull ()
{
	if (m_MySQLRes == NULL)
		return false;

	return true;
}



#if (0)

// static members
bool DataBase::bIsLibInit = FALSE;



//----------------------------------------------------------------------------
// DataBase
//----------------------------------------------------------------------------
DataBase::DataBase()
{
	IsDatabaseOpen = FALSE;
	pMySQL = NULL;
}

DataBase::~DataBase()
{
	DataBaseClose();
}


/***************************************************************************//**
 @brief Init the MySQL library
 @details This is the first function to call. It must return 0 for continuing.
 This function log (wxLogError) in case of problems.
 @param path the path of the project we want to open
 @return 0 if succeed, other value if an error occur. (see wxLogError for
 more informations)
 @author Lucien Schreiber (c) CREALP 2009
 @date 25 February 2009
 *******************************************************************************/
int DataBase::DataBaseInitLibrary (const wxString & path)
{

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

#if defined(__WINDOWS__)
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

#elif defined(__WXMAC__)
	char *server_args[] =
	{
		"this_program",       /* this string is not used */
		stemps,
		"--language=./ToolMap2.app/Contents/share/english",
		//"--skip-plugin-innodb",
		"--port=3309",
		//"--default-character-set=utf8",
		"--lower_case_table_names=1"
		//"--character-sets-dir=./share/charsets",
		//"--default-character-set=cp1250"
	};


#elif defined(__WXGTK20__)
	char *server_args[] =
	{
		"this_program",       /* this string is not used */
		stemps,
		//"--language=./ToolMap2.app/Contents/share/english",
		//"--skip-plugin-innodb",
		"--port=3309",
		//"--default-character-set=utf8",
		//"--lower_case_table_names=1"
		//"--character-sets-dir=./share/charsets",
		//"--default-character-set=cp1250"
	};


#else
	wxASSERT_MSG (0, _T("Check compilation option for MySQL"));
	char *server_args[] =
	{
		"this_program",       /* this string is not used */
		stemps,
		"--port=3309",
	};
#endif

	char *server_groups[] = {
		"embedded",
		"server",
		"this_program_SERVER",
		(char *)NULL
	};

	wxASSERT (pMySQL==NULL);

	int num_elements = (sizeof(server_args) / sizeof(char *));
	int myReturn = mysql_library_init(num_elements, server_args, server_groups);
	if (myReturn != 0)
	{

			wxLogError(_("Return code: %d, Error: %s"), myReturn,
					   DataBaseGetLastError().c_str());
	}
	else
	{
		// the lib was initialised so we must end the lib when quitting the program
		bIsLibInit = TRUE;
	}

	delete [] stemps;
	return myReturn;
}




bool DataBase::DataBaseOpen (wxString path, enum Lang_Flag flag)
{
	bool Bsucces = FALSE;
	if(DataBaseInitLibrary(path)==0)
	{
		// the lib was initialised so we must end the lib when quitting the program
		//bIsLibInit = TRUE;

		pMySQL = mysql_init(NULL);
		mysql_options(pMySQL, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);
		mysql_thread_init();

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
	wxLogDebug(_T("Error connecting to DB : %s"), DataBaseGetLastError().c_str());
	return Bsucces;

}



bool DataBase::DataBaseClose()
{
	wxLogDebug(_T("Closing database"));
	DataBaseCloseLibrary();

	pMySQL = NULL;
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
	{
		mysql_thread_end();
		mysql_library_end();
		bIsLibInit = false;
	}

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
			m_resultCount =0;
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
			// if only one result clean imediately
			if (m_resultCount == 1)
			{
				// clean
				m_resultNumber=0;
				m_resultCount=0;
				mysql_free_result(pResults);
				pResults = NULL;
			}

			return TRUE;
		}
		else
		{
			// clean
			m_resultNumber=0;
			m_resultCount=0;
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
			m_resultCount=0;
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
	wxString sSentence = wxString::Format (_T("SHOW TABLES  LIKE  \"%s\""), tableName.c_str());


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


/***************************************************************************//**
 @brief Return the next increment
 @details When an object is inserted into an auto-incremented table, an id
 corresponding to this increment is used. This function may be used for
 guessing the new number
 @return  the new number (or -1 if something goes wrong)
 @author Lucien Schreiber (c) CREALP 2007
 @date 05 May 2008
 *******************************************************************************/
long DataBase::DataBaseGetLastInsertID ()
{
	wxString sSentence = wxString::Format(_T("SELECT LAST_INSERT_ID(); "));
	if(DataBaseQuery(sSentence))
	{
		return DataBaseGetNextResultAsLong();
	}
	else
	{
		wxLogDebug(_T("Error getting last inserted id : %s"), sSentence.c_str());
		return -1;
	}

}

/***************************************************************************//**
 @brief Return an array of long for each row
 @details For each row of results, an array of long is returned containing all
 results changed into long values. This function does no check about the values
 @param resultArray this array of long is filled by the function with all values
 from one row
 @author Lucien Schreiber (c) CREALP 2007
 @date 31 March 2008
 *******************************************************************************/
void DataBase::DataBaseGetNextResultAsLong(wxArrayLong & resultArray)
{
	MYSQL_ROW record;
	if (m_resultNumber > 0 && pResults != NULL)
	{
		record = mysql_fetch_row(pResults);
		if(record != NULL)
		{
			for (int i = 0; i<m_resultNumber; i++)
			{
				resultArray.Add(atol(record[i]));
			}
		}
		else
		{
			// clean
			m_resultNumber=0;
			m_resultCount=0;
			mysql_free_result(pResults);
			pResults = NULL;
		}
	}
}





long DataBase::DataBaseGetNextResultAsLong()
{
	long result = wxNOT_FOUND;
	MYSQL_ROW record;
	if (m_resultNumber > 0 && pResults != NULL)
	{
		record = mysql_fetch_row(pResults);
		if(record != NULL)
		{
			result = atol(record[0]);
		}
		else
		{
			// clean
			m_resultNumber=0;
			m_resultCount=0;
			mysql_free_result(pResults);
			pResults = NULL;
		}
	}
	return result;
}



int DataBase::DataBaseGetResultAsInt(bool ClearResultDirect)
{
	MYSQL_ROW record;
	int iReturnedValue = -1;

	if (m_resultNumber > 0 && pResults != NULL)
	{
		record = mysql_fetch_row(pResults);
		if(record != NULL)
		{
			 iReturnedValue =  atoi(record[0]);
			// should we clear the results imediately
			// after the first turn ?
			if (ClearResultDirect == TRUE)
			{
				// clean
				m_resultNumber=0;
				m_resultCount=0;
				mysql_free_result(pResults);
				pResults = NULL;
			}
		}
		else
		{
			// clean
			m_resultNumber=0;
			m_resultCount=0;
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



bool DataBase::DataBaseQueryNoResult(wxString myQuery, bool logerror)
{
	MYSQL_RES *results;
	int iRetour = mysql_query(pMySQL, (const char*)myQuery.mb_str(wxConvUTF8) );
	results = mysql_store_result(pMySQL);
	mysql_free_result(results);

	if (iRetour == 0)
		return true;

	if (logerror)
		wxLogDebug(_T("Error in query : %s - %s"), myQuery.c_str(),
				   DataBaseGetLastError().c_str());

	return false;
}

bool DataBase::DataBaseQuery(const wxString & myQuery, bool logerror)
{
	pResults = NULL;
	bool bReturn = false;


	int iRetour = mysql_query(pMySQL, (const char*)myQuery.mb_str(wxConvUTF8));
	if (iRetour == 0)
	{
		pResults = mysql_store_result(pMySQL);
		m_resultNumber = mysql_field_count(pMySQL);
		m_resultCount = DatabaseGetCountResults();
		bReturn = true;
	}

	if (bReturn == false && logerror)
		wxLogDebug(_T("Error in query : %s - %s"), myQuery.c_str(),
				   DataBaseGetLastError().c_str());

	return bReturn;
}



/***************************************************************************//**
 @brief Thread Safe query returning array of long
 @param myQuery The query as a string
 @return  A pointer to a valid wxArrayLong (user should destroy the array when
 finished), or null if no results
 @author Lucien Schreiber (c) CREALP 2008
 @date 08 November 2008
 *******************************************************************************/
wxArrayLong * DataBase::DataBaseQuerySafe (const wxString & myQuery)
{


	DataBaseDestroyResults();

	// start new thread
	if (!DataBaseNewThreadInit())
		return NULL;


	int iRetour = mysql_query(pMySQL, (const char*)myQuery.mb_str(wxConvUTF8));
	MYSQL_RES * myResults = NULL;
	if (iRetour == 0)
	{
		myResults = mysql_store_result(pMySQL);
	}

	MYSQL_ROW record;
	wxArrayLong * myRetArray = new wxArrayLong ();

	if (myResults)
	{
		while (1)
		{
			record = mysql_fetch_row(myResults);
			if (!record)
				break;

			myRetArray->Add(atol(record[0]));
		}


		mysql_free_result(myResults);
	}

	// uninit thead
	DataBaseNewThreadUnInit();

	if (myRetArray->GetCount() < 1)
	{
		delete myRetArray;
		return NULL;
	}

	return myRetArray;

}


/***************************************************************************//**
 @brief Check results
 @details This function checks if a result exists AND isn't empty.
 @see For
 checking only the existence of a result (even if this result is empty) :
 DataBase::DataBaseResultExists()
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 May 2008
 *******************************************************************************/
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


/***************************************************************************//**
 @brief Check results
 @details This function only checks if a result exists. No check is done to know
 if this result is or isn't empty
 @see Function checking if result isn't empty is : DataBase::DataBaseHasResult()
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 May 2008
 *******************************************************************************/
bool DataBase::DataBaseResultExists()
{
	if (pResults != NULL)
		return TRUE;
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

int DataBase::DatabaseGetCountCols()
{
	if (pResults)
		return mysql_num_fields(pResults);
	return -1;
}


void DataBase::DataBaseDestroyResults ()
{
	if (pResults != NULL) // && DataBaseHasResult())
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

	wxFileName myFullName (DataBasePath, _T(""));
	myFullName.AppendDir(DataBaseName);
	int ierror = DataBaseInitLibrary(myFullName.GetFullPath());
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
	if (BSucces == false)
		wxLogError(_T("Error Creating project : %s"), DataBaseGetLastError().c_str());


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

		myReturnPath.RemoveLast();

	}
	else
	{
		return originalPath;
	}

	return myReturnPath;
}

/***************************************************************************//**
 @brief Convert path from windows path
 @details This function replace \ with \\ in the specified path.
 @note This functions does nothing for path containing / (like unix path)
 @param spath the path to change
 @author Lucien Schreiber (c) CREALP 2007
 @date 28 March 2008
 *******************************************************************************/
void DataBase::DataBaseConvertWindowsPath (wxString & spath)
{
	wxArrayString myNewNameArray;
	wxFileName myDirName =  wxFileName::DirName(spath);

	// get the separator
	wxString mySeparator = myDirName.GetPathSeparator();

	// if we detect a windows separator
	if (mySeparator == _T("\\"))
	{

		myNewNameArray = wxStringTokenize(spath,_T("\\"));

		// clear the original path
		spath.Clear();

		for (unsigned int i = 0; i< myNewNameArray.GetCount(); i++)
		{
			spath += myNewNameArray.Item(i);
			spath += _T("\\\\"); // add two \ for windows
		}
		// remove last \ (2x)
		spath.RemoveLast(2);
		wxLogDebug(_T("Path converted for windows : %s"), spath.c_str());
	}

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



/***************************************************************************//**
 @brief Init internal variables for new thread
 @details Call this function to init internal values for each new
 thread.
 @return TRUE if thread init success, FALSE in case of faillure. An
 LogDebug message is also issued
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
bool DataBase::DataBaseNewThreadInit()
{
	if (mysql_thread_init() == 0)
	{

		return TRUE;
	}
	else
	{
		// try to get the last error
		wxLogDebug(DataBaseGetLastError());
		return FALSE;
	}
}


/***************************************************************************//**
 @brief End internal variables for new thread
 @details Call this function for each thread you have called
 DataBase::DataBaseNewThreadInit()
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
void DataBase::DataBaseNewThreadUnInit()
{
	mysql_thread_end();
}



/***************************************************************************//**
 @brief Send query to the database
 @details This query may be used for storing binary data into the database
 because no conversion are done for the 'query' parameter
 @param query contain the query, no transformations done
 @param DestroyResult if = TRUE, we destroy results imediately
 @return  TRUE if query passed, false otherwise (with a debug message in debug
 mode)
 @author Lucien Schreiber (c) CREALP 2008
 @date 03 September 2008
 *******************************************************************************/
bool DataBase::DataBaseQueryBinary(const char * query,  bool DestroyResult)
{
	pResults = NULL;

	int iRetour = mysql_query(pMySQL, query);
	if (iRetour == 0)
	{

		if (!DestroyResult)
		{
			pResults = mysql_store_result(pMySQL);
			m_resultNumber = mysql_field_count(pMySQL);
			m_resultCount = DatabaseGetCountResults();
		}
		return TRUE;
	}

	// try to get the last error
	wxLogDebug(DataBaseGetLastError());
	return FALSE;

}
#endif
